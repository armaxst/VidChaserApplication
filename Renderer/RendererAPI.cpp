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

	void draw(long timeStamp)
	{
		RenderManager::getInstance()->draw(timeStamp);
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
    
    void setTransformMatrix(int idx, long timeStamp, Matrix44F & matrix)
    {
        RenderManager::getInstance()->setTransformMatrix(idx, timeStamp, matrix);
    }
}
