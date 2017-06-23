/*
 * Copyright 2017 Maxst, Inc. All Rights Reserved.
 */

#include "ProjectionMatrix.h"

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
    }
}

void ProjectionMatrix::setSurfaceSize(int width, int height)
{
    if (surfaceWidth != width || surfaceHeight != height)
    {
        surfaceWidth = width;
        surfaceHeight = height;
        needToCalcProjectionMatrix = true;
    }
}

gl_helper::Mat4 ProjectionMatrix::getProjectionMatrix()
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
        
        gl_helper::Mat4 viewMatrix;
        viewMatrix = gl_helper::Mat4::Identity();
        viewMatrix = gl_helper::Mat4::LookAt(gl_helper::Vec3(0, 0, 3), gl_helper::Vec3(0, 0, 0), gl_helper::Vec3(0, 1, 0));
        
        projectionMatrix = gl_helper::Mat4::Identity();
        projectionMatrix = gl_helper::Mat4::Ortho(-projectionWidth, projectionWidth, -projectionHeight, projectionHeight, -100.f, 100.f);
        
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
