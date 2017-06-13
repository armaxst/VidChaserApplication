/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#include "RenderManager.h"

namespace Renderer
{
	shared_ptr<RenderManager> RenderManager::instance = nullptr;

	RenderManager::RenderManager()
	{
        traceIdx = -1;
        drawTimestamp = -1;
	}

	RenderManager::~RenderManager()
	{
		for (list<Sticker *>::iterator itor = stickerList.begin();
			itor != stickerList.end();
			itor++)
		{
			delete *itor;
		}

		stickerList.clear();
	}

	void RenderManager::initRendering()
	{
		for (list<Sticker *>::iterator itor = stickerList.begin();
			itor != stickerList.end();
			itor++)
		{
			(*itor)->init();
		}
	}

	void RenderManager::updateRendering(int width, int height)
	{

	}

	void RenderManager::draw(int imageIndex)
	{
        for (list<Sticker *>::iterator itor = stickerList.begin();
            itor != stickerList.end();
            itor++)
        {
            (*itor)->draw(imageIndex);
        }
    }
    
	void RenderManager::deinit()
	{
		for (list<Sticker *>::iterator itor = stickerList.begin();
			itor != stickerList.end();
			itor++)
		{
			delete *itor;
		}

		stickerList.clear();
	}

	Sticker * RenderManager::createSticker()
	{
		Sticker * newSticker = new Sticker();
		return newSticker;
	}
    
    void RenderManager::addSticker(Sticker *sticker)
    {
        stickerList.push_back(sticker);
    }
    
    void RenderManager::removeSticker(Sticker *sticker)
    {
        stickerList.remove(sticker);
    }

	Sticker * RenderManager::getTouchedSticker(int touchX, int touchY)
	{
		list<Sticker *>::iterator itor = stickerList.begin();
		for (list<Sticker *>::iterator itor = stickerList.begin();
			itor != stickerList.end();
			itor++)
		{
			if ((*itor)->isTouched(touchX, touchY))
			{
				return *itor;
			}
		}

		return nullptr;
	}
    
	shared_ptr<RenderManager> RenderManager::getInstance()
	{
		if (instance == nullptr)
		{
			instance = shared_ptr<RenderManager>(new RenderManager());
		}

		return instance;
	}
}
