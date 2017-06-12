/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/

#pragma once

class CoordiCvtUtil
{
public:
	static void GetImageCoordiFromScreenCoordi(int screenWidth, int screenHeight, int imageWidth, int imageHeight,
		int screenX, int screenY, int & imageX, int & imageY) {
		float widthRatio = (float)screenWidth / imageWidth;
		float heightRatio = (float)screenHeight / imageHeight;

		// When image's upper and lower part cropped.
		float ratio = widthRatio;

		// When image's left and right part cropped.
		if (widthRatio < heightRatio) {
			ratio = heightRatio;
		}

		imageX = (screenX - screenWidth / 2) / ratio + imageWidth / 2;
		imageY = -(screenHeight / 2 - screenY) / ratio + imageHeight / 2;
	}

	static void GetGLCoordiFromScreenCoordi(
		int screenWidth, int screenHeight, int imageWidth, int imageHeight,
		int screenX, int screenY, float & glX, float & glY) {
		float widthRatio = (float)screenWidth / imageWidth;
		float heightRatio = (float)screenHeight / imageHeight;

		int screenCenterX = screenWidth / 2;
		int screenCenterY = screenHeight / 2;

		float ratio = widthRatio;

		glX = (float)screenX - screenCenterX;
		glY = (float)screenCenterY - screenY;

		// When image's left and right part cropped.
		if (widthRatio < heightRatio) {
			ratio = heightRatio;
		}

		glX = ((float)screenX - screenCenterX) / ratio;
		glY = ((float)screenCenterY - screenY) / ratio;
	}
};