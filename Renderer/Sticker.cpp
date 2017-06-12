/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#include "Sticker.h"

#include <iostream>

using namespace std;

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
		glProgram = 0;
		positionHandle = 0;
		textureCoordHandle = 0;
		mvpMatrixHandle = 0;
		textureHandle = 0;

		vertexBuff = stickerVertices;
		indexBuff = stickerIndices;
		textureCoordBuff = stickerTextureCoords;
	}

	Sticker::~Sticker()
	{

	}

	void Sticker::init(int textureWidth, int textureHeight, unsigned char * rgba32TextureData)
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
			GL_RGBA, GL_UNSIGNED_BYTE, rgba32TextureData);
	}

	void Sticker::draw()
	{
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

		Matrix44F fullTransform = transformMatrix * modelMatrix;

		mvpMatrix = vpMatrix * fullTransform;
		mvpMatrix = mvpMatrix.Transpose();
		glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, &mvpMatrix.m[0][0]);

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
		modelMatrix.Scale(x, y, z);
	}

	void Sticker::setTranslate(float x, float y, float z)
	{
		modelMatrix.Translate(x, y, z);
	}

	void Sticker::setRotation(float angle, float x, float y, float z)
	{
		modelMatrix.Rotate(angle, x, y, z);
	}

	void Sticker::applyTransform(Matrix44F & transform)
	{
		transformMatrix = transform;
	}
    
    void Sticker::setVPMatrix(Matrix44F & vp)
    {
        vpMatrix = vp;
    }
    
    void Sticker::setTimeStamp(long& time)
    {
        timeStamp = time;
    }
    
    long Sticker::getTimeStamp()
    {
        return timeStamp;
    }
}
