/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;

class VidChaserUtil {

	static final String TAG = VidChaserUtil.class.getSimpleName();

	static final String ROOT_PATH = Environment.getExternalStorageDirectory() + "/VidChaser";
	static final String IMAGE_PATH = ROOT_PATH + "/ImageSequences";

	// region - Preference
	static final String PREF_NAME = "VidChaser";
	static final String PREF_KEY_CAM_RESOLUTION = "cam_resolution";
	static final String PREF_KEY_IMAGE_FORMAT = "image_foramt";
	static final String PREF_KEY_TRACKING_METHOD = "tracking_method";

	static final int TRACKING_METHOD_AFFINE = 0;
	static final int TRACKING_METHOD_HOMOGRAPHY = 1;
	static final int TRACKING_METHOD_RIGID = 2;
	static final int TRACKING_METHOD_TRANSLATION = 3;
}
