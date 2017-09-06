/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;

import com.maxst.videoplayer.VideoPlayer;

import java.io.File;
import java.util.Locale;

import butterknife.Bind;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class VideoCaptureActivity extends Activity {

	public static final String TAG = VideoCaptureActivity.class.getSimpleName();

	@Bind(R.id.gl_surface_view)
	GLSurfaceView glSurfaceView;

	@Bind(R.id.capture_count)
	TextView captureCountView;

	@Bind(R.id.start_capture)
	Button startCapture;

	VideoCaptureRenderer videoCaptureRenderer;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_video_capture);

		ButterKnife.bind(this);

		glSurfaceView.setEGLContextClientVersion(2);

		File targetDir = new File(VidChaserUtil.IMAGE_PATH);
		if (!targetDir.exists()) {
			boolean result = targetDir.mkdirs();
			Log.d(TAG, "Make directory result : " + result);
		}

		VideoPlayer videoPlayer = new VideoPlayer(this);
		videoPlayer.openVideo(VidChaserUtil.TEST_VIDEO_FILE_NAME);
		videoPlayer.setOnCompletionListener(videoCompletionListener);

		videoCaptureRenderer = new VideoCaptureRenderer(videoPlayer, captureCallback);
		glSurfaceView.setRenderer(videoCaptureRenderer);
	}

	@Override
	protected void onResume() {
		super.onResume();
		glSurfaceView.onResume();
		glSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				videoCaptureRenderer.onResume();
			}
		});
	}

	@Override
	protected void onPause() {
		super.onPause();
		glSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				videoCaptureRenderer.onPause();
			}
		});
		glSurfaceView.onPause();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

		ButterKnife.unbind(this);
	}

	private int captureCount = 0;

	@OnClick(R.id.start_capture)
	public void startCapture() {
		startCapture.setEnabled(false);
		captureCount = 0;

		glSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				videoCaptureRenderer.startCapturing();
			}
		});
	}

	private VideoCaptureRenderer.CaptureCallback captureCallback = new VideoCaptureRenderer.CaptureCallback() {
		@Override
		public void onVideoCaptured(int position) {
			captureCount++;
			runOnUiThread(new Runnable() {
				@Override
				public void run() {
					captureCountView.setText(String.format(Locale.US, "Capture count : %04d", captureCount));
				}
			});
		}
	};

	private VideoPlayer.VideoCompletionListener videoCompletionListener = new VideoPlayer.VideoCompletionListener() {
		@Override
		public void onComplete() {
			if (!startCapture.isEnabled()) {
				startCapture.setEnabled(true);
				glSurfaceView.queueEvent(new Runnable() {
					@Override
					public void run() {
						videoCaptureRenderer.stopCapturing();
						finish();
					}
				});
			}
		}
	};
}
