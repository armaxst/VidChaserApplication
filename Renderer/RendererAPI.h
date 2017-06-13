/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma once

#include "Sticker.h"
#include <Matrix44F.h>

namespace Renderer
{
	void initRendering();

	void updateRendering(int width, int height);

	void draw(int imageIndex);

	void deinit();

	/**
	* @brief Create quad renderer with size 1 (width 1, height 1)
	*/
	Sticker * createSticker();
    
    void addSticker(Sticker *sticker);
    
    void removeSticker(Sticker *sticker);
    
	Sticker * getTouchedSticker(int touchX, int touchY);
}
