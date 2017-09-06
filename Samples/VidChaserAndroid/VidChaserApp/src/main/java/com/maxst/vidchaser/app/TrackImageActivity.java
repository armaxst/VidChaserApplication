/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.app.Activity;
import android.app.Dialog;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.maxst.vidchaser.ProjectionMatrix;
import com.maxst.vidchaser.VidChaserAPI;
import com.maxst.videoplayer.VideoPlayer;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import butterknife.Bind;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class TrackImageActivity extends AppCompatActivity implements View.OnTouchListener {

	private static final String TAG = TrackImageActivity.class.getName();

	@Bind(R.id.image_size)
	TextView imageSizeTextView;

	@Bind(R.id.engine_version)
	TextView engineVersion;

	@Bind(R.id.gl_surface_view)
	GLSurfaceView glSurfaceView;

	@Bind(R.id.sticker_container)
	StickerContainer stickerContainer;

	@Bind(R.id.trash_box)
	ImageButton trashBox;

	@Bind(R.id.play_info)
	TextView playInfo;

	ImageReader imageReader;

	List<Texture> textureList = new ArrayList<>();
	int imageWidth = 0;
	int imageHeight = 0;
	int imageFormat = 0;
	int imageStride = 0;
	GestureDetector gestureDetector;
	DisplayMetrics displayMetrics;
	boolean trackingReady = false;
	Dialog progressDialog;
	ARManager arManager;

	int trackingMethod;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_track_image);

		ButterKnife.bind(this);

		trackingMethod = getSharedPreferences(VidChaserUtil.PREF_NAME, Activity.MODE_PRIVATE).
				getInt(VidChaserUtil.PREF_KEY_TRACKING_METHOD, VidChaserUtil.TRACKING_METHOD_RIGID);

		engineVersion.setText(String.format(Locale.US, "Tracker version : %s", VidChaserAPI.getEngineVersion()));

		imageReader = new ImageReader();
		if (!imageReader.setPath(VidChaserUtil.IMAGE_PATH)) {
			Toast.makeText(this, "You must save images first!", Toast.LENGTH_SHORT).show();
			finish();
		}

		// Read first image data to get image width, height, format
		byte[] imageFileBytes = imageReader.readFrame();

		imageWidth = VidChaserUtil.byteArrayToInt(imageFileBytes, 0);
		imageHeight = VidChaserUtil.byteArrayToInt(imageFileBytes, 4);
		imageFormat = VidChaserUtil.byteArrayToInt(imageFileBytes, 8);
		imageStride = VidChaserUtil.byteArrayToInt(imageFileBytes, 12);

		ProjectionMatrix.getInstance().setImageSize(imageWidth, imageHeight);

		imageReader.reset();

		imageSizeTextView.setText("Image Size : " + imageWidth + "x" + imageHeight + ", stride : " + imageStride);

		glSurfaceView.setEGLContextClientVersion(2);
		glSurfaceView.setOnTouchListener(this);
		glSurfaceView.setRenderer(new TrackImageRenderer(renderCallback));

		VidChaserAPI.create(getApplicationContext(), getResources().getString(R.string.app_pro_key));

		try {
			String[] textureFiles = getAssets().list("Sticker");
			for (String fileName : textureFiles) {
				textureList.add(Texture.loadTextureFromApk("Sticker/" + fileName, getAssets()));
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		stickerContainer.setStickerData(textureList);
		stickerContainer.setOnStickerSelected(stickerSelectedListener);

		gestureDetector = new GestureDetector(this, onGestureListener);

		displayMetrics = getResources().getDisplayMetrics();

		progressDialog = new Dialog(this, android.R.style.Theme_Translucent_NoTitleBar_Fullscreen);
		progressDialog.setContentView(R.layout.dialog_progress);
		progressDialog.setCancelable(false);

		arManager = new ARManager();
	}

	@Override
	protected void onResume() {
		super.onResume();
		glSurfaceView.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();

		glSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				VidChaserAPI.deinitRendering();
			}
		});
		glSurfaceView.onPause();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

		ButterKnife.unbind(this);
		VidChaserAPI.destroy();
	}

	@OnClick(R.id.show_sticker_list)
	public void onShowStickerClick() {
		stickerContainer.show();
	}

	final GestureDetector.OnGestureListener onGestureListener = new GestureDetector.OnGestureListener() {
		@Override
		public boolean onDown(MotionEvent e) {
			return false;
		}

		@Override
		public void onShowPress(MotionEvent e) {
		}

		@Override
		public boolean onSingleTapUp(MotionEvent e) {
			return false;
		}

		@Override
		public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
			return false;
		}

		public void onLongPress(MotionEvent e) {
			Log.e(TAG, "Long press detected");

			float[] glCoord = new float[2];
			VidChaserAPI.getGLCoordFromScreenCoord((int) e.getX(), (int) e.getY(), glCoord);
			selectedSticker = arManager.getTouchedSticker((int) glCoord[0], (int) glCoord[1]);
			if (selectedSticker != null) {
				trackingReady = true;
				trashBox.setVisibility(View.GONE);
			}
		}

		@Override
		public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
			return false;
		}
	};

	private Sticker selectedSticker = null;

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		switch (event.getAction()) {
			case MotionEvent.ACTION_DOWN:
				gestureDetector.onTouchEvent(event);

				float[] glCoord = new float[2];
				VidChaserAPI.getGLCoordFromScreenCoord((int) event.getX(), (int) event.getY(), glCoord);

				selectedSticker = arManager.getTouchedSticker((int) glCoord[0], (int) glCoord[1]);
				if (selectedSticker != null) {
					arManager.stopTracking(selectedSticker);
					trashBox.setVisibility(View.VISIBLE);
				}
				break;

			case MotionEvent.ACTION_MOVE:
				gestureDetector.onTouchEvent(event);

				if (selectedSticker != null) {
					glCoord = new float[2];
					VidChaserAPI.getGLCoordFromScreenCoord((int) event.getX(), (int) event.getY(), glCoord);
					selectedSticker.setTranslate(glCoord[0], glCoord[1], 0.0f);
				}
				break;

			case MotionEvent.ACTION_UP:
				gestureDetector.onTouchEvent(event);
				if (selectedSticker == null) {
					break;
				}

				trashBox.setVisibility(View.GONE);
				if (!trackingReady && event.getY() > displayMetrics.heightPixels - 200) {
					glSurfaceView.queueEvent(new Runnable() {
						@Override
						public void run() {
							arManager.removeSticker(selectedSticker);
							selectedSticker = null;
						}
					});
				}

				if (trackingReady) {
					imageReader.rewind();
					imageReader.updateIndex();

					progressDialog.show();

					int imageIndexWhenTouch = imageReader.getCurrentIndex();
					arManager.deativateAllTrackingPoint();
					arManager.startTracking(selectedSticker, imageIndexWhenTouch,
							imageReader.getLastIndex(), (int) event.getX(), (int) event.getY(), trackingMethod);
					trackingReady = false;
				} else {
					arManager.stopTracking(selectedSticker);
				}
		}
		return true;
	}

	private StickerContainer.OnStickerSelectedListener stickerSelectedListener = new StickerContainer.OnStickerSelectedListener() {
		@Override
		public void onStickerSelected(final Texture texture, final int size, MotionEvent event) {
			final int touchX = (int) event.getRawX();
			final int touchY = (int) event.getRawY();

			final float[] glCoords = new float[2];
			VidChaserAPI.getGLCoordFromScreenCoord(touchX, touchY, glCoords);

			float ratio = imageWidth / (float) glSurfaceView.getWidth();
			final float glStickerScale = size * ratio;

			glSurfaceView.queueEvent(new Runnable() {
				@Override
				public void run() {
					Sticker sticker = new Sticker();
					sticker.setProjectionMatrix(ProjectionMatrix.getInstance().getProjectionMatrix());
					sticker.setTexture(texture.bitmap);
					sticker.setScale(glStickerScale, glStickerScale, glStickerScale);
					sticker.setTranslate(glCoords[0], glCoords[1], 0.0f);
					arManager.addSticker(sticker);
				}
			});
		}
	};

	private TrackImageRenderer.RenderCallback renderCallback = new TrackImageRenderer.RenderCallback() {

		@Override
		public void onRender() {

			final int imageIndex = imageReader.getCurrentIndex();

			if (trackingReady) {
				VidChaserAPI.renderBackground();
				arManager.updateTransform(imageIndex);
				arManager.drawSticker(imageIndex);
				return;
			}

			if (imageReader.hasNext()) {
				byte[] fileBytes = imageReader.readFrame();
				VidChaserAPI.setNewFrame(fileBytes, 16, fileBytes.length - 16, imageWidth, imageHeight, imageStride, imageFormat, imageIndex);
			}

			VidChaserAPI.renderBackground();

			arManager.updateTransform(imageIndex);
			arManager.drawSticker(imageIndex);

			imageReader.updateIndex();

			if (!imageReader.hasNext()) {
				arManager.deativateAllTrackingPoint();
				imageReader.reset();
				runOnUiThread(new Runnable() {
					@Override
					public void run() {
						progressDialog.dismiss();
					}
				});
				return;
			}

			runOnUiThread(new Runnable() {
				@Override
				public void run() {
					playInfo.setText(String.format(Locale.UK, "Play image : %d / %d", imageIndex, imageReader.getLastIndex() + 1));
				}
			});
		}
	};
}
