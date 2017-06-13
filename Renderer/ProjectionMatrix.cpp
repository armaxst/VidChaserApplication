/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/

#include "ProjectionMatrix.h"
#include <MatrixUtil.h>
#include "Logger.h"

using namespace std;

shared_ptr<ProjectionMatrix> ProjectionMatrix::instance = nullptr;

shared_ptr<ProjectionMatrix> ProjectionMatrix::getInstance()
{
	if (instance == nullptr)
	{
		instance = shared_ptr<ProjectionMatrix>(new ProjectionMatrix());
	}

	return instance;
}

ProjectionMatrix::ProjectionMatrix()
{
}

ProjectionMatrix::~ProjectionMatrix()
{
}

void ProjectionMatrix::setImageSize(int width, int height)
{
	if (imageWidth != width || imageHeight != height)
	{
		imageWidth = width;
		imageHeight = height;
		needToCalcProjectionMatrix = true;

		LOGI("Image with : %d, height : %d", imageWidth, imageHeight);
	}
}

void ProjectionMatrix::setSurfaceSize(int width, int height)
{
	if (surfaceWidth != width || surfaceHeight != height)
	{
		surfaceWidth = width;
		surfaceHeight = height;
		needToCalcProjectionMatrix = true;

		LOGI("Suface with : %d, height : %d", surfaceWidth, surfaceHeight);
	}
}

Matrix44F ProjectionMatrix::getProjectionMatrix()
{
	if (needToCalcProjectionMatrix)
	{
		float widthRatio = (float)surfaceWidth / imageWidth;
		float heightRatio = (float)surfaceHeight / imageHeight;

		float projectionWidth = imageWidth / 2.0f;
		float projectionHeight = imageHeight / 2.0f;

		//reduce projection frustum area for keep frame aspect ratio
		if (widthRatio > heightRatio)
		{
			projectionHeight = surfaceHeight / (widthRatio * 2.0f);
		}
		else
		{
			projectionWidth = surfaceWidth / (heightRatio * 2.0f);
		}

		Matrix44F viewMatrix;
		viewMatrix.SetIdentity();
		MatrixUtil::LookAt(viewMatrix, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		projectionMatrix.SetIdentity();
		MatrixUtil::Ortho(projectionMatrix,
			-projectionWidth, projectionWidth,
			-projectionHeight, projectionHeight, -100.0f, 100.0f);

		projectionMatrix = projectionMatrix * viewMatrix;
		needToCalcProjectionMatrix = false;
	}

	return projectionMatrix;
}

int ProjectionMatrix::getImageWidth()
{
	return imageWidth;
}

int ProjectionMatrix::getImageHeight()
{
	return imageHeight;
}

int ProjectionMatrix::getSurfaceWidth()
{
	return surfaceWidth;
}

int ProjectionMatrix::getSurfaceHeight()
{
	return surfaceHeight;
}