/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#include "Sticker.h"
#include "ProjectionMatrix.h"
#include "Logger.h"

#ifdef __IOS__
#include <VidChaserEngine/VidChaserAPI.h>
#include <VidChaserEngine/CoordiCvtUtil.h>
#else
#include <VidChaserAPI.h>
#include <CoordiCvtUtil.h>
#endif

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

namespace Renderer
{
	Sticker::Sticker()
	{
        trackableId = -1;
		glProgram = 0;
		positionHandle = 0;
		textureCoordHandle = 0;
		mvpMatrixHandle = 0;
		textureHandle = 0;

		vertexBuff = stickerVertices;
		indexBuff = stickerIndices;
		textureCoordBuff = stickerTextureCoords;
        
        transformMatrixRecords.clear();
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

	void Sticker::draw(int imageIndex)
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

        if(imageIndex == lastIndex)
        {
            VidChaser::removeTrackingPosition(trackableId);
            touchIndex = -1;
            lastIndex = -1;
            trackableId = -1;
        }
        
        if(imageIndex == touchIndex)
        {
			VidChaser::addTrackingPosition(imageCoordX, imageCoordY, &trackableId, trackingMethod);
        }
        
        if(trackableId != -1)
        {
            int processTime = 0;
            Matrix33F transformMatrix33F;
            Matrix44F transformMatrix44F, mvpMatrix44F;
            
            VidChaser::ResultCode temp = VidChaser::getTrackingResult(&transformMatrix33F.m[0][0], trackableId, &processTime);
            
            if(temp == VidChaser::ResultCode::SUCCESS)
            {
                MatrixUtil::GetTransformMatrix44F(
					ProjectionMatrix::getInstance()->getImageWidth(), 
					ProjectionMatrix::getInstance()->getImageHeight(), 
					transformMatrix44F, transformMatrix33F, mvpMatrix44F);
                
                transformMatrixRecords[imageIndex] = transformMatrix44F;
            }
            
            if(touchIndex < imageIndex)
            {
                touchIndex = imageIndex;
            }
        }

		if (imageIndex == 0)
		{
			VidChaser::removeTrackingPosition(trackableId);
            trackableId = -1;
		}
        
        Matrix44F temp;
        if(transformMatrixRecords.find(imageIndex) != transformMatrixRecords.end())
        {
            temp = transformMatrixRecords[imageIndex];
        }
        
		Matrix44F modelMatrix = translation * scale * rotation;
		Matrix44F fullTransform = temp * modelMatrix;
		minX = fullTransform.m[0][3] - scale.m[0][0] / 2;
		maxX = fullTransform.m[0][3] + scale.m[0][0] / 2;
		minY = fullTransform.m[1][3] - scale.m[0][0] / 2;
		maxY = fullTransform.m[1][3] + scale.m[0][0] / 2;

		Matrix44F projectionMatrix = ProjectionMatrix::getInstance()->getProjectionMatrix();
		projectionMatrix = projectionMatrix * fullTransform;
		projectionMatrix = projectionMatrix.Transpose();
		glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, &projectionMatrix.m[0][0]);

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
    
    int Sticker::getTrackableId()
    {
        return trackableId;
    }
    
	void Sticker::setScale(float x, float y, float z)
	{
		scale.SetIdentity();
		scale.Scale(x, y, z);
	}

	void Sticker::setTranslate(float x, float y, float z)
	{
		translation.SetIdentity();
		translation.Translate(x, y, z);
	}

	void Sticker::setRotation(float angle, float x, float y, float z)
	{
		rotation.SetIdentity();
		rotation.Rotate(angle, x, y, z);
	}

    void Sticker::setTransform(int imageIndex, Matrix44F &transform)
    {
		transformMatrixRecords[imageIndex] = transform;
    }

	bool Sticker::isTouched(int touchX, int touchY)
	{
		LOGD("touch x:%d, y:%d", touchX, touchY);
		LOGD("min x:%f, y:%f", minX, minY);
		LOGD("max x:%f, y:%f", maxX, maxY);
		return (touchX >= minX && touchX <= maxX && touchY >= minY && touchY <= maxY);
	}

	void Sticker::startTracking(int imageIndex, int lastIndex, int touchX, int touchY, VidChaser::TrackingMethod trackingMethod)
	{
        this->lastIndex = lastIndex;
		this->trackingMethod = trackingMethod;

        LOGD("Tracking method : %d", trackingMethod);
		CoordiCvtUtil::GetImageCoordiFromScreenCoordi(
			ProjectionMatrix::getInstance()->getSurfaceWidth(), 
			ProjectionMatrix::getInstance()->getSurfaceHeight(), 
			ProjectionMatrix::getInstance()->getImageWidth(), 
			ProjectionMatrix::getInstance()->getImageHeight(), 
			touchX, touchY, 
			imageCoordX, imageCoordY);

		VidChaser::addTrackingPosition(imageCoordX, imageCoordY, &trackableId, trackingMethod);
		touchIndex = imageIndex;
	}
    
    void Sticker::stopTracking()
    {
        VidChaser::removeTrackingPosition(trackableId);
        trackableId = -1;
        touchIndex = -1;
        lastIndex = -1;
        transformMatrixRecords.clear();
    }
}
