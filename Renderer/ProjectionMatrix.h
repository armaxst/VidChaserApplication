/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma once

#include <memory>
#include "Matrix44F.h"

using namespace std;

class ProjectionMatrix
{
public:
	static shared_ptr<ProjectionMatrix> getInstance();
	~ProjectionMatrix();

	void setImageSize(int width, int height);
	void setSurfaceSize(int width, int height);
	Matrix44F getProjectionMatrix();
	int getImageWidth();
	int getImageHeight();
	int getSurfaceWidth();
	int getSurfaceHeight();

private:
	static shared_ptr<ProjectionMatrix> instance;

	ProjectionMatrix();

	Matrix44F projectionMatrix;
	int imageWidth = 0;
	int imageHeight = 0;
	int surfaceWidth = 0;
	int surfaceHeight = 0;
	bool needToCalcProjectionMatrix = true;
};