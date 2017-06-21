/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.graphics.Bitmap;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.opengl.Matrix;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class Sticker {

	private static final int QUAD_IDX_BUFF_LENGTH = 6;

	private static final String VERTEX_SHADER_SRC =
			"attribute vec4 vertexPosition;\n" +
					"attribute vec2 vertexTexCoord;\n" +
					"varying vec2 texCoord;\n" +
					"uniform mat4 modelViewProjectionMatrix;\n" +
					"void main()							\n" +
					"{										\n" +
					"	gl_Position = modelViewProjectionMatrix * vertexPosition;\n" +
					"	texCoord = vertexTexCoord; 			\n" +
					"}										\n";

	private static final String FRAGMENT_SHADER_SRC =
			"precision mediump float;\n" +
					"varying vec2 texCoord;\n" +
					"uniform sampler2D s_texture_1;\n" +

					"void main(void)\n" +
					"{\n" +
					"	gl_FragColor = texture2D(s_texture_1, texCoord);\n" +
					"}\n";


	private static final float[] VERTEX_BUF = {
			-0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f
	};

	private static final byte[] INDEX_BUF = {
			0, 1, 2, 2, 3, 0
	};

	private static final float[] TEXTURE_COORD_BUF = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
	};

	private boolean initDone = false;

	private FloatBuffer vertexBuffer;
	private ByteBuffer indexBuffer;
	private FloatBuffer textureCoordBuff;

	private Bitmap textureBitmap;

	private int shaderProgramId = 0;
	private int positionHandle;
	private int textureCoordHandle;
	private int mvpMatrixHandle;
	private int textureHandle;
	private int[] textureNames;
	private int minX = 0;
	private int minY = 0;
	private int maxX = 0;
	private int maxY = 0;

	private float[] localMvpMatrix = new float[16];
	private float [] projectionMatrix = new float[16];
	private float[] modelMatrix = new float[16];
	private float[] translation = new float[16];
	private float[] scale = new float[16];
	private float[] rotation = new float[16];

	public Sticker() {
		textureNames = new int[1];
		ByteBuffer bb = ByteBuffer.allocateDirect(VERTEX_BUF.length * 4);
		bb.order(ByteOrder.nativeOrder());
		vertexBuffer = bb.asFloatBuffer();
		vertexBuffer.put(VERTEX_BUF);
		vertexBuffer.position(0);

		indexBuffer = ByteBuffer.allocateDirect(INDEX_BUF.length);
		indexBuffer.order(ByteOrder.nativeOrder());
		indexBuffer.put(INDEX_BUF);
		indexBuffer.position(0);

		bb = ByteBuffer.allocateDirect(TEXTURE_COORD_BUF.length * 4);
		bb.order(ByteOrder.nativeOrder());
		textureCoordBuff = bb.asFloatBuffer();
		textureCoordBuff.put(TEXTURE_COORD_BUF);
		textureCoordBuff.position(0);

		Matrix.setIdentityM(projectionMatrix, 0);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.setIdentityM(translation, 0);
		Matrix.setIdentityM(scale, 0);
		Matrix.setIdentityM(rotation, 0);
	}

	public void init() {
		initDone = false;
	}

	private void initGL() {
		int vertexShader = ShaderUtil.loadShader(GLES20.GL_VERTEX_SHADER, VERTEX_SHADER_SRC);
		int fragmentShader = ShaderUtil.loadShader(GLES20.GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);

		shaderProgramId = GLES20.glCreateProgram();
		GLES20.glAttachShader(shaderProgramId, vertexShader);
		GLES20.glAttachShader(shaderProgramId, fragmentShader);
		GLES20.glLinkProgram(shaderProgramId);

		positionHandle = GLES20.glGetAttribLocation(shaderProgramId, "vertexPosition");
		textureCoordHandle = GLES20.glGetAttribLocation(shaderProgramId, "vertexTexCoord");

		mvpMatrixHandle = GLES20.glGetUniformLocation(shaderProgramId, "modelViewProjectionMatrix");

		GLES20.glGenTextures(1, textureNames, 0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureNames[0]);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);

		textureHandle = GLES20.glGetUniformLocation(shaderProgramId, "s_texture_1");

		GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, textureBitmap, 0);
	}

	public void setTexture(Bitmap bitmap) {
		this.textureBitmap = bitmap;
	}

	public void draw(float[] transform) {
		if (!initDone) {
			initGL();
			initDone = true;
		}

		GLES20.glUseProgram(shaderProgramId);

		GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false,
				0, vertexBuffer);
		GLES20.glEnableVertexAttribArray(positionHandle);

		GLES20.glVertexAttribPointer(textureCoordHandle, 2, GLES20.GL_FLOAT, false,
				0, textureCoordBuff);
		GLES20.glEnableVertexAttribArray(textureCoordHandle);

		float[] transformTranspose = new float[16];
		Matrix.transposeM(transformTranspose, 0, transform, 0);
		Matrix.multiplyMM(modelMatrix, 0, scale, 0, rotation, 0);
		Matrix.multiplyMM(modelMatrix, 0, translation, 0, modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, transformTranspose, 0, modelMatrix, 0);

		minX = (int) (modelMatrix[12] - scale[0] / 2);
		maxX = (int) (modelMatrix[12] + scale[0] / 2);
		minY = (int) (modelMatrix[13] - scale[0] / 2);
		maxY = (int) (modelMatrix[13] + scale[0] / 2);

		Matrix.multiplyMM(localMvpMatrix, 0, projectionMatrix, 0, modelMatrix, 0);
		GLES20.glUniformMatrix4fv(mvpMatrixHandle, 1, false, localMvpMatrix, 0);

		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);

		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glUniform1i(textureHandle, 0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureNames[0]);

		GLES20.glDrawElements(GLES20.GL_TRIANGLES, QUAD_IDX_BUFF_LENGTH,
				GLES20.GL_UNSIGNED_BYTE, indexBuffer);

		GLES20.glDisableVertexAttribArray(positionHandle);
		GLES20.glDisableVertexAttribArray(textureCoordHandle);

		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
	}

	public void setProjectionMatrix(float [] projectionMatrix) {
		this.projectionMatrix = projectionMatrix;
	}

	public void setScale(float x, float y, float z) {
		Matrix.setIdentityM(scale, 0);
		Matrix.scaleM(scale, 0, x, y, z);
	}

	public void setTranslate(float x, float y, float z) {
		Matrix.setIdentityM(translation, 0);
		Matrix.translateM(translation, 0, x, y, z);
	}

	public void setRotation(float angle, float x, float y, float z) {
		Matrix.setIdentityM(rotation, 0);
		Matrix.rotateM(rotation, 0, angle, x, y, z);
	}

	public boolean isTouched(int touchX, int touchY) {
		return (touchX >= minX && touchX <= maxX && touchY >= minY && touchY <= maxY);
	}
}
