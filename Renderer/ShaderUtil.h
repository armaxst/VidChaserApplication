/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma once

#ifdef __ANDROID__
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif __IOS__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif _WIN32
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Logger.h"

using namespace std;

namespace Renderer
{
	class ShaderUtil
	{
	public:
		static GLuint loadShader(GLenum shaderType, const char* pSource);
		static GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
		static void checkGlError(const char* op);
	};
}
