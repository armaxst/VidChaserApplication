#pragma once

#if defined VIDCHASER_EXPORTS
#  define VIDCHASER_API __declspec(dllexport)
#else
#  define VIDCHASER_API
#endif

#include "VidChaserResultCode.h"
#include "VidChaserDefine.h"
#include "CameraPreviewCallback.h"
#include <string>

namespace VidChaser {
	/**
	* @brief get engine version.
	* @return Engine version text.
	*/
	VIDCHASER_API char* getEngineVersion();

    /**
     * @brief start a #cameraId camera with width and height resolution
     * @param cameraId camera hardware index(cameraId >= 0)
     * @param width pixel unit camera image resolution width (width > 0)
     * @param height pixel unit camera image resolution height (height > 0)
     * @param extra when use video, set image path (optional)
     */
   	VIDCHASER_API void startCamera(int cameraId, int width, int height, std::string extra = std::string());
    
    /**
     * @brief close the operating camera
     */
    VIDCHASER_API void stopCamera();
    
    /**
     * @brief register callback using camera preview buffer
     * @param callback preview callback
     */
    VIDCHASER_API void setPreviewCallback(CameraPreviewCallback callback);
    
    /**
     * @brief initialize an internal resource for rendering
     */
    VIDCHASER_API void initRendering();

    /**
     * @brief input information necessary for calculating projection matrix
     * @param screenWidth pixel unit screen resolution width (width > 0)
     * @param screenHeight pixel unit screen resolution height (height > 0)
     */
    VIDCHASER_API void updateRendering(int screenWidth, int screenHeight);
    
    /**
     * @brief set Screen Orientation
     * @param orientation screen orientation
     */
    VIDCHASER_API void setScreenOrientation(ScreenOrientation orientation);
    
    /**
     * @brief draw background Image.
     */
    VIDCHASER_API void renderScene();
    
    /**
     * @brief deinitialize an internal resource for rendering
     */
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
