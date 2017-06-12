#pragma once

#if defined VIDCHASER_EXPORTS
#  define VIDCHASER_API __declspec(dllexport)
#else
#  define VIDCHASER_API
#endif

#include "VidChaserResultCode.h"
#include "VidChaserDefine.h"

namespace VidChaser {
	/*
	* @brief get engine version.
	* @return Engine version text.
	*/
	VIDCHASER_API char* getEngineVersion();

	/*
	* @brief create Engine.
	* @return result code.
	*/
	VIDCHASER_API ResultCode create();

	/*
	* @brief Add tracking position(image coordinate).
	* @param x tracking position x(pixel).
	* @param y tracking position y(pixel).
	* @param idx output tracking position index.
	* @param trackingMethod select tracking method.\n
	*		0 : Affine transform.\n
	*		1 : Homography transform.\n
	*		2 : Rigid transform.\n
	*		3 : translation.\n
	* @return result code.
	*/
	VIDCHASER_API ResultCode addTrackingPosition(int x, int y, int* idx, TrackingMethod trackingMethod = TrackingMethod::TRANSLATION);

	/*
	* @brief Remove the tracking position of the corresponding input index.
	* @param idx index you want to delete.
	* @return result code.
	*/
	VIDCHASER_API ResultCode removeTrackingPosition(int idx);

	/*
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
	VIDCHASER_API ResultCode setTrackingImage(unsigned char* image, int width, int height, int colorFormat, long imageIdx);

	/*
	* @brief get tracking result for the index.
	* @param transfromMatrx3x3 output 3x3 transform matrix.
	* @param idx index you want to get result.
	* @param millis  output tracking time(milli second).
	*/
	VIDCHASER_API ResultCode getTrackingResult(float* transformMatrix3x3, int idx, int* millis);

	/*
	* @brief destory tracking engine.
	* @return result code.
	*/
	VIDCHASER_API ResultCode destroy();
}