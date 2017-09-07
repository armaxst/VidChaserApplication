/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.app.Activity;
import android.app.Dialog;
import android.opengl.GLSurfaceView;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.maxst.vidchaser.ProjectionMatrix;
import com.maxst.vidchaser.VidChaserAPI;
import com.maxst.videoplayer.VideoPlayer;

import java.io.IOException;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import butterknife.Bind;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class TrackVideoAsyncActivity extends AppCompatActivity implements View.OnTouchListener {

	private static final String TAG = TrackVideoAsyncActivity.class.getName();

	@Bind(R.id.image_size)
	TextView imageSizeTextView;

	@Bind(R.id.state)
	TextView stateTextView;

	@Bind(R.id.engine_version)
	TextView engineVersion;

	@Bind(R.id.gl_surface_view)
	GLSurfaceView glSurfaceView;

	@Bind(R.id.sticker_container)
	StickerContainer stickerContainer;

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
	Dialog progressDialog;
	ARManager arManager;
	TrackVideoRenderer trackVideoRenderer;
	VideoPlayer videoPlayer;
	private Sticker selectedSticker = null;
	private DebugQuad debugQuad = null;
	private byte [] debugImageBuffer = null;

	int trackingMethod;
	VidChaserUtil.TrackingState trackingState = VidChaserUtil.TrackingState.None;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_track_video);

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

		imageSizeTextView.setText("Captured Image Size : " + imageWidth + "x" + imageHeight + ", stride : " + imageStride);

		videoPlayer = new VideoPlayer(this);
		videoPlayer.openVideo(VidChaserUtil.TEST_VIDEO_FILE_NAME);
		videoPlayer.setOnCompletionListener(videoCompletionListener);

		trackVideoRenderer = new TrackVideoRenderer(videoPlayer, renderCallback);
		glSurfaceView.setEGLContextClientVersion(2);
		glSurfaceView.setOnTouchListener(this);
		glSurfaceView.setRenderer(trackVideoRenderer);

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
		debugQuad = new DebugQuad();
	}

	@Override
	protected void onResume() {
		super.onResume();
		glSurfaceView.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();
		glSurfaceView.onPause();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

		videoPlayer.destroy();
		ButterKnife.unbind(this);
		VidChaserAPI.destroy();
	}

	private boolean showDebug = false;

	@OnClick({R.id.show_sticker_list, R.id.debug_result})
	public void onClick(View view) {
		switch(view.getId()) {
			case R.id.show_sticker_list:
				stickerContainer.show();
				break;

			case R.id.debug_result:
				showDebug = !showDebug;
				break;
		}
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
			selectedSticker = arManager.getTouchedSticker((int)glCoord[0], (int)glCoord[1]);

			if (selectedSticker != null) {
				videoPlayer.pause();
				int index = imageReader.findNearestIndexByPosition(videoPlayer.getCurrentPosition());
				imageReader.setCurrentIndex(index);
				trackingState = VidChaserUtil.TrackingState.ReadyToTracking;
			}
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
				gestureDetector.onTouchEvent(event);

				float[] glCoord = new float[2];
				VidChaserAPI.getGLCoordFromScreenCoord((int) event.getX(), (int) event.getY(), glCoord);

				selectedSticker = arManager.getTouchedSticker((int)glCoord[0], (int)glCoord[1]);
				if (selectedSticker != null) {
					arManager.stopTracking(selectedSticker);
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

				if (trackingState == VidChaserUtil.TrackingState.ReadyToTracking) {
					imageReader.rewind();
					//progressDialog.show();
					int imageIndexWhenTouch = imageReader.getCurrentIndex();

					arManager.deativateAllTrackingPoint();
					arManager.startTracking(selectedSticker, imageIndexWhenTouch,
							imageReader.getLastIndex(), (int)event.getX(), (int)event.getY(), trackingMethod);

					trackingState = VidChaserUtil.TrackingState.Tracking;
					new TrackImageTask(this).execute();

					glSurfaceView.queueEvent(new Runnable() {
						@Override
						public void run() {
							debugImageBuffer = null;
							showDebug = true;
						}
					});
				} else {
					arManager.stopTracking(selectedSticker);
				}
		}
		return true;
	}

	void trackImageCompleted() {
		progressDialog.dismiss();
		glSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				trackingState = VidChaserUtil.TrackingState.PlayAfterTracking;
				videoPlayer.start();
				videoPlayer.setPosition(0);
			}
		});
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

	private int imageIndex = 0;

	private TrackVideoRenderer.RenderCallback renderCallback = new TrackVideoRenderer.RenderCallback() {

		@Override
		public void onRender(int position) {

			if (trackingState == VidChaserUtil.TrackingState.None || trackingState == VidChaserUtil.TrackingState.PlayAfterTracking) {
				imageIndex = imageReader.findNearestIndexByPosition(position);
				imageReader.setCurrentIndex(imageIndex);
			}

			if (imageIndex < 0) {
				return;
			}

			if (trackingState == VidChaserUtil.TrackingState.ReadyToTracking) {
				imageIndex = imageReader.findNearestIndexByPosition(position);
				imageReader.setCurrentIndex(imageIndex);
				byte[] fileBytes = imageReader.readFrame();
				VidChaserAPI.setNewFrame(fileBytes, 16, fileBytes.length - 16, imageWidth, imageHeight, imageStride, imageFormat, imageIndex);

				arManager.updateTransform(imageIndex);
				arManager.drawSticker(imageIndex);
				return;
			}

			if (trackingState == VidChaserUtil.TrackingState.Tracking) {
				arManager.drawSticker();
			}

			if (trackingState != VidChaserUtil.TrackingState.Tracking) {
				arManager.updateTransform(imageIndex);
				arManager.drawSticker(imageIndex);

				final int finalImageIndex = imageIndex;
				runOnUiThread(new Runnable() {
					@Override
					public void run() {
						playInfo.setText(String.format(Locale.UK, "Play image : %d / %d", finalImageIndex, imageReader.getLastIndex() + 1));
					}
				});
			}

			// region -- For debugging
			if (trackingState == VidChaserUtil.TrackingState.Tracking) {
				if (showDebug) {
					float[] resultTransform3x3 = new float[9];
					int[] processTime = new int[1];

					if (debugImageBuffer == null) {
						debugImageBuffer = new byte [1280 * 720];
						VidChaserAPI.getTrackingResultWithImage(resultTransform3x3, 0, processTime, debugImageBuffer);
						debugQuad.draw(debugImageBuffer, 1280, 720);
					} else {
						debugQuad.draw();
					}
				}
			}

			if (showDebug) {
				if (debugImageBuffer != null) {
					debugQuad.draw();
				}
			}
			// endregion -- For debugging
		}
	};

	private VideoPlayer.VideoCompletionListener videoCompletionListener = new VideoPlayer.VideoCompletionListener() {
		@Override
		public void onComplete() {
		}
	};

	private static class TrackImageTask extends AsyncTask<Void, Integer, Void> {

		private WeakReference<TrackVideoAsyncActivity> trackVideoActivityWeakReference;

		TrackImageTask(TrackVideoAsyncActivity activity) {
			trackVideoActivityWeakReference = new WeakReference<>(activity);
		}

		@Override
		protected Void doInBackground(Void... params) {
			TrackVideoAsyncActivity activity = trackVideoActivityWeakReference.get();
			ImageReader imageReader;
			if (activity != null) {
				while(true) {
					imageReader = activity.imageReader;
					int imageIndex = imageReader.getCurrentIndex();
					byte[] fileBytes = imageReader.readFrame();
					VidChaserAPI.setNewFrame(fileBytes, 16, fileBytes.length - 16,
							activity.imageWidth, activity.imageHeight, activity.imageStride, activity.imageFormat, imageIndex);

					activity.arManager.updateTransform(imageIndex);

					imageReader.updateIndex();
					if (!imageReader.hasNext()) {
						activity.arManager.deativateAllTrackingPoint();
						imageReader.reset();
					}

					if (imageIndex >= imageReader.getLastIndex()) {
						break;
					}

					publishProgress(imageIndex);
				}
			}
			return null;
		}

		@Override
		protected void onProgressUpdate(Integer... values) {
			TrackVideoAsyncActivity activity = trackVideoActivityWeakReference.get();
			if (activity != null) {
				activity.playInfo.setText(String.format(Locale.UK, "Play image : %d / %d", values[0], activity.imageReader.getLastIndex() + 1));
			}
		}

		@Override
		protected void onPostExecute(Void aVoid) {
			TrackVideoAsyncActivity activity = trackVideoActivityWeakReference.get();
			if (activity != null) {
				activity.trackImageCompleted();
			}
		}
	}
}
