/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.opengl.GLES20;

import com.maxst.videoplayer.VideoPlayer;

public class VideoRenderManager {

	private VideoPlayer videoPlayer;
	private int[] textureNames;
	private boolean videoSizeAcquired = false;
	private VideoQuad videoQuad;

	VideoRenderManager() {
		videoQuad = new VideoQuad();
		videoQuad.setScale(2, 2, 1);
	}

	void setVideoPlayer(VideoPlayer videoPlayer) {
		this.videoPlayer = videoPlayer;
		textureNames = new int[1];
	}

	void renderVideo() {
		if (!videoSizeAcquired) {
			int videoWidth = videoPlayer.getVideoWidth();
			int videoHeight = videoPlayer.getVideoHeight();

			if (videoWidth == 0 || videoHeight == 0) {
				return;
			}

			videoSizeAcquired = true;

			GLES20.glGenTextures(1, textureNames, 0);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureNames[0]);
			GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
			GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
			GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
			GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
			GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGB, videoWidth, videoHeight, 0, GLES20.GL_RGB,
					GLES20.GL_UNSIGNED_SHORT_5_6_5, null);

			videoPlayer.setTexture(textureNames[0]);
			videoPlayer.start();
			return;
		}

		if (videoPlayer.getState() != VideoPlayer.STATE_PLAYING && videoPlayer.getState() != VideoPlayer.STATE_PAUSE) {
			return;
		}

		videoQuad.drawTexture(textureNames[0]);
	}

	int getVideoTexture() {
		return textureNames[0];
	}
}
