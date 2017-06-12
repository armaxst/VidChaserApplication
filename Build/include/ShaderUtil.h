#pragma once

#include <stdlib.h>
#include <string>
#include "MaxstARExports.h"

class MAXSTAR_API ShaderUtil
{
public:
	static unsigned int createProgram(const char * vertexString, const char * fragmentString);

	static unsigned int loadShader(unsigned int shaderType, const char *pSource);

	static void checkGlError(const char *op);
};