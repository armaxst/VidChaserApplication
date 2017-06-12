#pragma once

#include <string>
#include "MaxstARDef.h"
#include <MatrixUtil.h>
#include "CameraPreviewCallback.h"

#if (defined WIN32) && defined MAXSTAR_EXPORTS
#  define MAXSTAR_API __declspec(dllexport)
#else
#  define MAXSTAR_API
#endif

using namespace std;

namespace maxstAR
{
	// Camera api
	MAXSTAR_API void startCamera(int cameraId, int width, int height, string extra = string());
	MAXSTAR_API void stopCamera();
	MAXSTAR_API int getCameraWidth();
	MAXSTAR_API int getCameraHeight();
	MAXSTAR_API void setNewFrame(Byte * buf, int length, int width, int height, ColorFormat format);
	MAXSTAR_API void setPreviewCallback(CameraPreviewCallback callback);
	// Camera api

	// Renderer api
	MAXSTAR_API void setAppSignature(string signature);
	MAXSTAR_API void initRendering();
	MAXSTAR_API void updateRendering(int screenWidth, int screenHeight);
	MAXSTAR_API void setScreenOrientation(ScreenOrientation orientation);
	MAXSTAR_API void setClipPlane(float nearClipPlane, float farClipPlane);
	MAXSTAR_API void renderScene();
	MAXSTAR_API void renderSceneToTexture(unsigned int textureId);
	MAXSTAR_API void deinitRendering();
	MAXSTAR_API const Matrix44F getProjectionMatrix();
	MAXSTAR_API void getFarClipPlaneModelMatrix(Matrix44F & matrix);
	// Renderer api
}
