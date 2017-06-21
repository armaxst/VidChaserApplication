/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.opengl.GLES20;

public class ShaderUtil {

	public static int loadShader(int type, String shaderSrc) {
		int shader;
		shader = GLES20.glCreateShader(type);
		GLES20.glShaderSource(shader, shaderSrc);
		GLES20.glCompileShader(shader);
		return shader;
	}
}