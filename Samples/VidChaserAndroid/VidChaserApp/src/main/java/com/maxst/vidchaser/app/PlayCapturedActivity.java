/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;
import android.widget.Toast;

import com.maxst.vidchaser.ProjectionMatrix;
import com.maxst.vidchaser.VidChaserAPI;

import java.util.Locale;

import butterknife.Bind;
import butterknife.ButterKnife;

public class PlayCapturedActivity extends AppCompatActivity {

	private static final String TAG = PlayCapturedActivity.class.getName();

	@Bind(R.id.captured_info)
	TextView capturedInfoView;

	@Bind(R.id.status)
	TextView status;

	@Bind(R.id.gl_surface_view)
	GLSurfaceView glSurfaceView;

	ImageReader imageReader;
	int imageWidth;
	int imageHeight;
	int imageFormat;
	int imageStride;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_play_captured);

		ButterKnife.bind(this);

		imageReader = new ImageReader();
		if (!imageReader.setPath(VidChaserUtil.IMAGE_PATH)) {
			Toast.makeText(this, "You must capture video first!", Toast.LENGTH_SHORT).show();
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

		capturedInfoView.setText("Captured Size : " + imageWidth + "x" + imageHeight + ", stride : " + imageStride);

		glSurfaceView.setEGLContextClientVersion(2);
		glSurfaceView.setRenderer(new PlayCapturedRenderer(renderCallback));

		VidChaserAPI.create(getApplicationContext(), getResources().getString(R.string.app_pro_key));
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

	private PlayCapturedRenderer.RenderCallback renderCallback = new PlayCapturedRenderer.RenderCallback() {

		@Override
		public void onRender() {
			if (imageReader.hasNext()) {
				byte[] fileBytes = imageReader.readFrame();
				final int imageIndex = imageReader.getCurrentIndex();
				VidChaserAPI.setNewFrame(fileBytes, 16, fileBytes.length - 16, imageWidth, imageHeight, imageStride, imageFormat, imageIndex);
				imageReader.updateIndex();

				runOnUiThread(new Runnable() {
					@Override
					public void run() {
						status.setText(String.format(Locale.US, "Index : %d / %d", imageIndex, imageReader.getLastIndex()));
					}
				});
			} else {
				imageReader.reset();
			}
		}
	};
}
