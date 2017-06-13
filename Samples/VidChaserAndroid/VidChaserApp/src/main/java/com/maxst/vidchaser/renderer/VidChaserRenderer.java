/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.renderer;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class VidChaserRenderer implements GLSurfaceView.Renderer {

	private final static String TAG = VidChaserRenderer.class.getSimpleName();

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		initRendering();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		updateRendering(width, height);
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		drawFrame();
	}

	// region -- GL Thread method
	public static native void initRendering();
	public static native void updateRendering(int width, int height);
	public static native void drawFrame();
	public static native void addSticker(int textureWidth, int textureHeight, byte [] textureData, int touchX, int touchY, float scale);
	public static native void removeSticker(long nativeStickerPointer);
	// endregion -- GL Thread method

	public static native void deinitRendering();

	public static native void setImageSize(int width, int height);
	public static native void setImageIndex(int index);
	public static native long setTouchEvent(int touchAction, int touchX, int touchY);

	public static native void startTracking(long nativeStickerId, int imageIndex, int touchX, int touchY);
	public static native void stopTracking();
}