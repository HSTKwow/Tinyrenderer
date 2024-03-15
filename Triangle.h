#pragma once
#include "Gmath.h"
#include "Texture.h"

class triangle
{
	public:
		Vector4 v[3];					//齐次坐标
		Vector3 normal[3];				//法线
		Vector2 texCoords[3];			//纹理坐标		
		Vector3 color[3];				//颜色
		triangle();						//初始化
		void computeGColor(Vector3 light_dir);
		void computeFcolor(Vector3 light_dir);
};