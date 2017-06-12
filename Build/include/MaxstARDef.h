#pragma once

typedef unsigned char Byte;

enum ColorFormat
{
	RGBA8888 = 0,
	RGB888 = 1,
	YUV420sp = 2,
	YUV420 = 3,
	YUV420_888 = 4,
	GRAY8 = 5
};

enum ScreenOrientation // unity screen orientation order
{
	UNKNOWN = 0,
	PORTRAIT = 1,
	PORTRAIT_UP = 1,
	PORTRAIT_DOWN = 2,
	LANDSCAPE = 3,
	LANDSCAPE_LEFT = 3,
	LANDSCAPE_RIGHT = 4,
};