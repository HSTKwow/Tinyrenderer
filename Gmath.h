#pragma once

#pragma warning(disable:4996)
#include<iostream>
#include<algorithm>
#include <vector>
#include<assert.h>
using namespace std;

#define white RGB(255,255,255)
#define black RGB(0,0,0)
#define red RGB(255,0,0)
#define yellow RGB(255,255,0)
#define green RGB(0,255,128)
#define blue RGB(0,255,255)


//二维向量
class Vector2
{
public:
	float x, y;
	Vector2() :x(0), y(0) {};
	Vector2(float a, float b)
	{
		x = a;
		y = b;
	}
	Vector2 operator=(const Vector2& v1)
	{
		this->x = v1.x;
		this->y = v1.y;
		return *this;
	}
	float operator[](int idx)
	{
		if (idx <= 0)
			return x;
		else
			return y;
	}
	Vector2 operator +(const Vector2& v)
	{
		return Vector2(x + v.x, y + v.y);
	}
	Vector2 operator -(const Vector2& v)
	{
		return Vector2(x - v.x, y - v.y);
	}
	Vector2 operator *(float n)
	{
		return Vector2(x * n, y * n);
	}
};

//三维向量
class Vector3
{
public:
	float x, y, z;
	Vector3(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}
	Vector3()
	{
		x = y = z = 0;
	}
	Vector3 operator=(const Vector3& v1)
	{
		this->x = v1.x;
		this->y = v1.y;
		this->z = v1.z;
		return *this;
	}
	float operator[](int idx)
	{
		if (idx <= 0)
			return x;
		else if (idx == 1)
			return y;
		else
			return z;
	}
	Vector3 operator+(const Vector3& v)		//加法
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	Vector3 operator-(const Vector3& v)		//减法
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	float operator*(const Vector3& v)		//乘法
	{
		return x * v.x + y * v.y + z * v.z;
	}
	Vector3 operator^(const Vector3& v)		//叉乘
	{
		return Vector3((y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x));
	}
	Vector3 operator*(float a)				//数乘
	{
		return Vector3(a * x, a * y, a * z);
	}
	Vector3 operator/(float a)				//除法
	{
		return Vector3(x / a, y / a, z / a);

	}
	float normal()							//模长
	{
		return sqrt(x * x + y * y + z * z);
	}										
	Vector3 normalize()						//单位化
	{
		return Vector3(x /= normal(), y /= normal(), z /= normal());
	}
	Vector3 Lerp(const Vector3& from, const Vector3& to, float t)
	{
		return Vector3((to.x - from.x) * t + from.x, (to.y - from.y) * t + from.y,
			(to.z - from.z) * t + from.z);
	}
	bool operator==(const  Vector3& v)				//判断向量是否相等
	{
		return ((x == v.x) && (y == v.y) && (z == v.z));
	}
	bool operator!=(const Vector3& v)
	{
		return ((x != v.x) || (y != v.y) || (z != v.z));
	}

};

//四维向量
class Vector4
{
public:
	float x, y, z, w;
	Vector4(float a, float b, float c,float d)
	{
		x = a;
		y = b;
		z = c;
		w = d;
	}
	Vector4()
	{
		x = y = z = w = 0;
	}
	Vector4 operator=(const Vector4& v1)
	{
		this->x = v1.x;
		this->y = v1.y;
		this->z = v1.z;
		this->w = v1.w;
		return *this;
	}
	float& operator[](const int idx)
	{
		if (idx <= 0)
			return x;
		else if (idx == 1)
			return y;
		else if (idx == 2)
			return z;
		else
			return w;
	}
	Vector4 operator+(const Vector4& v)			//加法
	{
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	Vector4 operator-(const Vector4& v)			//减法
	{
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	float operator*(const Vector4& v)			//点乘
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}
	Vector4 operator*(float a)					//数乘
	{
		return Vector4(a * x, a * y, a * z, a * w);
	}
	float normal()								//模长
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}
	Vector4 normalize()							//单位化
	{
		return Vector4(x /= normal(), y /= normal(), z /= normal(), w /= normal());
	}
};

//矩阵
class Matrix
{
private:
	vector<vector<float>> m;
	int rows, cols;												
public:
	Matrix(int r = 0, int c = 0) : rows(r), cols(c)	//构造函数
	{
		m = vector<vector<float>>(r, vector<float>(c, 0.0f));		//r行c列
	}
	int nrows()
	{
		return rows;
	}
	int ncols()
	{
		return cols;
	}
	static Matrix identity(int dimensions)			//单位化						
	{
		//dimensions维度
		Matrix E(dimensions, dimensions);
		for (int i = 0; i < dimensions; i++)
			for (int j = 0; j < dimensions; j++)
				E[i][j] = (i == j ? 1.0f : 0.0f);
		return E;
	}
	vector<float>& operator[](const int i)
	{
		assert(i >= 0 && i < rows);
		return m[i];
	}
	Matrix operator*(const Matrix& a)		//乘
	{
		assert(cols == a.rows);			//左列=右行
		Matrix ans(rows, a.cols);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < a.cols; j++)
				for (int k = 0; k < cols; k++)
					ans[i][j] += m[i][k] * a.m[k][j];
		return ans;
	}
	Matrix transpose()			//转置
	{
		Matrix ans(cols, rows);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				ans[i][j] = m[j][i];
		return ans;
	}
	Matrix operator*(Vector4& v)
	{
		Matrix ans(4, 1);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				ans[i][0] += m[i][j] * v[j];
			}
		}
		return ans;
	}

};

//3阶矩阵
class Matrix3
{
public:
	Vector3 rows[3];
	Matrix3();
	Vector3& operator[](const int);							//访问第i行
	Matrix3 operator*(const Matrix3&); 						//矩阵乘法
	Matrix3 operator*(const Vector3&); 						//矩阵乘法
	Matrix3 transpose();									//转置
};

//4阶矩阵
class Matrix4
{
	Vector4 rows[4];
	Matrix4();
	Vector4& operator[](const int);							//访问第i行
	Matrix4 operator*(const Matrix4&); 						//矩阵乘法
	Matrix4 operator*(const Vector4&); 						//矩阵乘法
	Matrix4 transpose();									//转置
};

