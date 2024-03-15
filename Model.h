#pragma once

#include "Gmath.h"				//数学库
#include "tgaimage.h"
#include "Texture.h"			
#include "Triangle.h"			

class Model
{
private:
	Texture* texture = nullptr;					//纹理名字
	int vertNum;								//顶点数
	int faceNum;								//面数

public:
	vector<triangle> triangleList;				//三角形表
	Model(const char*);							//根据文件导入模型函数
	~Model();	
	int nVerts();								//返回顶点个数的函数
	int nFaces();								//返回三角形数的函数
};

