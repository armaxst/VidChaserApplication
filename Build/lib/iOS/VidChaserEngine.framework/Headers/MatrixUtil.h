/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma once

#include "Matrix44F.h"
#include "Matrix33F.h"

class MatrixUtil
{
public:
	static float GetLength(float x, float y, float z)
	{
		return (float)sqrtf(x * x + y * y + z * z);
	}

	static void GetImageCoordFromScreenCoord(int screenWidth, int screenHeight, int imageWidth, int imageHeight,
		int screenX, int screenY, int & imageX, int & imageY)
	{
		float widthRatio = (float)screenWidth / imageWidth;
		float heightRatio = (float)screenHeight / imageHeight;

		// When image's upper and lower part cropped.
		float ratio = widthRatio;

		// When image's left and right part cropped.
		if (widthRatio < heightRatio)
		{
			ratio = heightRatio;
		}

		imageX = (screenX - screenWidth / 2) / ratio + imageWidth / 2;
		imageY = -(screenHeight / 2 - screenY) / ratio + imageHeight / 2;
	}

	static void GetGLCoordFromScreenCoord(
		int screenWidth, int screenHeight, int imageWidth, int imageHeight,
		int screenX, int screenY, float & glX, float & glY)
	{
		float widthRatio = (float)screenWidth / imageWidth;
		float heightRatio = (float)screenHeight / imageHeight;

		int screenCenterX = screenWidth / 2;
		int screenCenterY = screenHeight / 2;

		float ratio = widthRatio;

		glX = (float)screenX - screenCenterX;
		glY = (float)screenCenterY - screenY;

		// When image's left and right part cropped.
		if (widthRatio < heightRatio)
		{
			ratio = heightRatio;
		}

		glX = ((float)screenX - screenCenterX) / ratio;
		glY = ((float)screenCenterY - screenY) / ratio;
	}

	static void GetGLCoordinateFromTouchPoint(
		int surfaceWidth, int surfaceHeight, int imageWidth, int imageHeight, int touchX, int touchY,
		float & resultGLX, float & resultGLY)
	{
		float wr = (float)surfaceWidth / imageWidth;
		float hr = (float)surfaceHeight / imageHeight;

		int cx = surfaceWidth / 2;
		int cy = surfaceHeight / 2;

		// image w / h ratio is greater than screen w / h ratio
		if (wr > hr)
		{
			float convertSW = surfaceWidth / wr;
			float convertSH = surfaceHeight / wr;

			float diffH = imageHeight - convertSH;

			resultGLX = (float)touchX - cx;
			resultGLY = (float)cy - touchY;

			resultGLX = resultGLX / wr;
			resultGLY = resultGLY / wr;
		}
		// Screen w / h ratio is greater than image w / h ratio
		else
		{
			float convertSW = surfaceWidth / hr;
			float convertSH = surfaceHeight / hr;

			float diffW = imageWidth - convertSW;

			resultGLX = (float)touchX - cx;
			resultGLY = (float)cy - touchY;

			resultGLX = resultGLX / hr;
			resultGLY = resultGLY / hr;
		}
	}

