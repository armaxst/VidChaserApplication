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

extern "C"
{
	static int surfaceWidth = 0;
	static int surfaceHeight = 0;
	static Matrix44F vpMatrix44F;
	static Renderer::Sticker * sticker = nullptr;

	JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_initRendering
	(JNIEnv * env, jclass obj)
	{
		maxstAR::initRendering();
		Renderer::initRendering();

		sticker = nullptr;
	}

	JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_updateRendering
	(JNIEnv * env, jclass obj, jint width, jint height)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		maxstAR::updateRendering(width, height);
		surfaceWidth = width;
		surfaceHeight = height;
	}

	JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_drawFrame
	(JNIEnv * env, jclass obj)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, surfaceWidth, surfaceHeight);
		maxstAR::renderScene();

		glDisable(GL_DEPTH_TEST);
		if (sticker != nullptr)
		{
			sticker->draw();
		}
		glEnable(GL_DEPTH_TEST);
	}

	JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_getImageCoordiFromScreenCoordi
		(JNIEnv * env, jclass clazz, jint surfaceWidth, jint surfaceHeight, jint imageWidth, jint imageHeight, 
		jint surfaceX, jint surfaceY, jintArray imageCoordi)
	{
		jint * nativeInt = (jint *)env->GetPrimitiveArrayCritical(imageCoordi, 0);
		if (nativeInt == nullptr)
		{
			return;
		}

		int imageCoordX = 0;
		int imageCoordY = 0;

		CoordiCvtUtil::GetImageCoordiFromScreenCoordi(surfaceWidth, surfaceHeight, imageWidth, imageHeight,
			surfaceX, surfaceY, imageCoordX, imageCoordY);

		nativeInt[0] = imageCoordX;
		nativeInt[1] = imageCoordY;

		env->ReleasePrimitiveArrayCritical(imageCoordi, nativeInt, JNI_ABORT);
	}

	JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_getGLCoordiFromScreenCoordi
		(JNIEnv * env, jclass clazz, jint surfaceWidth, jint surfaceHeight, jint imageWidth, jint imageHeight,
		jint surfaceX, jint surfaceY, jfloatArray glCoordi)
	{
		jfloat * nativeFloat = (jfloat *)env->GetPrimitiveArrayCritical(glCoordi, 0);
		if (nativeFloat == nullptr)
		{
			return;
		}

		float glX = 0.0f;
		float glY = 0.0f;

		CoordiCvtUtil::GetGLCoordiFromScreenCoordi(surfaceWidth, surfaceHeight,
			imageWidth, imageHeight, surfaceX, surfaceY, glX, glY);

        nativeFloat[0] = glX;
        nativeFloat[1] = glY;

		env->ReleasePrimitiveArrayCritical(glCoordi, nativeFloat, JNI_ABORT);

		MatrixUtil::GetVPMatrix(surfaceWidth, surfaceHeight, imageWidth, imageHeight, vpMatrix44F);
	}

	JNIEXPORT void JNICALL Java_com_maxst_vidchaser_renderer_VidChaserRenderer_addSticker
		(JNIEnv * env, jclass clazz, jint textureWidth, jint textureHeight, jbyteArray textureData, jfloat glPositionX, jfloat glPositionY, jfloat scale)
	{
	    jbyte * nativeBytes = env->GetByteArrayElements(textureData, 0);
        sticker = Renderer::createSticker();
        sticker->init(textureWidth, textureHeight, (unsigned char *)nativeBytes);
        sticker->setScale(scale, scale, 1.0f);
        sticker->setTranslate(glPositionX, glPositionY, 0.0f);
        sticker->setVPMatrix(vpMatrix44F);
        env->ReleaseByteArrayElements(textureData, nativeBytes, JNI_ABORT);
	}
}
