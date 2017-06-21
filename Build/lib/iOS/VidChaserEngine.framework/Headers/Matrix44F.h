/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma  once

#include <math.h>
#include <string.h>

#define PI 3.1415926535897932384626433832795f

class Matrix44F
{
public:
	float m[4][4];

	Matrix44F()
	{
		memset((float *)&m[0][0], 0x0, sizeof(float) * 16);
		m[0][0] = 1.0f;
		m[1][1] = 1.0f;
		m[2][2] = 1.0f;
		m[3][3] = 1.0f;
	}

	void SetIdentity()
	{
		memset((float *)&m[0][0], 0x0, sizeof(float) * 16);
		m[0][0] = 1.0f;
		m[1][1] = 1.0f;
		m[2][2] = 1.0f;
		m[3][3] = 1.0f;
	}

	void Rotate(float angle, float x, float y, float z)
	{
		float sinAngle, cosAngle;
		float mag = (float)sqrtf(x * x + y * y + z * z);

		sinAngle = sinf(angle * PI / 180.0f);
		cosAngle = cosf(angle * PI / 180.0f);

		if (mag > 0.0f) {
			float xx, yy, zz, xy, yz, zx, xs, ys, zs;
			float oneMinusCos;
			Matrix44F rotMat;

			x /= mag;
			y /= mag;
			z /= mag;

			xx = x * x;
			yy = y * y;
			zz = z * z;
			xy = x * y;
			yz = y * z;
			zx = z * x;
			xs = x * sinAngle;
			ys = y * sinAngle;
			zs = z * sinAngle;

			oneMinusCos = 1.0f - cosAngle;
			rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
			rotMat.m[0][1] = (oneMinusCos * xy) - zs;
			rotMat.m[0][2] = (oneMinusCos * zx) + ys;
			rotMat.m[0][3] = 0.0F;
			rotMat.m[1][0] = (oneMinusCos * xy) + zs;
			rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
			rotMat.m[1][2] = (oneMinusCos * yz) - xs;
			rotMat.m[1][3] = 0.0F;

			rotMat.m[2][0] = (oneMinusCos * zx) - ys;
			rotMat.m[2][1] = (oneMinusCos * yz) + xs;
			rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
			rotMat.m[2][3] = 0.0F;

			rotMat.m[3][0] = 0.0F;
			rotMat.m[3][1] = 0.0F;
			rotMat.m[3][2] = 0.0F;
			rotMat.m[3][3] = 1.0F;

			Multiply(rotMat);
		}
	}

	void RotateEuler(float x, float y, float z)
	{
		x *= PI / 180.0f;
		y *= PI / 180.0f;
		z *= PI / 180.0f;
		float cx = cosf(x);
		float sx = sinf(x);
		float cy = cosf(y);
		float sy = sinf(y);
		float cz = cosf(z);
		float sz = sinf(z);
		float cxsy = cx * sy;
		float sxsy = sx * sy;

		Matrix44F rotMat;

		rotMat.m[0][0] = cy * cz;
		rotMat.m[0][1] = -cy * sz;
		rotMat.m[0][2] = sy;
		rotMat.m[0][3] = 0.0f;

		rotMat.m[1][0] = cxsy * cz + cx * sz;
		rotMat.m[1][1] = -cxsy * sz + cx * cz;
		rotMat.m[1][2] = -sx * cy;
		rotMat.m[1][3] = 0.0f;

		rotMat.m[2][0] = -sxsy * cz + sx * sz;
		rotMat.m[2][1] = sxsy * sz + sx * cz;
		rotMat.m[2][2] = cx * cy;
		rotMat.m[2][3] = 0.0f;

		rotMat.m[3][0] = 0.0f;
		rotMat.m[3][1] = 0.0f;
		rotMat.m[3][2] = 0.0f;
		rotMat.m[3][3] = 1.0f;

		Multiply(rotMat);
	}