	static Matrix44F GetProjectionMatrix(int imageWidth, int imageHeight, int surfaceWidth, int surfaceHeight)
	{
		float widthRatio = (float)surfaceWidth / imageWidth;
		float heightRatio = (float)surfaceHeight / imageHeight;

		float projectionWidth = imageWidth / 2.0f;
		float projectionHeight = imageHeight / 2.0f;

		//reduce projection frustum area for keep frame aspect ratio
		if (widthRatio > heightRatio)
		{
			projectionHeight = surfaceHeight / (widthRatio * 2.0f);
		}
		else
		{
			projectionWidth = surfaceWidth / (heightRatio * 2.0f);
		}

		Matrix44F projectionMatrix;
		Matrix44F viewMatrix;
		viewMatrix.SetIdentity();
		LookAt(viewMatrix, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		projectionMatrix.SetIdentity();
		Ortho(projectionMatrix,
			-projectionWidth, projectionWidth,
			-projectionHeight, projectionHeight, -100.0f, 100.0f);

		projectionMatrix = projectionMatrix * viewMatrix;
		return projectionMatrix;
	}

	static void LookAt(Matrix44F &result, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
	{
		// See the OpenGL GLUT documentation for gluLookAt for a description
		// of the algorithm. We implement it in a straightforward way:

		float fx = centerX - eyeX;
		float fy = centerY - eyeY;
		float fz = centerZ - eyeZ;

		// Normalize f
		float rlf = 1.0f / GetLength(fx, fy, fz);
		fx *= rlf;
		fy *= rlf;
		fz *= rlf;

		// compute s = f x up (x means "cross product")
		float sx = fy * upZ - fz * upY;
		float sy = fz * upX - fx * upZ;
		float sz = fx * upY - fy * upX;

		// and normalize s
		float rls = 1.0f / GetLength(sx, sy, sz);
		sx *= rls;
		sy *= rls;
		sz *= rls;

		// compute u = s x f
		float ux = sy * fz - sz * fy;
		float uy = sz * fx - sx * fz;
		float uz = sx * fy - sy * fx;

		Matrix44F lookAt;

		lookAt.m[0][0] = sx;
		lookAt.m[0][1] = ux;
		lookAt.m[0][2] = -fx;
		lookAt.m[0][3] = 0.0f;

		lookAt.m[1][0] = sy;
		lookAt.m[1][1] = uy;
		lookAt.m[1][2] = -fy;
		lookAt.m[1][3] = 0.0f;

		lookAt.m[2][0] = sz;
		lookAt.m[2][1] = uz;
		lookAt.m[2][2] = -fz;
		lookAt.m[2][3] = 0.0f;

		lookAt.m[3][0] = 0.0f;
		lookAt.m[3][1] = 0.0f;
		lookAt.m[3][2] = 0.0f;
		lookAt.m[3][3] = 1.0f;

		lookAt.Translate(-eyeX, -eyeY, -eyeZ);

		//Matrix44FMultiply(result, lookAt, result);
		result = lookAt * result;
	}

	static void Ortho(Matrix44F &result, float left, float right, float bottom, float top, float nearZ, float farZ)
	{
		float deltaX = right - left;
		float deltaY = top - bottom;
		float deltaZ = farZ - nearZ;

		Matrix44F ortho;

		if ((deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f))
			return;

		ortho.SetIdentity();

		ortho.m[0][0] = 2.0f / deltaX;
		ortho.m[3][0] = -(right + left) / deltaX;
		ortho.m[1][1] = 2.0f / deltaY;
		ortho.m[3][1] = -(top + bottom) / deltaY;
		ortho.m[2][2] = -2.0f / deltaZ;
		ortho.m[3][2] = -(nearZ + farZ) / deltaZ;

		//Matrix44FMultiply(result, ortho, result);
		result = ortho * result;
	}

	static void Matrix44FMultiply(Matrix44F &result, const Matrix44F &srcA, const Matrix44F &srcB)
	{
		Matrix44F tmp;
		for (int i = 0; i < 4; i++)
		{
			tmp.m[i][0] =
				(srcA.m[i][0] * srcB.m[0][0]) +
				(srcA.m[i][1] * srcB.m[1][0]) +
				(srcA.m[i][2] * srcB.m[2][0]) +
				(srcA.m[i][3] * srcB.m[3][0]);

			tmp.m[i][1] =
				(srcA.m[i][0] * srcB.m[0][1]) +
				(srcA.m[i][1] * srcB.m[1][1]) +
				(srcA.m[i][2] * srcB.m[2][1]) +
				(srcA.m[i][3] * srcB.m[3][1]);

			tmp.m[i][2] =
				(srcA.m[i][0] * srcB.m[0][2]) +
				(srcA.m[i][1] * srcB.m[1][2]) +
				(srcA.m[i][2] * srcB.m[2][2]) +
				(srcA.m[i][3] * srcB.m[3][2]);

			tmp.m[i][3] =
				(srcA.m[i][0] * srcB.m[0][3]) +
				(srcA.m[i][1] * srcB.m[1][3]) +
				(srcA.m[i][2] * srcB.m[2][3]) +
				(srcA.m[i][3] * srcB.m[3][3]);
		}
		memcpy(&result, &tmp, sizeof(Matrix44F));
	}

	static void Matrix33FMultiply(Matrix33F &result, const Matrix33F &srcA, const Matrix33F &srcB)
	{
		Matrix33F tmp;
		for (int i = 0; i < 3; i++)
		{
			tmp.m[i][0] =
				(srcA.m[i][0] * srcB.m[0][0]) +
				(srcA.m[i][1] * srcB.m[1][0]) +
				(srcA.m[i][2] * srcB.m[2][0]);

			tmp.m[i][1] =
				(srcA.m[i][0] * srcB.m[0][1]) +
				(srcA.m[i][1] * srcB.m[1][1]) +
				(srcA.m[i][2] * srcB.m[2][1]);

			tmp.m[i][2] =
				(srcA.m[i][0] * srcB.m[0][2]) +
				(srcA.m[i][1] * srcB.m[1][2]) +
				(srcA.m[i][2] * srcB.m[2][2]);
		}
		memcpy(&result, &tmp, sizeof(Matrix33F));
	}

	static void GetTransformMatrix44F(
		int imageWidth, int imageHeight, Matrix33F transformMatrix33F, Matrix44F & transformMatrix44F)
	{
		Matrix33F convertGLToImage;
		convertGLToImage.m[0][2] = imageWidth / 2.0f;
		convertGLToImage.m[1][1] = -1.0f;
		convertGLToImage.m[1][2] = imageHeight / 2.0f;

		Matrix33F convertImageToGL;
		convertImageToGL.m[0][2] = -imageWidth / 2.0f;
		convertImageToGL.m[1][1] = -1.0f;
		convertImageToGL.m[1][2] = imageHeight / 2.0f;

		Matrix33F fullTransform3x3;
		fullTransform3x3 = convertImageToGL * transformMatrix33F * convertGLToImage;

		transformMatrix44F.m[0][0] = fullTransform3x3.m[0][0] / fullTransform3x3.m[2][2];
		transformMatrix44F.m[0][1] = fullTransform3x3.m[0][1] / fullTransform3x3.m[2][2];
		transformMatrix44F.m[0][2] = 0.0f;
		transformMatrix44F.m[0][3] = fullTransform3x3.m[0][2] / fullTransform3x3.m[2][2];

		transformMatrix44F.m[1][0] = fullTransform3x3.m[1][0] / fullTransform3x3.m[2][2];
		transformMatrix44F.m[1][1] = fullTransform3x3.m[1][1] / fullTransform3x3.m[2][2];
		transformMatrix44F.m[1][2] = 0.0f;
		transformMatrix44F.m[1][3] = fullTransform3x3.m[1][2] / fullTransform3x3.m[2][2];

		transformMatrix44F.m[2][0] = 0.0f;
		transformMatrix44F.m[2][1] = 0.0f;
		transformMatrix44F.m[2][2] = 1.0f;
		transformMatrix44F.m[2][3] = 0.0f;

		transformMatrix44F.m[3][0] = fullTransform3x3.m[2][0] / fullTransform3x3.m[2][2];
		transformMatrix44F.m[3][1] = fullTransform3x3.m[2][1] / fullTransform3x3.m[2][2];
		transformMatrix44F.m[3][2] = 0.0f;
		transformMatrix44F.m[3][3] = fullTransform3x3.m[2][2] / fullTransform3x3.m[2][2];
	}

	static void PerspectivePortraitUp(Matrix44F &result, int cameraWidth, int cameraHeight, int screenWidth, int screenHeight, float nearClipPlane, float farClipPlane)
	{
		float w = (float)cameraWidth;
		float h = (float)cameraWidth * screenWidth / screenHeight;
		float sr = (float)screenHeight / screenWidth;
		float cr = (float)cameraWidth / cameraHeight;

		float x = 0.0f;
		float y = 0.0f;

		if (sr < cr)
		{
			x = h / w * cr;
			y = cr;
		}
		else
		{
			x = 1;
			y = w / h;
		}

		result.m[0][0] = 0.0f;
		result.m[0][1] = 2.0f * -x;
		result.m[0][2] = 0.0f;
		result.m[0][3] = 0.0f;

		result.m[1][0] = 2.0f * -y;
		result.m[1][1] = 0.0f;
		result.m[1][2] = 0.0f;
		result.m[1][3] = 0.0f;

		result.m[2][0] = 0.0f;
		result.m[2][1] = 0.0f;
		result.m[2][2] = (farClipPlane + nearClipPlane) / (farClipPlane - nearClipPlane);
		result.m[2][3] = 1.0f;

		result.m[3][0] = 0.0f;
		result.m[3][1] = 0.0f;
		result.m[3][2] = -(2.0f * farClipPlane * nearClipPlane) / (farClipPlane - nearClipPlane);
		result.m[3][3] = 0.0f;
	}

	static void PerspectivePortraitDown(Matrix44F &result, int cameraWidth, int cameraHeight, int screenWidth, int screenHeight, float nearClipPlane, float farClipPlane)
	{
		float w = (float)cameraWidth;
		float h = (float)cameraWidth * screenWidth / screenHeight;
		float sr = (float)screenHeight / screenWidth;
		float cr = (float)cameraWidth / cameraHeight;

		float x = 0.0f;
		float y = 0.0f;

		if (sr < cr)
		{
			x = h / w * cr;
			y = cr;
		}
		else
		{
			x = 1;
			y = w / h;
		}

		result.m[0][0] = 0.0f;
		result.m[0][1] = 2.0f * x;
		result.m[0][2] = 0.0f;
		result.m[0][3] = 0.0f;

		result.m[1][0] = 2.0f * y;
		result.m[1][1] = 0.0f;
		result.m[1][2] = 0.0f;
		result.m[1][3] = 0.0f;

		result.m[2][0] = 0.0f;
		result.m[2][1] = 0.0f;
		result.m[2][2] = (farClipPlane + nearClipPlane) / (farClipPlane - nearClipPlane);
		result.m[2][3] = 1.0f;

		result.m[3][0] = 0.0f;
		result.m[3][1] = 0.0f;
		result.m[3][2] = -(2.0f * farClipPlane * nearClipPlane) / (farClipPlane - nearClipPlane);
		result.m[3][3] = 0.0f;
	}

	static void PerspectiveLandscapeLeft(Matrix44F &result, int cameraWidth, int cameraHeight, int screenWidth, int screenHeight, float nearClipPlane, float farClipPlane)
	{
		float w = (float)cameraWidth;
		float h = (float)cameraWidth * screenHeight / screenWidth;
		float sr = (float)screenWidth / screenHeight;
		float cr = (float)cameraWidth / cameraHeight;

		float x = 0.0f;
		float y = 0.0f;

		if (sr < cr)
		{
			x = h / w * cr;
			y = cr;
		}
		else
		{
			x = 1;
			y = w / h;
		}

		result.m[0][0] = 2.0f * x;
		result.m[0][1] = 0.0f;
		result.m[0][2] = 0.0f;
		result.m[0][3] = 0.0f;

		result.m[1][0] = 0.0f;
		result.m[1][1] = 2.0f * -y;
		result.m[1][2] = 0.0f;
		result.m[1][3] = 0.0f;

		result.m[2][0] = 0.0f;
		result.m[2][1] = 0.0f;
		result.m[2][2] = (farClipPlane + nearClipPlane) / (farClipPlane - nearClipPlane);
		result.m[2][3] = 1.0f;

		result.m[3][0] = 0.0f;
		result.m[3][1] = 0.0f;
		result.m[3][2] = -(2.0f * farClipPlane * nearClipPlane) / (farClipPlane - nearClipPlane);
		result.m[3][3] = 0.0f;
	}

	static void PerspectiveLandscapeRight(Matrix44F &result, int cameraWidth, int cameraHeight, int screenWidth, int screenHeight, float nearClipPlane, float farClipPlane)
	{
		float w = (float)cameraWidth;
		float h = (float)cameraWidth * screenHeight / screenWidth;
		float sr = (float)screenWidth / screenHeight;
		float cr = (float)cameraWidth / cameraHeight;

		float x = 0.0f;
		float y = 0.0f;

		if (sr < cr)
		{
			x = h / w * cr;
			y = cr;
		}
		else
		{
			x = 1;
			y = w / h;
		}

		result.m[0][0] = 2.0f * -x;
		result.m[0][1] = 0.0f;
		result.m[0][2] = 0.0f;
		result.m[0][3] = 0.0f;

		result.m[1][0] = 0.0f;
		result.m[1][1] = 2.0f * y;
		result.m[1][2] = 0.0f;
		result.m[1][3] = 0.0f;

		result.m[2][0] = 0.0f;
		result.m[2][1] = 0.0f;
		result.m[2][2] = (farClipPlane + nearClipPlane) / (farClipPlane - nearClipPlane);
		result.m[2][3] = 1.0f;

		result.m[3][0] = 0.0f;
		result.m[3][1] = 0.0f;
		result.m[3][2] = -(2.0f * farClipPlane * nearClipPlane) / (farClipPlane - nearClipPlane);
		result.m[3][3] = 0.0f;
	}
};