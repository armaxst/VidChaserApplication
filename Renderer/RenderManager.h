/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma once

#include <memory>
#include <list>
#include "Sticker.h"

using namespace std;

namespace Renderer
{
	class RenderManager
	{
	public:
		static shared_ptr<RenderManager> getInstance();

		RenderManager();
		~RenderManager();

		void initRendering();
		void updateRendering(int width, int height);
		void draw(long timeStamp);
		void deinit();
		Sticker * createSticker();
        void addSticker(Sticker * sticker);
        void setTransformMatrix(int idx, long timeStamp, Matrix44F &matrix);
	private:
        void drawInitFrame(long timeStamp);
        void drawInternal();
        
		static shared_ptr<RenderManager> instance;
		list<Sticker *> stickerList;
        int traceIdx;
        long drawTimestamp;
	};
}
