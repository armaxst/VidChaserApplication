#pragma once

namespace VidChaser
{
	enum ColorFormat
	{
		RGBA8888 = 0,
		RGB888 = 1,
		YUV420sp = 2,
		YUV420 = 3,
		YUV420_888 = 4,
		GRAY8 = 5
	};

	enum TrackingMethod
	{
		AFFINE = 0,
		HOMOGRAPHY = 1,
		RIGID = 2,
		TRANSLATION = 3
	};
}