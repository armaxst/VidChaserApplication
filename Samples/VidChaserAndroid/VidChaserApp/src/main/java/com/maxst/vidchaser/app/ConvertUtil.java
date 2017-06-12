/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;


import junit.framework.Assert;

public class ConvertUtil {

	public static int byteArrayToInt(byte[] bytes, int start) {
		Assert.assertTrue("Byte buffer length is not 4", (bytes.length - start) >= 4);
		return	(bytes[start + 3] & 0xFF) << 24|
				(bytes[start + 2] & 0xFF) << 16 |
				(bytes[start + 1] & 0xFF) << 8 |
				(bytes[start] & 0xFF);
	}
}
