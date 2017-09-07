/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import butterknife.ButterKnife;
import butterknife.OnClick;

public class MainActivity extends Activity implements View.OnClickListener {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		ButterKnife.bind(this);
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

		ButterKnife.unbind(this);
	}

	@OnClick({R.id.capture_video, R.id.play_captured, R.id.track_image, R.id.track_video, R.id.track_video_async, R.id.settings})
	public void onClick(View view) {
		switch (view.getId()) {
			case R.id.capture_video: {
				Intent intent = new Intent(MainActivity.this, VideoCaptureActivity.class);
				startActivity(intent);
			}
			break;

			case R.id.play_captured: {
				Intent intent = new Intent(MainActivity.this, PlayCapturedActivity.class);
				startActivity(intent);
			}
			break;

			case R.id.track_image: {
				Intent intent = new Intent(MainActivity.this, TrackImageActivity.class);
				startActivity(intent);
			}
			break;

			case R.id.track_video: {
				Intent intent = new Intent(MainActivity.this, TrackVideoActivity.class);
				startActivity(intent);
			}
			break;

			case R.id.track_video_async: {
				Intent intent = new Intent(MainActivity.this, TrackVideoAsyncActivity.class);
				startActivity(intent);
			}
			break;

			case R.id.settings: {
				Intent intent = new Intent(MainActivity.this, SettingsActivity.class);
				startActivity(intent);
			}
			break;
		}
	}
}
