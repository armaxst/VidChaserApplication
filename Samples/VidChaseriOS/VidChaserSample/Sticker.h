/*
 * Copyright 2017 Maxst, Inc. All Rights Reserved.
 */
#pragma once

#include <VidChaserEngine/ShaderUtil.h>
#include <VidChaserEngine/MatrixUtil.h>
#include <VidChaserEngine/Logger.h>
#include <VidChaserEngine/vecmath.h>

using namespace std;

class Sticker
{
public:
    Sticker();
    ~Sticker();
    
    void init();
    void setProjectionMatrix(const gl_helper::Mat4 & projectionMatrix);
    void setTexture(int textureWidth, int textureHeight, unsigned char * rgba32TextureData);
    void draw(gl_helper::Mat4 inputTransformMatrix);
    void setScale(float x, float y, float z = 1.0f);
    void setTranslate(float x, float y, float z = 1.0f);
    void setRotation(float angle, float x, float y, float z);
    bool isTouched(int touchX, int touchY);
    
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
    
    gl_helper::Mat4 projection;
    gl_helper::Mat4 translation;
    gl_helper::Mat4 scale;
    gl_helper::Mat4 rotation;
    
    float minX, minY;
    float maxX, maxY;
};
