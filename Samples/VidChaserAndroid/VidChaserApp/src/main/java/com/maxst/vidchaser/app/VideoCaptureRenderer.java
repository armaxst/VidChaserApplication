/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import com.maxst.videoplayer.VideoPlayer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class VideoCaptureRenderer implements GLSurfaceView.Renderer {

	private static final String TAG = VideoCaptureRenderer.class.getSimpleName();

	public interface CaptureCallback {
		void onVideoCaptured(int position);
	}

	private int surfaceWidth;
	private int surfaceHeight;

	private VideoRenderManager videoRenderManager;
	private VideoCaptureManager videoCaptureManager;
	private CaptureCallback captureCallback;
	private VideoPlayer videoPlayer;
	private boolean keepCapturing = false;

	VideoCaptureRenderer(VideoPlayer videoPlayer, CaptureCallback callback) {
		this.videoPlayer = videoPlayer;
		this.captureCallback = callback;
	}

	@Override
	public void onSurfaceCreated(GL10 unused, EGLConfig config) {
		videoRenderManager = new VideoRenderManager();
		videoRenderManager.setVideoPlayer(videoPlayer);

		videoCaptureManager = new VideoCaptureManager();
		videoCaptureManager.setVideoPlayer(videoPlayer);
	}

	@Override
	public void onSurfaceChanged(GL10 unused, int width, int height) {
		surfaceWidth = width;
		surfaceHeight = height;
	}

	@Override
	public void onDrawFrame(GL10 unused) {
		GLES20.glViewport(0, 0, surfaceWidth, surfaceHeight);
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

		videoPlayer.update();

		if (keepCapturing) {
			int position = videoCaptureManager.captureOneFrame();
			captureCallback.onVideoCaptured(position);
		}

		videoRenderManager.renderVideo();
	}

	void startCapturing() {
		videoCaptureManager.setVideoTexture(videoRenderManager.getVideoTexture());
		videoCaptureManager.readyToCapture();
		videoPlayer.setPosition(0);
		keepCapturing = true;
	}

	void stopCapturing() {
		keepCapturing = false;
		videoCaptureManager.stopCapturing();
	}

	public void onResume() {
	}

	public void onPause() {
		videoPlayer.destroy();
	}
}