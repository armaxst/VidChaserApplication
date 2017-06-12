/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma once

#include "ShaderUtil.h"
#include <MatrixUtil.h>

namespace Renderer
{
	class Sticker
	{
	public:
		Sticker();
		~Sticker();

		void init(int textureWidth, int textureHeight, unsigned char * rgba32TextureData);
		void draw();
		void setScale(float x, float y, float z = 1.0f);
		void setTranslate(float x, float y, float z = 1.0f);
		void setRotation(float angle, float x, float y, float z);
		void applyTransform(Matrix44F & transform);
        void setVPMatrix(Matrix44F & vp);
        void setTimeStamp(long &time);
        long getTimeStamp();

	private:
		GLuint glProgram;
		GLuint positionHandle;
		GLuint textureHandle;
		GLuint textureCoordHandle;
		GLuint mvpMatrixHandle;
		GLuint textureNames;

		float * vertexBuff;
		unsigned char *indexBuff;
		float *textureCoordBuff;

		Matrix44F modelMatrix;
		Matrix44F mvpMatrix;
		Matrix44F transformMatrix;
        Matrix44F vpMatrix;
        
        long timeStamp;
	};
}
