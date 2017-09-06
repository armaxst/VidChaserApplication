/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.os.Environment;

import junit.framework.Assert;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

class VidChaserUtil {

	static final String TAG = VidChaserUtil.class.getSimpleName();

	static final String ROOT_PATH = Environment.getExternalStorageDirectory() + "/VidChaser";
	static final String IMAGE_PATH = ROOT_PATH + "/ImageSequences";
	static final String TEST_VIDEO_FILE_NAME = "20170905_111519.mp4";

	// region - Preference
	static final String PREF_NAME = "VidChaser";
	static final String PREF_KEY_CAPTURE_SCALE = "capture_scale";
	static final String PREF_KEY_TRACKING_METHOD = "tracking_method";

	static final int TRACKING_METHOD_AFFINE = 0;
	static final int TRACKING_METHOD_HOMOGRAPHY = 1;
	static final int TRACKING_METHOD_RIGID = 2;
	static final int TRACKING_METHOD_TRANSLATION = 3;

	public static int byteArrayToInt(byte[] bytes, int start) {
		Assert.assertTrue("Byte buffer length is not 4", (bytes.length - start) >= 4);
		return	(bytes[start + 3] & 0xFF) << 24|
				(bytes[start + 2] & 0xFF) << 16 |
				(bytes[start + 1] & 0xFF) << 8 |
				(bytes[start] & 0xFF);
	}

	static byte[] readImageBytesFromFile(String srcFile) {
		File file = new File(srcFile);
		int size = (int) file.length();
		byte[] bytes = new byte[size];
		try {
			BufferedInputStream buf = new BufferedInputStream(new FileInputStream(file));
			buf.read(bytes, 0, bytes.length);
			buf.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return bytes;
	}
}
