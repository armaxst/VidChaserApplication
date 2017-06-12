#define LOG_NDEBUG 0
#define LOG_TAG "Sticker"

#include "Logger.h"
#include <string.h>
#include <stdint.h>
#include "Sticker.h"

static const char stickerVertexShader[] =
"attribute vec4 vertexPosition;\n"
"attribute vec2 vertexTexCoord;\n"
"varying vec2 texCoord;\n"
"uniform mat4 modelViewProjectionMatrix;\n"
"void main()							\n"
"{										\n"
"	gl_Position = modelViewProjectionMatrix * vertexPosition;\n"
"	texCoord = vertexTexCoord; 			\n"
"}										\n";

static const char stickerFragmentShader[] =
"precision mediump float;\n"
"varying vec2 texCoord;\n"
"uniform sampler2D s_texture_1;\n"

"void main(void)\n"
"{\n"
"	gl_FragColor = texture2D(s_texture_1, texCoord);\n"
"}\n";

float stickerVertices[] = {
	-0.5f, 0.5f, 0.0f,   // top left
	-0.5f, -0.5f, 0.0f,   // bottom left
	0.5f, -0.5f, 0.0f,   // bottom right
	0.5f, 0.5f, 0.0f  // top right
};

unsigned char stickerIndices[] = {
	0, 1, 2, 2, 3, 0
};

float stickerTextureCoords[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
};

Sticker::Sticker(int32_t id)
	:stickerId(id),
	pointX(0),
	pointY(0),
	imageWidth(0),
	imageHeight(0),
	drawTimestamp(-1ll),
	imageData(NULL)
	//traceIdx(-1)
{
	glProgram = 0;
	positionHandle = 0;
	textureCoordHandle = 0;
	mvpMatrixHandle = 0;
	for (int i = 0; i < RGB_TEXTURE_COUNT; i++)
	{
		textureHandle[i] = 0;
	}

	vertexBuff = stickerVertices;
	indexBuff = stickerIndices;
	textureCoordBuff = stickerTextureCoords;
}

Sticker::~Sticker()
{
	if (imageData)
		imageData.reset();
	//delete [] imageData;

	/*vector<TraceInfo*>::iterator tmpit = traceInfos.begin();

	while (tmpit != traceInfos.end())
	{
		TraceInfo *tmpinfo = (*tmpit);
		delete tmpinfo;

		tmpit++;
	}

	traceInfos.clear();*/

	map<int64_t, float*>::iterator iter;
	for (iter = traceInfos.begin(); iter != traceInfos.end(); ++iter)
	{
		delete[] iter->second;
	}
	traceInfos.clear();
}

int32_t Sticker::setStickerInfo(int32_t x, int32_t y, unsigned char *img, int32_t w, int32_t h)
{
	int32_t length = w * h * 4;
	if (imageData == UCharPtr())
		imageData = UCharPtr(new unsigned char[length]);

	memcpy(imageData.get(), img, length);

	imageWidth = w;
	imageHeight = h;

	pointX = x;
	pointY = y;
	return 0;
}

void Sticker::pushTraceInfo(int64_t timestamp, float *mvpMatrix4x4)
{
	/*TraceInfo *tmpInfo = new TraceInfo;
	tmpInfo->timestamp = timestamp;
	memcpy(tmpInfo->matrix, mvpMatrix4x4, sizeof(tmpInfo->matrix));

	traceInfos.push_back(tmpInfo);*/

	float* matrix = new float[16];
	memcpy(matrix, mvpMatrix4x4, sizeof(float[16]));
	traceInfos[timestamp] = matrix; //traceInfos.insert(map<int64_t, float*>::value_type(timestamp, matrix));
}

