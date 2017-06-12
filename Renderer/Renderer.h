/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma once

#ifdef __ANDROID__
#include <jni.h>
#include <android/native_window_jni.h>
#endif

#include <sys/types.h>
#include <stdint.h>

typedef void* HRENDERER;

extern "C"
{
	/**
	* @brief Create Renderer.
	* @return Renderer instance
	*/
	RENDERER_API HRENDERER createRenderer();

	/**
	* @brief initialize Renderer.
	* @param handle Renderer instance
	* @param bHaveThread is flag
	*		 true - exist external render thread
	*		 false - no exist external render thread
	* @return 0 when initialize success
	*		  else when initialize fail
	*/
	RENDERER_API int32_t initialize(HRENDERER handle, bool bHaveThread);

	/**
	* @brief deinitialize Renderer.
	* @param handle Renderer instance
	* @return 0 when initialize success
	*		  else when deinitialize fail
	*/
	RENDERER_API int32_t deinitialize(HRENDERER handle);

	/**
	* @brief initialize internal Renderer thread.
	* @param handle Renderer instance
	* @return 0 when initialize success
	*		  else when initRender fail
	*/
	RENDERER_API int32_t initRenderer(HRENDERER handle);

	/**
	* @brief deinitialize internal Renderer thread.
	* @param handle Renderer instance
	* @return 0 when deinitRender success
	*		  else when deinitRender fail
	*/
	RENDERER_API int32_t deinitRenderer(HRENDERER handle);

#ifdef __ANDROID__
	/**
	* @brief set up display window information.
	* @param handle Renderer instance
	* @param window Display window
	* @param width Surface width
	* @param height Surface height
	* @return 0 when setWindow success
	*		  else when setWindow fail
	*/
	RENDERER_API void setWindow(HRENDERER handle, ANativeWindow *window, int32_t width, int32_t height);
#else
	RENDERER_API void setWindow(HRENDERER handle, int32_t width, int32_t height);
#endif 

	/**
	* @brief Draw video background
	* @param handle Renderer instance
	* @param buf frame
	* @param width Frame width
	* @param height Frame height
	* @param pixelFormat Set pixel format 0 is RGBA8888, 1 is RGB888, 2 is GRAYSCALE8
	* @param timestamp Frame timestamp
	* @return 0 when render success
	*		  else when render fail
	*/
	RENDERER_API int32_t renderVideoBackground(HRENDERER handle, unsigned char *buf, int32_t width, int32_t height, int32_t pixelFormat, int64_t timestamp);

	RENDERER_API int32_t drawStickers(HRENDERER handle, int64_t timestamp);

	/**
	* @brief set up attach sticker information.
	* @param handle Renderer instance
	* @param x Sticker point x
	* @param y Sticker point y
	* @param width Sticker width
	* @param height Sticker height
	* @param timestamp Frame timestamp
	* @return 0 when setSticker success
	*		  else when setSticker fail
	*/
	RENDERER_API int32_t setSticker(HRENDERER handle, int32_t x, int32_t y, unsigned char * imgbuf, int32_t width, int32_t height);

	/**
	* @brief Draw sticker with tracking result.
	* @param handle Renderer instance
	* @param stickerId Sticker id
	* @param timestamp Frame timestamp
	* @param mvpMatrix Tracking result
	* @return 0 when success
	*		  others when fail
	*/

	RENDERER_API int32_t pushStickerTrackingResult(HRENDERER handle, int32_t id, int64_t timestamp, float mvpMatrix[16]);

	RENDERER_API void swapBuffers(HRENDERER handle);

	/**
	* @brief Delete Renderer.
	* @param handle Renderer instance
	*/
	RENDERER_API void deleteRenderer(HRENDERER handle);
}