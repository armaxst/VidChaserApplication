/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma once

#include "ShaderUtil.h"
#include <MatrixUtil.h>
#include <map>
#ifdef __IOS__
#include <VidChaserEngine/VidChaserDefine.h>
#else
#include <VidChaserDefine.h>
#endif

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
        void setScale(float x, float y, float z = 1.0f);
		void setTranslate(float x, float y, float z = 1.0f);
		void setRotation(float angle, float x, float y, float z);
        void setTransform(int imageIndex, Matrix44F& transform);
		bool isTouched(int touchX, int touchY);
        
		void startTracking(int imageIndex, int lastIndex, int touchX, int touchY, VidChaser::TrackingMethod trackingMethod);
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
        int touchIndex = -1;
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

		VidChaser::TrackingMethod trackingMethod = VidChaser::TrackingMethod::TRANSLATION;

        map<int, Matrix44F> transformMatrixRecords;
		float minX, minY;
		float maxX, maxY;
	};
}