void Sticker::initShader()
{
	glProgram = ShaderUtil::createProgram(stickerVertexShader, stickerFragmentShader);
	ShaderUtil::checkGlError("createProgram");

	positionHandle = glGetAttribLocation(glProgram, "vertexPosition");
	ShaderUtil::checkGlError("vertexPosition");

	textureCoordHandle = glGetAttribLocation(glProgram, "vertexTexCoord");
	ShaderUtil::checkGlError("vertexTexCoord");

	mvpMatrixHandle = glGetUniformLocation(glProgram, "modelViewProjectionMatrix");
	ShaderUtil::checkGlError("modelViewProjectionMatrix");

	glGenTextures(RGB_TEXTURE_COUNT, textureNames);
	for (int i = 0; i < RGB_TEXTURE_COUNT; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textureNames[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		char tempString[32];
		sprintf(tempString, "s_texture_%d", i + 1);
		textureHandle[i] = glGetUniformLocation(glProgram, tempString);
		ShaderUtil::checkGlError("glGetUniformLocation");
	}
}

int32_t Sticker::draw(int64_t timestamp)
{
	if (!glProgram)
	{
		initShader();
	}

	//if (traceIdx == -1)
	//{
	//	drawInitFrame(timestamp);
	//}
	//else
	//{
	//	vector<TraceInfo*>::iterator it = traceInfos.begin();
	//	vector<TraceInfo*>::iterator tmpit;
	//	it += traceIdx;

	//	tmpit = it;
	//	tmpit++;

	//	if (tmpit == traceInfos.end())
	//	{
	//		if (timestamp > (*it)->timestamp)
	//		{
	//			//				LOGV("Sticker::draw draw end  stickerId %d frameTs %lld idx %d _drawTs %lld", stickerId, timestamp, traceIdx, drawTimestamp);
	//			drawInternal((*it)->matrix);
	//			return 0;
	//		}

	//		// rollover
	//		traceIdx = -1;
	//		drawTimestamp = -1ll;
	//		drawInitFrame(timestamp);
	//		return 0;	// skip draw
	//	}

	//	if (timestamp < (*tmpit)->timestamp)
	//	{
	//		if (drawTimestamp == -1ll)
	//		{
	//			// skip draw
	//			//				LOGV("Sticker::draw skip !end stickerId %d frameTs %lld idx %d _drawTs %lld", stickerId, timestamp, traceIdx, drawTimestamp);
	//			return 0;
	//		}

	//		if ((timestamp - drawTimestamp) > ((*tmpit)->timestamp - timestamp))
	//		{
	//			it = tmpit;
	//			drawTimestamp = (*it)->timestamp;
	//			traceIdx++;
	//		}
	//	}
	//	else
	//	{
	//		it = tmpit;
	//		drawTimestamp = (*it)->timestamp;
	//		traceIdx++;
	//	}

	//	//		LOGV("Sticker::draw draw stickerId %d frameTs %lld idx %d _drawTs %lld", stickerId, timestamp, traceIdx, drawTimestamp);
	//	drawInternal((*it)->matrix);
	//}
	
	//if not have matrix on timestamp, then find near timestamp value and draw it.
	if (traceInfos.size() == 0)
	{
		return 1;
	}

	if (!(traceInfos.count(timestamp)))
	{
		map<int64_t, float*>::iterator lowerIter = traceInfos.lower_bound(timestamp);

		if (!(traceInfos.count(lowerIter->first))) 
		{
			map<int64_t, float*>::iterator lastIter = traceInfos.end();
			lastIter--;
			drawInternal(lastIter->second);
		}
		else if (lowerIter->first > timestamp) //if lowerIter value is bigger than timestamp, then lowerIter is left value 
		{
			map<int64_t, float*>::iterator leftIter = lowerIter;
			leftIter--; //get left value

			if ((timestamp - leftIter->first) < (lowerIter->first - timestamp))
			{
				drawInternal(leftIter->second);
			}
			else
			{
				drawInternal(lowerIter->second);
			}
		}
		else  //if lowerIter value is smaller than timestamp, then lowerIter is right value 
		{
			map<int64_t, float*>::iterator rightIter = lowerIter;
			rightIter++;

			if ((timestamp - lowerIter->first) < (rightIter->first - timestamp))
			{
				drawInternal(lowerIter->second);
			}
			else
			{
				drawInternal(rightIter->second);
			}
		}
	}
	else
	{
		drawInternal(traceInfos.find(timestamp)->second);
	}
	return 0;
}

//void Sticker::drawReset()
//{
//	drawTimestamp = -1ll;
//	traceIdx = -1;
//}

void Sticker::drawInternal(float *mvpMatrix)
{
	glUseProgram(glProgram);
	ShaderUtil::checkGlError("glUseProgram");

	glEnableVertexAttribArray(positionHandle);
	glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(float), vertexBuff);
	ShaderUtil::checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(textureCoordHandle);
	glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, GL_FALSE, 
		2 * sizeof(float), textureCoordBuff);
	ShaderUtil::checkGlError("glVertexAttribPointer 2");

	glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, mvpMatrix);

	ShaderUtil::checkGlError("glUniformMatrix4fv");

	if (imageData)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(textureHandle[0], 0);
		glBindTexture(GL_TEXTURE_2D, textureNames[0]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, imageData.get());

		glDrawElements(GL_TRIANGLES, QUAD_IDX_BUFF_LENGTH,
			GL_UNSIGNED_BYTE, indexBuff);

		glDisableVertexAttribArray(positionHandle);
		glDisableVertexAttribArray(textureCoordHandle);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glUseProgram(0);
}

int32_t Sticker::drawInitFrame(int64_t timestamp)
{
	//vector<TraceInfo*>::iterator it = traceInfos.begin();
	//if (it == traceInfos.end())
	//{
	//	//		LOGV("Sticker::draw skip empty stickerId %d infos frameTs %lld", stickerId, timestamp);
	//	return 0;	// skip draw
	//}

	//traceIdx = 0;

	//if (timestamp < (*it)->timestamp)
	//{
	//	//		LOGV("Sticker::draw skip stickerId %d frameTs %lld idx %d _drawTs %lld", stickerId, timestamp, traceIdx, drawTimestamp);
	//	return 0; // skip draw
	//}

	//while (1)
	//{
	//	it++;
	//	if (it == traceInfos.end())
	//		break;
	//	if (timestamp <= (*it)->timestamp)
	//		break;
	//	traceIdx++;
	//}

	//it--;
	//drawTimestamp = (*it)->timestamp;

	////	LOGV("Sticker::draw draw frameTs %lld idx %d _drawTs %lld", timestamp, traceIdx, drawTimestamp);
	//drawInternal((*it)->matrix);

	return 0;
}