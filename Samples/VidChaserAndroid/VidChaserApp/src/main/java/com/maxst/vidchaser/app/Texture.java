/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;

class Texture {

	private static final String TAG = Texture.class.getSimpleName();

	public int width;
	public int height;
	public Bitmap bitmap;

	static Texture loadTextureFromApk(String fileName, AssetManager assets) {
		InputStream inputStream = null;
		try {
			inputStream = assets.open(fileName, AssetManager.ACCESS_BUFFER);

			BufferedInputStream bufferedStream = new BufferedInputStream(
					inputStream);
			Bitmap bitMap = BitmapFactory.decodeStream(bufferedStream);

			Texture texture = new Texture();
			texture.width = bitMap.getWidth();
			texture.height = bitMap.getHeight();
			texture.bitmap = bitMap;
			return texture;
		} catch (IOException e) {
			Log.i(TAG, e.getMessage());
			return null;
		}
	}
}
