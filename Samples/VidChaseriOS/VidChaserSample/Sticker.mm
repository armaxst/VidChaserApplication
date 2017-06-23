/*
 * Copyright 2017 Maxst, Inc. All Rights Reserved.
 */
#include "Sticker.h"
#include <iostream>

using namespace std;

#define STICKER_BOX_SIZE 25
#define	QUAD_IDX_BUFF_LENGTH 6

static const char stickerVertexShader[] =
"attribute vec4 vertexPosition;\n"
"attribute vec2 vertexTexCoord;\n"
"varying vec2 texCoord;\n"
"uniform mat4 modelViewProjectionMatrix;\n"
"void main()							\n"
"{										\n"
"	gl_Position = modelViewProjectionMatrix * vertexPosition;\n"
"	texCoord = vertexTexCoord; 			\n"
"}										\n";

static const char stickerFragmentShader[] =
"precision mediump float;\n"
"varying vec2 texCoord;\n"
"uniform sampler2D s_texture_1;\n"

"void main(void)\n"
"{\n"
"	gl_FragColor = texture2D(s_texture_1, texCoord);\n"
"}\n";

float stickerVertices[] =
{
    -0.5f, 0.5f, 0.0f,   // top left
    -0.5f, -0.5f, 0.0f,   // bottom left
    0.5f, -0.5f, 0.0f,   // bottom right
    0.5f, 0.5f, 0.0f  // top right
};

unsigned char stickerIndices[] =
{
    0, 1, 2, 2, 3, 0
};

float stickerTextureCoords[] =
{
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
};


Sticker::Sticker()
{
    vertexBuff = stickerVertices;
    indexBuff = stickerIndices;
    textureCoordBuff = stickerTextureCoords;
    
    scale = gl_helper::Mat4::Identity();
    rotation = gl_helper::Mat4::Identity();
    translation = gl_helper::Mat4::Identity();
    projection = gl_helper::Mat4::Identity();
}

Sticker::~Sticker()
{
    if (textureData != nullptr)
    {
        delete[] textureData;
        textureData = nullptr;
    }
}

void Sticker::init()
{
    initDone = false;
}

void Sticker::setProjectionMatrix(const gl_helper::Mat4 & projectionMatrix)
{
    this->projection = projectionMatrix;
}

void Sticker::initGL()
{
    glProgram = ShaderUtil::createProgram(stickerVertexShader, stickerFragmentShader);
    ShaderUtil::checkGlError("createProgram");
    
    positionHandle = glGetAttribLocation(glProgram, "vertexPosition");
    ShaderUtil::checkGlError("vertexPosition");
    
    textureCoordHandle = glGetAttribLocation(glProgram, "vertexTexCoord");
    ShaderUtil::checkGlError("vertexTexCoord");
    
    mvpMatrixHandle = glGetUniformLocation(glProgram, "modelViewProjectionMatrix");
    ShaderUtil::checkGlError("modelViewProjectionMatrix");
    
    glGenTextures(1, &textureNames);
    glBindTexture(GL_TEXTURE_2D, textureNames);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    textureHandle = glGetUniformLocation(glProgram, "s_texture_1");
    ShaderUtil::checkGlError("glGetUniformLocation");
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, textureData);
}

void Sticker::setTexture(int textureWidth, int textureHeight, unsigned char * rgba32TextureData)
{
    if (textureData == nullptr)
    {
        textureData = new unsigned char[textureWidth * textureHeight * 4];
        memcpy(textureData, rgba32TextureData, textureWidth * textureHeight * 4);
        this->textureWidth = textureWidth;
        this->textureHeight = textureHeight;
    }
}

void Sticker::draw(gl_helper::Mat4 inputTransformMatrix)
{
    if (!initDone)
    {
        initGL();
        initDone = true;
    }
    
    glUseProgram(glProgram);
    ShaderUtil::checkGlError("glUseProgram");
    
    glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), vertexBuff);
    ShaderUtil::checkGlError("glVertexAttribPointer");
    
    glEnableVertexAttribArray(positionHandle);
    ShaderUtil::checkGlError("glEnableVertexAttribArray");
    
    glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, GL_FALSE,
                          2 * sizeof(float), textureCoordBuff);
    ShaderUtil::checkGlError("glVertexAttribPointer 2");
    
    glEnableVertexAttribArray(textureCoordHandle);
    ShaderUtil::checkGlError("glEnableVertexAttribArray 2");
    
    gl_helper::Mat4 modelMatrix = translation * scale * rotation;
    gl_helper::Mat4 fullTransform = inputTransformMatrix * modelMatrix;
    
    minX = fullTransform.Ptr()[12] - scale.Ptr()[0] / 2;
    maxX = fullTransform.Ptr()[12] + scale.Ptr()[0] / 2;
    minY = fullTransform.Ptr()[13] - scale.Ptr()[0] / 2;
    maxY = fullTransform.Ptr()[13] + scale.Ptr()[0] / 2;
    
    gl_helper::Mat4 tempMvpMatrix = projection * fullTransform;
    glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, tempMvpMatrix.Ptr());
    
    ShaderUtil::checkGlError("glUniformMatrix4fv");
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(textureHandle, 0);
    glBindTexture(GL_TEXTURE_2D, textureNames);
    
    glDrawElements(GL_TRIANGLES, QUAD_IDX_BUFF_LENGTH,
                   GL_UNSIGNED_BYTE, indexBuff);
    
    glDisableVertexAttribArray(positionHandle);
    glDisableVertexAttribArray(textureCoordHandle);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sticker::setScale(float x, float y, float z)
{
    scale = gl_helper::Mat4::Identity();
    scale = gl_helper::Mat4::Scale(x, y, z);
}

void Sticker::setTranslate(float x, float y, float z)
{
    translation = gl_helper::Mat4::Identity();
    translation = gl_helper::Mat4::Translation(x, y, z);
}

void Sticker::setRotation(float angle, float x, float y, float z)
{
    rotation = gl_helper::Mat4::Identity();
    rotation = rotation.PostRotate(angle, x, y, z);
}

bool Sticker::isTouched(int touchX, int touchY)
{
    Log("touch x:%d, y:%d", touchX, touchY);
    Log("min x:%f, y:%f", minX, minY);
    Log("max x:%f, y:%f", maxX, maxY);
    return (touchX >= minX && touchX <= maxX && touchY >= minY && touchY <= maxY);
}
