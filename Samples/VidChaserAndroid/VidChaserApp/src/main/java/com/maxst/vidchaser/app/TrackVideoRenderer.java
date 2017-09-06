/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import com.maxst.vidchaser.ProjectionMatrix;
import com.maxst.vidchaser.VidChaserAPI;
import com.maxst.videoplayer.VideoPlayer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class TrackVideoRenderer implements GLSurfaceView.Renderer {

	private static final String TAG = TrackVideoRenderer.class.getSimpleName();

	public interface RenderCallback {
		void onRender(int position);
	}

	private int surfaceWidth;
	private int surfaceHeight;

	private VideoRenderManager videoRenderManager;
	private VideoCaptureManager imageSaveManager;
	private RenderCallback renderCallback;
	private VideoPlayer videoPlayer;

	TrackVideoRenderer(VideoPlayer videoPlayer, RenderCallback callback) {
		this.videoPlayer = videoPlayer;
		this.renderCallback = callback;
	}

	@Override
	public void onSurfaceCreated(GL10 unused, EGLConfig config) {
		videoRenderManager = new VideoRenderManager();
		videoRenderManager.setVideoPlayer(videoPlayer);

		VidChaserAPI.initRedering();
	}

	@Override
	public void onSurfaceChanged(GL10 unused, int width, int height) {
		VidChaserAPI.updateRendering(width, height);
		ProjectionMatrix.getInstance().setSurfaceSize(width, height);
	}

	@Override
	public void onDrawFrame(GL10 unused) {
		GLES20.glViewport(0, 0, ProjectionMatrix.getInstance().getSurfaceWidth(), ProjectionMatrix.getInstance().getSurfaceHeight());
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

		videoPlayer.update();
		videoRenderManager.renderVideo();

		if (renderCallback != null) {
			renderCallback.onRender(videoPlayer.getCurrentPosition());
		}
	}
}