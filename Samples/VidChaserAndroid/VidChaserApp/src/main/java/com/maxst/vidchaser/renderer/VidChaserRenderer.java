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

	public static native void initRendering();
	public static native void updateRendering(int width, int height);
	public static native void drawFrame();
	public static native void getImageCoordiFromScreenCoordi(int surfaceWidth, int surfaceHeight, int imageWidth, int imageHeight,
															 int surfaceX, int surfaceY, int [] imageCoordi);
	public static native void getGLCoordiFromScreenCoordi(int surfaceWidth, int surfaceHeight, int imageWidth, int imageHeight,
														  int surfaceX, int surfaceY, float [] glCoordi);
	public static native void addSticker(int textureWidth, int textureHeight, byte [] textureData, float glPositionX, float glPositionY, float scale);
}