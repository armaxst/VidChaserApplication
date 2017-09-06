/*
 * Copyright 2017 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.RadioButton;

import butterknife.Bind;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class SettingsActivity extends Activity {

//	@Bind(R.id.divide_2)
//	RadioButton divide2;
//
//	@Bind(R.id.divide_4)
//	RadioButton divide4;

	@Bind(R.id.tracking_affine)
	RadioButton trackingAffine;

	@Bind(R.id.tracking_homography)
	RadioButton trackingHomography;

	@Bind(R.id.tracking_rigid)
	RadioButton trackingRigid;

	@Bind(R.id.tracking_translation)
	RadioButton trackingTranslation;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_settings);

		ButterKnife.bind(this);

//		int captureScale = getSharedPreferences(VidChaserUtil.PREF_NAME , Activity.MODE_PRIVATE).getInt(VidChaserUtil.PREF_KEY_CAPTURE_SCALE, 0);
//		switch (captureScale) {
//			case 0:
//				divide2.setChecked(true);
//				break;
//
//			case 1:
//				divide4.setChecked(true);
//				break;
//		}

		int trackingMethod = getSharedPreferences(VidChaserUtil.PREF_NAME , Activity.MODE_PRIVATE).
				getInt(VidChaserUtil.PREF_KEY_TRACKING_METHOD, VidChaserUtil.TRACKING_METHOD_RIGID);
		switch (trackingMethod) {
			case VidChaserUtil.TRACKING_METHOD_AFFINE:
				trackingAffine.setChecked(true);
				break;

			case VidChaserUtil.TRACKING_METHOD_HOMOGRAPHY:
				trackingHomography.setChecked(true);
				break;

			case VidChaserUtil.TRACKING_METHOD_RIGID:
				trackingRigid.setChecked(true);
				break;

			case VidChaserUtil.TRACKING_METHOD_TRANSLATION:
				trackingTranslation.setChecked(true);
				break;
		}
	}

	@Override
	public void onDestroy() {
		super.onDestroy();

		ButterKnife.unbind(this);
	}

//	@OnClick({R.id.divide_2, R.id.divide_4})
//	public void onScaleClick(View view) {
//		switch (view.getId()) {
//			case R.id.divide_2:
//				SharedPreferences.Editor editor = getSharedPreferences(VidChaserUtil.PREF_NAME , Activity.MODE_PRIVATE).edit();
//				editor.putInt(VidChaserUtil.PREF_KEY_CAPTURE_SCALE, 0);
//				editor.apply();
//				break;
//
//			case R.id.divide_4:
//				editor = getSharedPreferences(VidChaserUtil.PREF_NAME , Activity.MODE_PRIVATE).edit();
//				editor.putInt(VidChaserUtil.PREF_KEY_CAPTURE_SCALE, 1);
//				editor.apply();
//				break;
//		}
//	}

	@OnClick({R.id.tracking_affine, R.id.tracking_homography, R.id.tracking_rigid, R.id.tracking_translation})
	public void onTrackingMethodClick(View view) {
		switch (view.getId()) {
			case R.id.tracking_affine:
				SharedPreferences.Editor editor = getSharedPreferences(VidChaserUtil.PREF_NAME , Activity.MODE_PRIVATE).edit();
				editor.putInt(VidChaserUtil.PREF_KEY_TRACKING_METHOD, VidChaserUtil.TRACKING_METHOD_AFFINE);
				editor.apply();
				break;

			case R.id.tracking_homography:
				editor = getSharedPreferences(VidChaserUtil.PREF_NAME , Activity.MODE_PRIVATE).edit();
				editor.putInt(VidChaserUtil.PREF_KEY_TRACKING_METHOD, 1);
				editor.apply();
				break;

			case R.id.tracking_rigid:
				editor = getSharedPreferences(VidChaserUtil.PREF_NAME , Activity.MODE_PRIVATE).edit();
				editor.putInt(VidChaserUtil.PREF_KEY_TRACKING_METHOD, 2);
				editor.apply();
				break;

			case R.id.tracking_translation:
				editor = getSharedPreferences(VidChaserUtil.PREF_NAME , Activity.MODE_PRIVATE).edit();
				editor.putInt(VidChaserUtil.PREF_KEY_TRACKING_METHOD, 2);
				editor.apply();
				break;
		}
	}
}
