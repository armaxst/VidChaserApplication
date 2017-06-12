/*
* Copyright 2017 Maxst, Inc. All Rights Reserved.
*/
#pragma  once

#include <math.h>
#include <string.h>

class Matrix33F
{
public:
	float m[3][3];

	Matrix33F()
	{
		memset((float *)&m[0][0], 0x0, sizeof(float) * 9);
		m[0][0] = 1.0f;
		m[1][1] = 1.0f;
		m[2][2] = 1.0f;
	}

	void SetIdentity()
	{
		memset((float *)&m[0][0], 0x0, sizeof(float) * 9);
		m[0][0] = 1.0f;
		m[1][1] = 1.0f;
		m[2][2] = 1.0f;
	}

	void Multiply(Matrix33F & matrix)
	{
		Matrix33F tmp;
		for (int i = 0; i < 3; i++)
		{
			tmp.m[i][0] =
				(matrix.m[i][0] * this->m[0][0]) +
				(matrix.m[i][1] * this->m[1][0]) +
				(matrix.m[i][2] * this->m[2][0]);

			tmp.m[i][1] =
				(matrix.m[i][0] * this->m[0][1]) +
				(matrix.m[i][1] * this->m[1][1]) +
				(matrix.m[i][2] * this->m[2][1]);

			tmp.m[i][2] =
				(matrix.m[i][0] * this->m[0][2]) +
				(matrix.m[i][1] * this->m[1][2]) +
				(matrix.m[i][2] * this->m[2][2]);
		}
		memcpy(this, &tmp, sizeof(Matrix33F));
	}

	Matrix33F operator*(const Matrix33F& other)
	{
		Matrix33F tmp;
		for (int i = 0; i < 3; i++)
		{
			tmp.m[i][0] =
				(this->m[i][0] * other.m[0][0]) +
				(this->m[i][1] * other.m[1][0]) +
				(this->m[i][2] * other.m[2][0]);

			tmp.m[i][1] =
				(this->m[i][0] * other.m[0][1]) +
				(this->m[i][1] * other.m[1][1]) +
				(this->m[i][2] * other.m[2][1]);

			tmp.m[i][2] =
				(this->m[i][0] * other.m[0][2]) +
				(this->m[i][1] * other.m[1][2]) +
				(this->m[i][2] * other.m[2][2]);
		}

		return tmp;
	}

	Matrix33F Transpose()
	{
		Matrix33F tmp;
		for (int i = 0; i < 3; i++)
		{
			tmp.m[i][0] = this->m[0][i];
			tmp.m[i][1] = this->m[1][i];
			tmp.m[i][2] = this->m[2][i];
		}

		return tmp;
		//memcpy(this, &tmp, sizeof(Matrix33F));
	}
};