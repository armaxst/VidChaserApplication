#include <jni.h>
#include <string>
#include <MaxstARAPI.h>
#include <MatrixUtil.h>
#include <CoordiCvtUtil.h>
#include <RendererAPI.h>
#include <Logger.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <VidChaserAPI.h>
#include <VidChaserDefine.h>
#include <ProjectionMatrix.h>
#include <cmath>

extern "C"
{
	static int s_surfaceWidth = 0;
	static int s_surfaceHeight = 0;
	static int s_imageIndex = 0;
	static float s_stickerScale = 1.0f;
	static Renderer::Sticker * s_touchedSticker = nullptr;
	static int touchDownX = 0.0f;
	static int touchDownY = 0.0f;
	static const float TOUCH_TOLERANCE = 5.0f;

	JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_initRendering
	(JNIEnv * env, jclass obj)
	{
		maxstAR::initRendering();
		Renderer::initRendering();
	}

	JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_updateRendering
	(JNIEnv * env, jclass obj, jint width, jint height)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		maxstAR::updateRendering(width, height);
		s_surfaceWidth = width;
		s_surfaceHeight = height;
		ProjectionMatrix::getInstance()->setSurfaceSize(s_surfaceWidth, s_surfaceHeight);
	}

	JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_drawFrame
	(JNIEnv * env, jclass obj)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, s_surfaceWidth, s_surfaceHeight);
		maxstAR::renderScene();

		glDisable(GL_DEPTH_TEST);
		Renderer::draw(s_imageIndex);
		glEnable(GL_DEPTH_TEST);
	}

    JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_deinitRendering
    (JNIEnv * env, jclass obj)
    {
        Renderer::deinit();
    }

    JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_setImageSize
    (JNIEnv * env, jclass obj, jint width, jint height)
    {
        ProjectionMatrix::getInstance()->setImageSize(width, height);
    }

	JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_setImageIndex
	(JNIEnv * env, jclass obj, jint index)
	{
		s_imageIndex = index;
	}

	JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_addSticker
		(JNIEnv * env, jclass clazz, jint textureWidth, jint textureHeight, jbyteArray textureData, jint touchX, jint touchY, jfloat scale)
	{
		float glX = 0.0f;
		float glY = 0.0f;
		s_stickerScale = scale;

		int imageWidth = ProjectionMatrix::getInstance()->getImageWidth();
		int imageHeight = ProjectionMatrix::getInstance()->getImageHeight();

		CoordiCvtUtil::GetGLCoordiFromScreenCoordi(s_surfaceWidth, s_surfaceHeight,
			imageWidth, imageHeight, touchX, touchY, glX, glY);

		LOGD("image width:%d height:%d surface width:%d height:%d", imageWidth, imageHeight, s_surfaceWidth, s_surfaceHeight);
		LOGD("touch x:%d y:%d", touchX, touchY);
		LOGD("gl x:%f y:%f", glX, glY);

	    jbyte * nativeBytes = env->GetByteArrayElements(textureData, 0);
        Renderer::Sticker * sticker = Renderer::createSticker();
        sticker->setTexture(textureWidth, textureHeight, (unsigned char *)nativeBytes);
        sticker->setScale(scale, scale, 1.0f);
        sticker->setTranslate(glX, glY, 0.0f);

        Renderer::addSticker(sticker);
        env->ReleaseByteArrayElements(textureData, nativeBytes, JNI_ABORT);
	}

    JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_removeSticker
        (JNIEnv * env, jclass clazz, jlong stickerPointer)
    {
        s_touchedSticker = (Renderer::Sticker *)stickerPointer;
        Renderer::removeSticker(s_touchedSticker);
        s_touchedSticker = nullptr;
    }

	JNIEXPORT jlong Java_com_maxst_vidchaser_renderer_VidChaserRenderer_setTouchEvent
		(JNIEnv * env, jclass clazz, jint touchX, jint touchY, jint touchEvent)
	{
		float glX = 0.0f;
		float glY = 0.0f;
		int imageWidth = ProjectionMatrix::getInstance()->getImageWidth();
		int imageHeight = ProjectionMatrix::getInstance()->getImageHeight();

		switch(touchEvent)
		{
		case 0:
            CoordiCvtUtil::GetGLCoordiFromScreenCoordi(s_surfaceWidth, s_surfaceHeight,
                imageWidth, imageHeight, touchX, touchY, glX, glY);

			s_touchedSticker = Renderer::getTouchedSticker((int)glX, (int)glY);
            touchDownX = touchX;
            touchDownY = touchY;
            break;

		case 1:
			if (s_touchedSticker != nullptr)
			{
                s_touchedSticker->setScale(s_stickerScale, s_stickerScale, 1.0f);
			}
			break;

		case 2:
			if (s_touchedSticker != nullptr)
			{
                if (std::abs(touchDownX - touchX) > 5 || std::abs(touchDownY - touchY) > 5)
                {
                    CoordiCvtUtil::GetGLCoordiFromScreenCoordi(s_surfaceWidth, s_surfaceHeight,
                        imageWidth, imageHeight, touchX, touchY, glX, glY);

                    s_touchedSticker->setTranslate(glX, glY, 0.0f);
                    touchDownX = touchX;
                    touchDownY = touchY;
                }
			}
			break;

		case 3:
		    if (s_touchedSticker != nullptr)
		    {
		        s_touchedSticker->setScale(s_stickerScale * 1.3f, s_stickerScale * 1.3f, 1.0f);
		    }
		    break;
		}

		return (long)s_touchedSticker;
	}

    JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_startTracking
    (JNIEnv * env, jclass obj, jlong stickerPointer, int imageIndex, int touchX, int touchY, int trackingMethod)
    {
        s_touchedSticker = (Renderer::Sticker *)stickerPointer;
        s_touchedSticker->startTracking(imageIndex, touchX, touchY, (VidChaser::TrackingMethod)trackingMethod);
    }

    JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_stopTracking
    (JNIEnv * env, jclass obj, jlong stickerPointer)
    {
        s_touchedSticker = (Renderer::Sticker *)stickerPointer;
        s_touchedSticker->stopTracking();
    }
}