	void Scale(float sx, float sy, float sz)
	{
		m[0][0] *= sx;
		m[0][1] *= sx;
		m[0][2] *= sx;
		m[0][3] *= sx;

		m[1][0] *= sy;
		m[1][1] *= sy;
		m[1][2] *= sy;
		m[1][3] *= sy;

		m[2][0] *= sz;
		m[2][1] *= sz;
		m[2][2] *= sz;
		m[2][3] *= sz;
	}

	void Translate(float sx, float sy, float sz)
	{
		Matrix44F transMat;
		transMat.m[0][0] = 1.0f;
		transMat.m[0][1] = 0.0f;
		transMat.m[0][2] = 0.0f;
		transMat.m[0][3] = sx;

		transMat.m[1][0] = 0.0f;
		transMat.m[1][1] = 1.0f;
		transMat.m[1][2] = 0.0f;
		transMat.m[1][3] = sy;

		transMat.m[2][0] = 0.0f;
		transMat.m[2][1] = 0.0f;
		transMat.m[2][2] = 1.0f;
		transMat.m[2][3] = sz;

		transMat.m[3][0] = 0.0f;
		transMat.m[3][1] = 0.0f;
		transMat.m[3][2] = 0.0f;
		transMat.m[3][3] = 1.0f;

		Multiply(transMat);
	}

	void Multiply(Matrix44F & matrix)
	{
		Matrix44F tmp;
		for (int i = 0; i < 4; i++)
		{
			tmp.m[i][0] =
				(matrix.m[i][0] * this->m[0][0]) +
				(matrix.m[i][1] * this->m[1][0]) +
				(matrix.m[i][2] * this->m[2][0]) +
				(matrix.m[i][3] * this->m[3][0]);

			tmp.m[i][1] =
				(matrix.m[i][0] * this->m[0][1]) +
				(matrix.m[i][1] * this->m[1][1]) +
				(matrix.m[i][2] * this->m[2][1]) +
				(matrix.m[i][3] * this->m[3][1]);

			tmp.m[i][2] =
				(matrix.m[i][0] * this->m[0][2]) +
				(matrix.m[i][1] * this->m[1][2]) +
				(matrix.m[i][2] * this->m[2][2]) +
				(matrix.m[i][3] * this->m[3][2]);

			tmp.m[i][3] =
				(matrix.m[i][0] * this->m[0][3]) +
				(matrix.m[i][1] * this->m[1][3]) +
				(matrix.m[i][2] * this->m[2][3]) +
				(matrix.m[i][3] * this->m[3][3]);
		}

		memcpy(this, &tmp, sizeof(Matrix44F));
	}

	Matrix44F operator*(const Matrix44F& other)
	{
		Matrix44F tmp;
		for (int i = 0; i < 4; i++)
		{
			tmp.m[i][0] =
				(this->m[i][0] * other.m[0][0]) +
				(this->m[i][1] * other.m[1][0]) +
				(this->m[i][2] * other.m[2][0]) +
				(this->m[i][3] * other.m[3][0]);

			tmp.m[i][1] =
				(this->m[i][0] * other.m[0][1]) +
				(this->m[i][1] * other.m[1][1]) +
				(this->m[i][2] * other.m[2][1]) +
				(this->m[i][3] * other.m[3][1]);

			tmp.m[i][2] =
				(this->m[i][0] * other.m[0][2]) +
				(this->m[i][1] * other.m[1][2]) +
				(this->m[i][2] * other.m[2][2]) +
				(this->m[i][3] * other.m[3][2]);

			tmp.m[i][3] =
				(this->m[i][0] * other.m[0][3]) +
				(this->m[i][1] * other.m[1][3]) +
				(this->m[i][2] * other.m[2][3]) +
				(this->m[i][3] * other.m[3][3]);
		}

		return tmp;
	}

	Matrix44F Transpose()
	{
		Matrix44F tmp;
		for (int i = 0; i < 4; i++)
		{
			tmp.m[i][0] = this->m[0][i];
			tmp.m[i][1] = this->m[1][i];
			tmp.m[i][2] = this->m[2][i];
			tmp.m[i][3] = this->m[3][i];
		}

		return tmp;
	}
};