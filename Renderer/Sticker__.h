#ifndef __STICKER_H__
#define __STICKER_H__

#include "_internal_base.h"
#include <vector>
#include <map>
#include <memory>
#include "ShaderUtil.h"

using namespace std;

static const int RGB_TEXTURE_COUNT = 1;

#define	QUAD_IDX_BUFF_LENGTH 6
#define	QUAD_TEXCOORD_BUFF_LENGTH 8

typedef std::shared_ptr<unsigned char> UCharPtr;

class Sticker
{
public:
	Sticker(int32_t id);
	virtual ~Sticker();

public:
	int32_t setStickerInfo(int32_t x, int32_t y, unsigned char *img, int32_t w, int32_t h);
	void	pushTraceInfo(int64_t timestamp, float *mvpMatrix4x4);

	void initShader();
	int32_t draw(int64_t timestamp);
	//void	drawReset();

private:
	void drawInternal(float *mvpMatrix);
	int32_t drawInitFrame(int64_t timestamp);

//private:
//	struct TraceInfo
//	{
//		int64_t timestamp;
//		float	matrix[16];
//	};

private:
	int32_t		stickerId;
	int32_t		pointX;
	int32_t		pointY;
	int32_t		imageWidth;
	int32_t		imageHeight;

	int64_t		drawTimestamp;

	UCharPtr		imageData;		// think...

	//vector<TraceInfo*>	traceInfos;
	map<int64_t, float*> traceInfos;

	//int32_t		traceIdx;

	float frameWidth;
	float frameHeight;

	GLuint glProgram;
	GLuint positionHandle;
	GLuint textureHandle[RGB_TEXTURE_COUNT];
	GLuint textureCoordHandle;
	GLuint mvpMatrixHandle;

	float * vertexBuff;
	unsigned char *indexBuff;
	float *textureCoordBuff;

	GLuint textureNames[RGB_TEXTURE_COUNT];

private:
	DISALLOW_EVIL_CONSTRUCTORS(Sticker);
};
#endif //__STICKER_H__