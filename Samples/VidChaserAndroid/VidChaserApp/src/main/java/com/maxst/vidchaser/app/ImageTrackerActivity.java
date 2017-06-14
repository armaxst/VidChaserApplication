/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.app.Activity;
import android.app.Dialog;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.maxst.vidchaser.VidChaser;
import com.maxst.vidchaser.renderer.VidChaserRenderer;
import com.maxstar.MaxstAR;

import java.io.IOException;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import butterknife.Bind;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class ImageTrackerActivity extends AppCompatActivity implements View.OnTouchListener {

	private static final String TAG = ImageTrackerActivity.class.getName();

	private MessageHandler messageHandler;

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

	private ImageReader imageReader;

	private List<Texture> textureList = new ArrayList<>();
	private int imageWidth = 0;
	private int imageHeight = 0;
	private int imageFormat = 0;
	private GestureDetector gestureDetector;
	private DisplayMetrics displayMetrics;
	private boolean trackingReady = false;
	private long nativeStickerPointer = 0;
	private int imageIndexWhenTouch = 0;
	private Dialog progressDialog;

	int trackingMethod;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_image_tracker);

		ButterKnife.bind(this);

		trackingMethod = getSharedPreferences(VidChaserUtil.PREF_NAME , Activity.MODE_PRIVATE).
				getInt(VidChaserUtil.PREF_KEY_TRACKING_METHOD, VidChaserUtil.TRACKING_METHOD_RIGID);

		engineVersion.setText(String.format(Locale.US, "Tracker version : %s", VidChaser.getEngineVersion()));

		imageReader = new ImageReader();
		imageReader.setPath(VidChaserUtil.IMAGE_PATH);

		// Read first image data to get image width ,height, format
		byte[] imageFileBytes = imageReader.readFrame();

		imageWidth = ConvertUtil.byteArrayToInt(imageFileBytes, 0);
		imageHeight = ConvertUtil.byteArrayToInt(imageFileBytes, 4);
		imageFormat = ConvertUtil.byteArrayToInt(imageFileBytes, 8);

		imageReader.reset();

		imageSizeTextView.setText("Image Size : " + imageWidth + "x" + imageHeight);

		glSurfaceView.setEGLContextClientVersion(2);
		glSurfaceView.setOnTouchListener(this);
		glSurfaceView.setRenderer(new VidChaserRenderer());
		glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

		messageHandler = new MessageHandler(this);

		MaxstAR.init(this);
		MaxstAR.setSignature("icWQYj5ucBSSl2DXHNVSTdDalq+Doh1uEfCs+kgITS8=");
		VidChaser.create(this, "icWQYj5ucBSSl2DXHNVSTdDalq+Doh1uEfCs+kgITS8=");

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

		gestureDetector = new GestureDetector(this,onGestureListener);

		VidChaserRenderer.setImageSize(imageWidth, imageHeight);

		displayMetrics = getResources().getDisplayMetrics();

		progressDialog = new Dialog(this,android.R.style.Theme_Translucent_NoTitleBar_Fullscreen);
		progressDialog.setContentView(R.layout.dialog_progress);
		progressDialog.setCancelable(false);
	}

	@Override
	protected void onResume() {
		super.onResume();

		glSurfaceView.onResume();
		messageHandler.sendEmptyMessageDelayed(0, 33);
	}

	@Override
	protected void onPause() {
		super.onPause();

		messageHandler.removeCallbacksAndMessages(null);
		glSurfaceView.onPause();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

		ButterKnife.unbind(this);
		VidChaser.destroy();
		MaxstAR.deinit();
		VidChaserRenderer.deinitRendering();
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
			nativeStickerPointer = VidChaserRenderer.setTouchEvent((int)e.getX(), (int)e.getY(), 3);
			trackingReady = true;
			trashBox.setVisibility(View.GONE);
		}

		@Override
		public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
			return false;
		}
	};

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		switch (event.getAction()) {
			case MotionEvent.ACTION_DOWN:
				//messageHandler.removeCallbacksAndMessages(null);
				gestureDetector.onTouchEvent(event);
				nativeStickerPointer = VidChaserRenderer.setTouchEvent((int)event.getX(), (int)event.getY(), event.getAction());
				if (nativeStickerPointer != 0) {
					trashBox.setVisibility(View.VISIBLE);
				}
				break;

			case MotionEvent.ACTION_MOVE:
				gestureDetector.onTouchEvent(event);
				nativeStickerPointer = VidChaserRenderer.setTouchEvent((int)event.getX(), (int)event.getY(), event.getAction());
				break;

			case MotionEvent.ACTION_UP:
				gestureDetector.onTouchEvent(event);
				nativeStickerPointer = VidChaserRenderer.setTouchEvent((int)event.getX(), (int)event.getY(), event.getAction());
				trashBox.setVisibility(View.GONE);
				if (!trackingReady && event.getY() > displayMetrics.heightPixels - 200) {
					glSurfaceView.queueEvent(new Runnable() {
						@Override
						public void run() {
							VidChaserRenderer.removeSticker(nativeStickerPointer);
							nativeStickerPointer = 0;
						}
					});
				}

				if (trackingReady) {
					imageReader.rewind();
					progressDialog.show();
					imageIndexWhenTouch = imageReader.getCurrentIndex();
					VidChaserRenderer.startTracking(nativeStickerPointer, imageIndexWhenTouch, (int)event.getX(), (int)event.getY(), trackingMethod);
					trackingReady = false;
				} else {
					if (nativeStickerPointer != 0) {
						VidChaserRenderer.stopTracking(nativeStickerPointer);
					}
				}
		}
		return true;
	}

	private StickerContainer.OnStickerSelectedListener stickerSelectedListener = new StickerContainer.OnStickerSelectedListener() {
		@Override
		public void onStickerSelected(final Texture texture, final int size, MotionEvent event) {
			final int touchX = (int)event.getRawX();
			final int touchY = (int)event.getRawY();

			float ratio = imageWidth / (float)glSurfaceView.getWidth();
			final float glStickerScale = size * ratio;

			glSurfaceView.queueEvent(new Runnable() {
				@Override
				public void run() {
					VidChaserRenderer.addSticker(texture.width, texture.height, texture.data, touchX, touchY, glStickerScale);
				}
			});
		}
	};

	private static class MessageHandler extends Handler {
		private WeakReference<ImageTrackerActivity> trackerActivityWeakReference;

		MessageHandler(ImageTrackerActivity trackerActivity) {
			trackerActivityWeakReference = new WeakReference<>(trackerActivity);
		}

		@Override
		public void handleMessage(Message msg) {
			ImageTrackerActivity trackerActivity = trackerActivityWeakReference.get();
			if (trackerActivity != null) {
				ImageReader imageReader = trackerActivity.imageReader;

				switch (msg.what) {
					case 0:
						if (trackerActivity.trackingReady) {
							trackerActivity.glSurfaceView.requestRender();
							sendEmptyMessageDelayed(0, 10);
							return;
						}

						if (imageReader.hasNext()) {
							//Log.i(TAG, "imageIndexWhenTouch : " + trackerActivity.imageIndexWhenTouch);
							//Log.i(TAG, "getCurrentIndex : " + imageReader.getCurrentIndex());
							if (trackerActivity.imageIndexWhenTouch < imageReader.getCurrentIndex()) {
								trackerActivity.progressDialog.dismiss();
							}

							int imageIndex = imageReader.getCurrentIndex();
							if (imageIndex == 0) {
								Log.i(TAG, "Image index is 0");
							}

							byte[] fileBytes = imageReader.readFrame();

							VidChaserRenderer.setImageIndex(imageIndex);
							VidChaser.setTrackingImage(fileBytes, 12, trackerActivity.imageWidth, trackerActivity.imageHeight,
									trackerActivity.imageFormat, imageIndex);
							MaxstAR.setNewCameraFrame(fileBytes, 12, fileBytes.length - 12, trackerActivity.imageWidth, trackerActivity.imageHeight,
									trackerActivity.imageFormat);
						} else {
							Log.i(TAG, "Image index arrived last");
							Log.i(TAG, "Image index is : " + imageReader.getCurrentIndex());
							imageReader.reset();
						}

						sendEmptyMessageDelayed(0, 10);
						trackerActivity.glSurfaceView.requestRender();
						break;
				}
			}
		}
	}
}
