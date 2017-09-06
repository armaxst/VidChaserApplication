/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.util.Log;

import com.maxst.vidchaser.VideoCapturer;
import com.maxst.videoplayer.VideoPlayer;

import java.io.File;
import java.util.Locale;

public class VideoCaptureManager {

	private static final String TAG = VideoCaptureManager.class.getSimpleName();

	private VideoPlayer videoPlayer;

	private VideoQuad videoQuad;
	private int videoTexture;
	int prevPosition = 0;

	VideoCaptureManager() {
		videoQuad = new VideoQuad();
		videoQuad.setScale(2, -2, 1);
	}

	void readyToCapture() {
		File[] oldFiles = new File(VidChaserUtil.IMAGE_PATH).listFiles();
		if (oldFiles != null) {
			for (File file : oldFiles) {
				boolean result = file.delete();
				Log.d(TAG, "Delete file result : " + result);
			}
		}

		VideoCapturer.getInstance().init(videoPlayer.getVideoWidth() / 2, videoPlayer.getVideoHeight() / 2);
		prevPosition = 0;
	}

	public void setVideoPlayer(VideoPlayer videoPlayer) {
		this.videoPlayer = videoPlayer;
	}

	public void setVideoTexture(int textureId) {
		videoTexture = textureId;
	}

	public int captureOneFrame() {
		int position = videoPlayer.getCurrentPosition();
		if (prevPosition != position) {
			prevPosition = position;
			VideoCapturer.getInstance().beginCapture();
			videoQuad.drawTexture(videoTexture);
			String fileName = String.format(Locale.US, "%s/%05d.img", VidChaserUtil.IMAGE_PATH, prevPosition);
			VideoCapturer.getInstance().endCapture(true, fileName, 0);
		} else {
			Log.i(TAG, "Same position!!");
		}

		return prevPosition;
	}

	public void stopCapturing() {
		VideoCapturer.getInstance().stopCapturing();
	}
}
