/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma once

#include "ShaderUtil.h"
#include <MatrixUtil.h>
#include <map>

using namespace std;

namespace Renderer
{
	class Sticker
	{
	public:
		Sticker();
		~Sticker();

		void init();
		void setTexture(int textureWidth, int textureHeight, unsigned char * rgba32TextureData);
		void draw(int imageIndex);
		void reset();
        int getTrackableId();
        void startTracking(int imageIndex, int touchX, int touchY);
        void setScale(float x, float y, float z = 1.0f);
		void setTranslate(float x, float y, float z = 1.0f);
		void setRotation(float angle, float x, float y, float z);
        void setTransform(int imageIndex, Matrix44F& transform);
		bool isTouched(int touchX, int touchY);
        void stopTracking();
        
	private:
		void initGL();

		bool initDone = false;

		GLuint glProgram;
		GLuint positionHandle;
		GLuint textureHandle;
		GLuint textureCoordHandle;
		GLuint mvpMatrixHandle;
		GLuint textureNames;

        int trackableId = -1;
        int imageCoordX = 0;
        int imageCoordY = 0;
        int lastIndex = -1;
        
        unsigned char * textureData = nullptr;
		int textureWidth;
		int textureHeight;

		float * vertexBuff;
		unsigned char *indexBuff;
		float *textureCoordBuff;

		Matrix44F translation;
		Matrix44F scale;
		Matrix44F rotation;

        map<int, Matrix44F> transformMatrixRecords;
		float minX, minY;
		float maxX, maxY;
	};
}
