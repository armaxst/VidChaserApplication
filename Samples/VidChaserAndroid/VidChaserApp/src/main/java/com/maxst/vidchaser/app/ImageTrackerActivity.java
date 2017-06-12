/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
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

public class ImageTrackerActivity extends AppCompatActivity implements View.OnTouchListener {

	private static final String TAG = ImageTrackerActivity.class.getName();

	private MessageHandler messageHandler;
	private boolean needProcessing = false;
	private boolean skipProcessing = false;

	@Bind(R.id.image_size)
	TextView imageSizeTextView;

	@Bind(R.id.engine_version)
	TextView engineVersion;

	@Bind(R.id.gl_surface_view)
	GLSurfaceView glSurfaceView;

	private ImageReader imageReader;

	private ProgressBar processingProgressUI;
	private List<Texture> textureList = new ArrayList<>();
	private float pixelDensity = 1.0f;
	private int imageWidth = 0;
	private int imageHeight = 0;
	private int imageFormat = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_image_tracker);

		ButterKnife.bind(this);

		processingProgressUI = (ProgressBar) findViewById(R.id.progress_bar);

		engineVersion.setText(String.format(Locale.US, "Tracker version : %s", VidChaser.getEngineVersion()));

		imageReader = new ImageReader();
		imageReader.setPath(VidChaserUtil.IMAGE_PATH);

		// Read first image data to get image width ,height, format
		byte [] imageFileBytes = imageReader.readFrame();

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
			String [] textureFiles = getAssets().list("Sticker");
			for (String fileName : textureFiles) {
				textureList.add(Texture.loadTextureFromApk("Sticker/" + fileName, getAssets()));
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		pixelDensity = getResources().getDisplayMetrics().density;
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
	}

	int stickerIndex = 0;

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		switch (event.getAction()) {
			case MotionEvent.ACTION_DOWN:
				messageHandler.removeCallbacksAndMessages(null);
				break;

			case MotionEvent.ACTION_MOVE:
				break;

			case MotionEvent.ACTION_UP:
				int touchX = (int) event.getX();
				int touchY = (int) event.getY();
				Log.d(TAG, "onTouch (" + touchX + ", " + touchY + ")");

				int [] imageCoordi = new int[2];
				final float [] glCoordi = new float[2];

				VidChaserRenderer.getImageCoordiFromScreenCoordi(glSurfaceView.getWidth(), glSurfaceView.getHeight(),
						imageWidth, imageHeight, touchX, touchY, imageCoordi);
				VidChaserRenderer.getGLCoordiFromScreenCoordi(glSurfaceView.getWidth(), glSurfaceView.getHeight(),
						imageWidth, imageHeight, touchX, touchY, glCoordi);

				glSurfaceView.queueEvent(new Runnable() {
					@Override
					public void run() {
						// TODO : setTracking postion here

						if (stickerIndex >= textureList.size()) {
							stickerIndex = 0;
						}

						Texture texture = textureList.get(stickerIndex);
						VidChaserRenderer.addSticker(texture.width, texture.height, texture.data, glCoordi[0], glCoordi[1], 50 * pixelDensity);

						stickerIndex++;
					}
				});

				//vidChaser core
				// TODO : Gidools
//				VidChaserRenderer.convertCoordinateDisplayToGL(glRenderView.getWidth(), glRenderView.getHeight(), imageWidth, imageHeight, touchX, touchY, trackerGLX, trackerGLY);
//				VidChaserRenderer.convertCoordinateGLToImage(imageWidth, imageHeight, trackerGLX[0], trackerGLY[0], trackerImageX, trackerImageY);
//				Log.d(TAG, "convert gl xy (" + trackerGLX[0] + ", " + trackerGLY[0] + ")");
//				Log.d(TAG, "convert image xy (" + trackerImageX[0] + ", " + trackerImageY[0] + ")");
//				VidChaser.setTrackingPosition(trackerImageX[0], trackerImageY[0], stickerWidth, stickerHeight);
//
//				VidChaserRenderer.calcGLVPMatrix(glRenderView.getWidth(), glRenderView.getHeight(), imageWidth, imageHeight, resultVPMatrix4x4);
//				VidChaserRenderer.calcInitGLModelMatrix(trackerGLX[0], trackerGLY[0], stickerWidth, stickerHeight, resultModelMatrix4x4);
//
//				//render
//				glRenderView.setSticker((int) trackerGLX[0], (int) trackerGLY[0]);
//
//				if (!needProcessing) {
//					needProcessing = true;
//					skipProcessing = false;
//					enableProcessingUI(true);
//					imageIndexWhenTouch = imageReader.getCurrentIndex();
//					imageReader.rewind();
//				}
				messageHandler.sendEmptyMessage(0);

		}
		return true;
	}

	private void enableProcessingUI(boolean visibility) {
		if (visibility) {
			processingProgressUI.setVisibility(View.VISIBLE);
			glSurfaceView.setOnTouchListener(null);
		} else {
			processingProgressUI.setVisibility(View.GONE);
			glSurfaceView.setOnTouchListener(this);
		}
	}

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
						if (imageReader.isLast()) {
							if (trackerActivity.needProcessing) {
								trackerActivity.needProcessing = false;
								trackerActivity.skipProcessing = true;
								trackerActivity.enableProcessingUI(false);
							}
						}

						if (imageReader.hasNext()) {
							byte[] fileBytes = imageReader.readFrame();
							long timestamp = imageReader.getTimestamp();

							MaxstAR.setNewCameraFrame(fileBytes, 12, fileBytes.length - 12, trackerActivity.imageWidth, trackerActivity.imageHeight, trackerActivity.imageFormat);

							//initialize
							// TODO : Gidools
//							if (trackerActivity.imageWidth != tempImageWidth || trackerActivity.imageHeight != tempImageHeight) {
//								trackerActivity.imageSizeTextView.setText("Frame Size : " + tempImageWidth + "x" + tempImageHeight);
//								trackerActivity.imageWidth = tempImageWidth;
//								trackerActivity.imageHeight = tempImageHeight;
//
//								//vidChaser core
//								VidChaser.initTracker(trackerActivity.imageWidth, trackerActivity.imageHeight);
//								Log.d(TAG, "@@@@ initTracker width :" + tempImageWidth + ", height : " + tempImageHeight);
//								Log.d(TAG, "VidChaser.initTracker");
//							}
//
//							if (trackerActivity.needProcessing && trackerActivity.skipProcessing) {
//								if (trackerActivity.imageIndexWhenTouch == trackerActivity.imageReader.getCurrentIndex()) {
//									// Need to init tracking position when image index same.
//									VidChaser.setTrackingPosition(trackerActivity.trackerImageX[0], trackerActivity.trackerImageY[0], trackerActivity.stickerWidth, trackerActivity.stickerHeight);
//								}
//
//								if (trackerActivity.imageIndexWhenTouch < trackerActivity.imageReader.getCurrentIndex()) {
//									// Need to process from touch index to last
//									trackerActivity.skipProcessing = false;
//								}
//							}
//
//							if (trackerActivity.needProcessing && trackerActivity.skipProcessing) {
//								float[] resultMVPMatrix = new float[16];
//								float[] transformMatrix = new float[9];
//								Log.d(TAG, "@@@@ trackFrame width :" + tempImageWidth + ", height : " + tempImageHeight);
//								VidChaser.trackFrame(tempImageWidth, tempImageHeight, pixelFormat, fileBytes, false, 12, transformMatrix);
//								Log.d(TAG, "transform matrix 1: "
//										+ transformMatrix[0] + ", "+ transformMatrix[3] + ", "+ transformMatrix[6] + ", " +
//										+ transformMatrix[1] + ", "+ transformMatrix[4] + ", "+ transformMatrix[7] + ", " +
//										+ transformMatrix[2] + ", "+ transformMatrix[5] + ", "+ transformMatrix[8]);
//
//								VidChaserRenderer.calcGLMVPMatrix(trackerActivity.imageWidth, trackerActivity.imageHeight,
//										trackerActivity.resultVPMatrix4x4, trackerActivity.resultModelMatrix4x4, transformMatrix, resultMVPMatrix);
//								Log.d(TAG, "transform matrix 2: "
//										+ transformMatrix[0] + ", "+ transformMatrix[3] + ", "+ transformMatrix[6] + ", " +
//										+ transformMatrix[1] + ", "+ transformMatrix[4] + ", "+ transformMatrix[7] + ", " +
//										+ transformMatrix[2] + ", "+ transformMatrix[5] + ", "+ transformMatrix[8]);
//
//								Log.d(TAG, "mvp matrix : "
//										+ resultMVPMatrix[0] + ", "+ resultMVPMatrix[4] + ", "+ resultMVPMatrix[8] + ", "+ resultMVPMatrix[12] + ", "
//										+ resultMVPMatrix[1] + ", "+ resultMVPMatrix[5] + ", "+ resultMVPMatrix[9] + ", "+ resultMVPMatrix[13] + ", "
//										+ resultMVPMatrix[2] + ", "+ resultMVPMatrix[6] + ", "+ resultMVPMatrix[10] + ", "+ resultMVPMatrix[14] + ", "
//										+ resultMVPMatrix[3] + ", "+ resultMVPMatrix[7] + ", "+ resultMVPMatrix[11] + ", "+ resultMVPMatrix[15] + ", ");
//								trackerActivity.glRenderView.pushStickerTrackingResult(timestamp, resultMVPMatrix);
//							}

							if (trackerActivity.needProcessing) {
								sendEmptyMessage(0);
							} else {
								sendEmptyMessageDelayed(0, 33);
							}

							// TODO : Gidools
							//trackerActivity.glRenderView.setNewFrame(fileBytes, 12, tempImageWidth, tempImageHeight, pixelFormat, timestamp);
						} else {
							imageReader.reset();
							trackerActivity.skipProcessing = true;
							sendEmptyMessageDelayed(0, 33);
						}

						trackerActivity.glSurfaceView.requestRender();
						break;
				}
			}
		}
	}
}
