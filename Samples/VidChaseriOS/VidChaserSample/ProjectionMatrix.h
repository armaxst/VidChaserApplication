/*
 * Copyright 2017 Maxst, Inc. All Rights Reserved.
 */
#pragma once

#include <memory>

#include <VidChaserEngine/MatrixUtil.h>
#include <vidChaserEngine/vecmath.h>

using namespace std;

class ProjectionMatrix
{
public:
    static shared_ptr<ProjectionMatrix> getInstance();
    ~ProjectionMatrix();
    
    void setImageSize(int width, int height);
    void setSurfaceSize(int width, int height);
    gl_helper::Mat4 getProjectionMatrix();
    int getImageWidth();
    int getImageHeight();
    int getSurfaceWidth();
    int getSurfaceHeight();
    
private:
    static shared_ptr<ProjectionMatrix> instance;
    
    ProjectionMatrix();
    
    gl_helper::Mat4 projectionMatrix;
    int imageWidth = 0;
    int imageHeight = 0;
    int surfaceWidth = 0;
    int surfaceHeight = 0;
    bool needToCalcProjectionMatrix = true;
};
