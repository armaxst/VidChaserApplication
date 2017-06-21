#pragma once

#if defined VIDCHASER_EXPORTS
#  define VIDCHASER_API __declspec(dllexport)
#else
#  define VIDCHASER_API
#endif

#include "VidChaserResultCode.h"
#include "VidChaserDefine.h"
#include "CameraPreviewCallback.h"
#include "MatrixUtil.h"
#include <string>

namespace VidChaser {
	/**
	* @brief get engine version.
	* @return Engine version text.
	*/
	VIDCHASER_API char* getEngineVersion();

   	VIDCHASER_API void startCamera(int cameraId, int width, int height, std::string extra = std::string());
    
    VIDCHASER_API void stopCamera();
    
    VIDCHASER_API void setPreviewCallback(CameraPreviewCallback callback);
    
    VIDCHASER_API void initRendering();
    
    VIDCHASER_API void updateRendering(int screenWidth, int screenHeight);
    
    VIDCHASER_API void setScreenOrientation(ScreenOrientation orientation);
    
    VIDCHASER_API void renderScene();
    
    VIDCHASER_API void deinitRendering();
    
	/**
	* @brief create Engine.
	* @return result code.
	*/
	VIDCHASER_API ResultCode create(std::string appSignature = "For_Mobile");

	/**
	* @brief Add tracking position(image coordinate).
	* @param x tracking position x(pixel).
	* @param y tracking position y(pixel).
	* @param idx output tracking point index.
	* @param trackingMethod select tracking method.\n
	*		0 : Affine transform.\n
	*		1 : Homography transform.\n
	*		2 : Rigid transform.\n
	*		3 : translation.\n
	* @return result code.
	*/
	VIDCHASER_API ResultCode addTrackingPoint(int x, int y, int* idx, TrackingMethod trackingMethod = TrackingMethod::TRANSLATION);

	/**
	* @brief activate tracking point
	* @param x reset tracking position x(pixel).
	* @param y reset tracking position y(pixel).
	* @param idx tracking point index.
	* return result code.
	*/
	VIDCHASER_API ResultCode activateTrackingPoint(int x, int y, int idx);
    
    /**
     * @brief deactivate tracking point
     * @param idx tracking point index.
     * return result code.
     */
    VIDCHASER_API ResultCode deactivateTrackingPoint(int idx);

	/**
	* @brief Remove the tracking position of the corresponding input index.
	* @param idx index you want to delete.
	* @return result code.
	*/
	VIDCHASER_API ResultCode removeTrackingPoint(int idx);

	/**
	* @brief start tracking using input image.
	* @param image image data.
	* @param width image width.
	* @param height image height.
	* @param colorFormat image pixel format.\n
	*		0 : RGBA8888
	*		1 : RGB888
	*		2 : YUV420sp
	*		3 : YUV420
	*		4 : YUV420_888
	*		5 : GRAY
	* @param imageIdx image index. if input image index equal to previous input image index, tracking logic don't track.
	* @return result code.
	*/
	VIDCHASER_API ResultCode setNewFrame(unsigned char* image, int length, int width, int height, int colorFormat, long imageIdx);

	/**
	* @brief get tracking result for the index.
	* @param transfromMatrx3x3 output 3x3 transform matrix.
	* @param idx index you want to get result.
	* @param millis  output tracking time(milli second).
	*/
	VIDCHASER_API ResultCode getTrackingResult(float* transformMatrix3x3, int idx, int* millis);

	/**
	* @brief destory tracking engine.
	* @return result code.
	*/
	VIDCHASER_API ResultCode destroy();
}
