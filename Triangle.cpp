#include <iostream>
#include "Triangle.h"

triangle::triangle()			//初始化
{
	
	v[0] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);		//齐次坐标
	v[1] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	v[2] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	
	color[0] = Vector3(0.0f, 0.0f, 0.0f);		//颜色
	color[1] = Vector3(0.0f, 0.0f, 0.0f);
	color[2] = Vector3(0.0f, 0.0f, 0.0f);

	texCoords[0] = Vector2(0.0f, 0.0f);			//纹理坐标
	texCoords[1] = Vector2(0.0f, 0.0f);

	normal[0] = Vector3(0.0f, 0.0f, 0.0f);		//法线
	normal[1] = Vector3(0.0f, 0.0f, 0.0f);
	normal[2] = Vector3(0.0f, 0.0f, 0.0f);
}

//两种实现方法

void triangle::computeGColor(Vector3 light_dir)		//根据光源计算颜色
{
	for (int i = 0; i < 3; i++)		//计算三个顶点的颜色
	{
		float intensity = max(0.f, normal[i].normalize() * light_dir.normalize());		//计算光照强度
		if (intensity < 0)		//光照强度小于0
			intensity = 0.f;
		color[i] = Vector3(255, 255, 255) * intensity;
	}
}	

void triangle::computeFcolor(Vector3 light_dir)		//计算颜色
{
	//计算三角形法线
	Vector3 normal_ = (Vector3(v[0].x, v[0].y, v[0].z) - Vector3(v[1].x, v[1].y, v[1].z)) ^ (Vector3(v[2].x, v[2].y, v[2].z) - Vector3(v[1].x, v[1].y, v[1].z));
	normal_.normalize();		//归一化
	float intensity = normal_.normalize() * light_dir.normalize();
	if (intensity < 0)intensity = 0.f;	
	for (int i = 0; i < 3; i++)
	{
		color[i] = Vector3(255, 255, 255) * intensity;
	}
}

