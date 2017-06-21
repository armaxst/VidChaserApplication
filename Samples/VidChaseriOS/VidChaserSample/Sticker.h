/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma once

#ifdef __IOS__
#include <VidChaserEngine/ShaderUtil.h>
#include <VidChaserEngine/MatrixUtil.h>
#include <VidChaserEngine/Logger.h>
#else
#include <ShaderUtil.h>
#include <MatrixUtil.h>
#include <Logger.h>
#endif
#include <map>

using namespace std;

class Sticker
{
public:
	Sticker();
	~Sticker();

	void init();
	void setProjectionMatrix(const Matrix44F & projectionMatrix);
	void setTexture(int textureWidth, int textureHeight, unsigned char * rgba32TextureData);
	void draw(Matrix44F matrix);
	void setScale(float x, float y, float z = 1.0f);
	void setTranslate(float x, float y, float z = 1.0f);
	void setRotation(float angle, float x, float y, float z);
	bool isTouched(int x, int y);

private:
	void initGL();

	bool initDone = false;

	GLuint glProgram = 0;
	GLuint positionHandle = 0;
	GLuint textureHandle = 0;
	GLuint textureCoordHandle = 0;
	GLuint mvpMatrixHandle = 0;
	GLuint textureNames = 0;

	unsigned char * textureData = nullptr;
	int textureWidth;
	int textureHeight;

	float * vertexBuff;
	unsigned char *indexBuff;
	float *textureCoordBuff;

	Matrix44F projection;
	Matrix44F translation;
	Matrix44F scale;
	Matrix44F rotation;

	float minX, minY;
	float maxX, maxY;
};
