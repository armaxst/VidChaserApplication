/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;

import com.maxst.vidchaser.VidChaser;
import com.maxst.vidchaser.renderer.VidChaserRenderer;
import com.maxstar.MaxstAR;
import com.maxstar.camera.OnNewCameraFrameCallback;

import java.io.File;
import java.util.Locale;
import java.util.concurrent.locks.ReentrantLock;

import butterknife.Bind;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class ImageSaveActivity extends Activity {

	public static final String TAG = ImageSaveActivity.class.getSimpleName();

	@Bind(R.id.gl_surface_view)
	GLSurfaceView glSurfaceView;

	@Bind(R.id.save_image)
	Button saveImage;

	@Bind(R.id.cam_resolution)
	TextView camResolutionView;

	@Bind(R.id.image_save_format)
	TextView imageSaveFormatView;

	@Bind(R.id.image_saved_count)
	TextView imageSavedCountView;

	int preferResolution;
	int imageSaveFormat;

	boolean actualCamResolutionAcquired = false;

	HandlerThread handlerThread;
	Handler imageSaveHandler;

	ReentrantLock imageCopyLock = new ReentrantLock();
	int imageWidth = 0;
	int imageHeight = 0;
	int imageBufferLength = 0;
	byte [] sharedImageBuffer = null;
	byte [] imageBufferForSave = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_image_save);

		ButterKnife.bind(this);

		glSurfaceView.setEGLContextClientVersion(2);

		File targetDir = new File(VidChaserUtil.IMAGE_PATH);
		if (!targetDir.exists()) {
			boolean result = targetDir.mkdirs();
			Log.d(TAG, "Make directory result : " + result);
		}

		preferResolution = getSharedPreferences(VidChaserUtil.PREF_NAME , Activity.MODE_PRIVATE).getInt(VidChaserUtil.PREF_KEY_CAM_RESOLUTION, 0);
		imageSaveFormat = getSharedPreferences(VidChaserUtil.PREF_NAME , Activity.MODE_PRIVATE).getInt(VidChaserUtil.PREF_KEY_IMAGE_FORMAT, 0);

		switch (imageSaveFormat) {
			case 0:
				imageSaveFormatView.setText("Save format : RGBA8888");
				break;

			case 1:
				imageSaveFormatView.setText("Save format : RGB888");
				break;

			case 2:
				imageSaveFormatView.setText("Save format : YUV");
				break;
		}

		glSurfaceView.setRenderer(new VidChaserRenderer());

		MaxstAR.init(this);
		MaxstAR.setSignature("icWQYj5ucBSSl2DXHNVSTdDalq+Doh1uEfCs+kgITS8=");
		MaxstAR.setOnNewCameraFrameCallback(onNewCameraFrameCallback);

		handlerThread = new HandlerThread("ImageSaveHandlerThread");
		handlerThread.start();
		imageSaveHandler = new Handler(handlerThread.getLooper());
	}

	@Override
	protected void onResume() {
		super.onResume();

		switch (preferResolution) {
			case 0:
				MaxstAR.startCamera(0, 640, 480);
				break;

			case 1:
				MaxstAR.startCamera(0, 1280, 720);
				break;

			case 2:
				MaxstAR.startCamera(0, 1920, 1080);
				break;
		}

		glSurfaceView.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();

		glSurfaceView.onPause();
		MaxstAR.stopCamera();

		imageSaveHandler.removeCallbacksAndMessages(null);
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

		ButterKnife.unbind(this);
		handlerThread.quit();
		MaxstAR.deinit();
	}

	private static final int MAX_FRAME_COUNT = 300;
	private boolean keepWriting = false;
	private int frameCount = 0;

	@OnClick(R.id.save_image)
	public void onImageSaveClick() {
		File [] oldFiles = new File(VidChaserUtil.IMAGE_PATH).listFiles();
		if (oldFiles != null) {
			for (File file : oldFiles) {
				boolean result = file.delete();
				Log.d(TAG, "Delete file result : " + result);
			}
		}

		sharedImageBuffer = null;
		imageBufferForSave = null;
		saveImage.setEnabled(false);
		frameCount = 0;
		keepWriting = true;
	}

	OnNewCameraFrameCallback onNewCameraFrameCallback = new OnNewCameraFrameCallback() {
		@Override
		public void onNewCameraFrame(byte[] data, int length, int width, int height, int colorFormat) {

			if (!actualCamResolutionAcquired) {
				actualCamResolutionAcquired = true;
				camResolutionView.setText(String.format(Locale.US, "Camera resolution : %d x %d", width, height));
			}

			if (keepWriting) {
				if (imageWidth != width || imageHeight != height || imageBufferLength != length || sharedImageBuffer == null) {
					sharedImageBuffer = data.clone();
					imageWidth = width;
					imageHeight = height;
					imageBufferLength = length;
				} else {
					imageCopyLock.lock();
					System.arraycopy(data, 0, sharedImageBuffer, 0, length);
					imageCopyLock.unlock();
				}

				if (frameCount < MAX_FRAME_COUNT) {

					imageSaveHandler.post(new Runnable() {
						@Override
						public void run() {
							if (frameCount < MAX_FRAME_COUNT) {
								final String fileName = String.format(Locale.US, "%s/%dx%d_%04d.img", VidChaserUtil.IMAGE_PATH, imageWidth, imageHeight,
										frameCount);

								if (imageBufferForSave == null) {
									imageBufferForSave = sharedImageBuffer.clone();
								} else {
									imageCopyLock.lock();
									System.arraycopy(sharedImageBuffer, 0, imageBufferForSave, 0, imageBufferLength);
									imageCopyLock.unlock();
								}
								VidChaser.saveCameraFrame(fileName, imageBufferForSave, imageBufferLength, imageWidth, imageHeight, imageSaveFormat);
								runOnUiThread(new Runnable() {
									@Override
									public void run() {
										imageSavedCountView.setText(String.format(Locale.US, "Image Saved : %d / %d", frameCount, MAX_FRAME_COUNT));
									}
								});
								frameCount++;
							}
						}
					});
				} else {
					saveImage.setEnabled(true);
					keepWriting = false;
				}
			}
		}
	};
}
