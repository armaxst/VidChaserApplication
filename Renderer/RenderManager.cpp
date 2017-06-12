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
			delete *itor;
		}

		stickerList.clear();
	}

	void RenderManager::updateRendering(int width, int height)
	{

	}

	void RenderManager::draw(long timeStamp)
	{
        if(traceIdx == -1)
        {
            drawInitFrame(timeStamp);
        }
        else
        {
            list<Sticker *>::iterator itor = stickerList.begin();
            list<Sticker *>::iterator temp;
            
            for(int i = 0; i < traceIdx; i++)
            {
                itor++;
            }
            
            temp = itor;
            temp++;
            
            if(temp == stickerList.end())
            {
                if(timeStamp > (*itor)->getTimeStamp())
                {
                    (*itor)->draw();
                    return;
                }
                
                traceIdx = -1;
                drawTimestamp = -1;
                drawInitFrame(timeStamp);
                return;
            }
            
            if(timeStamp < (*temp)->getTimeStamp())
            {
                if(drawTimestamp == -1)
                {
                    return;
                }
                
                if((timeStamp - drawTimestamp) > ((*temp)->getTimeStamp() - timeStamp))
                {
                    itor = temp;
                    drawTimestamp = (*itor)->getTimeStamp();
                    traceIdx++;
                }
            }
            else
            {
                itor = temp;
                drawTimestamp = (*itor)->getTimeStamp();
                traceIdx++;
            }
            drawInternal();
        }
    }
    
    void RenderManager::drawInternal()
    {
        for (list<Sticker *>::iterator itor = stickerList.begin();
             itor != stickerList.end();
             itor++)
        {
            (*itor)->draw();
        }
    }
    
    void RenderManager::drawInitFrame(long timeStamp)
    {
        list<Sticker *>::iterator itor = stickerList.begin();
        if(itor == stickerList.end())
        {
            return;
        }
        
        traceIdx = 0;
        if(timeStamp < (*itor)->getTimeStamp())
        {
            return;
        }
        while(1)
        {
            itor++;
            if(itor == stickerList.end())
                break;
            if(timeStamp <= (*itor)->getTimeStamp())
                break;
            traceIdx++;
        }
        
        itor--;
        drawTimestamp = (*itor)->getTimeStamp();
        drawInternal();
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

    void RenderManager::setTransformMatrix(int idx, long timeStamp, Matrix44F &matrix)
    {
        list<Sticker *>::iterator itor = stickerList.begin();
        for(int i = 0; i < idx; i++)
        {
            itor++;
        }
        (*itor)->applyTransform(matrix);
        (*itor)->setTimeStamp(timeStamp);
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
