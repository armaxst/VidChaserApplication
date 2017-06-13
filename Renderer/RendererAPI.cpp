/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#include "RendererAPI.h"
#include "RenderManager.h"

namespace Renderer
{
	void initRendering()
	{
		RenderManager::getInstance()->initRendering();
	}

	void updateRendering(int width, int height)
	{
		RenderManager::getInstance()->updateRendering(width, height);
	}

	void draw(int imageIndex)
	{
		RenderManager::getInstance()->draw(imageIndex);
	}

	void deinit()
	{
		RenderManager::getInstance()->deinit();
	}

	Sticker * createSticker()
	{
		return RenderManager::getInstance()->createSticker();
	}
    
    void addSticker(Sticker* sticker)
    {
        RenderManager::getInstance()->addSticker(sticker);
    }
    
    void removeSticker(Sticker *sticker)
    {
        RenderManager::getInstance()->removeSticker(sticker);
    }
    
	Sticker * getTouchedSticker(int touchX, int touchY)
	{
		return RenderManager::getInstance()->getTouchedSticker(touchX, touchY);
	}
}
