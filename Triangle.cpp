#include <iostream>
#include "Triangle.h"

triangle::triangle()			//��ʼ��
{
	
	v[0] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);		//�������
	v[1] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	v[2] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	
	color[0] = Vector3(0.0f, 0.0f, 0.0f);		//��ɫ
	color[1] = Vector3(0.0f, 0.0f, 0.0f);
	color[2] = Vector3(0.0f, 0.0f, 0.0f);

	texCoords[0] = Vector2(0.0f, 0.0f);			//��������
	texCoords[1] = Vector2(0.0f, 0.0f);

	normal[0] = Vector3(0.0f, 0.0f, 0.0f);		//����
	normal[1] = Vector3(0.0f, 0.0f, 0.0f);
	normal[2] = Vector3(0.0f, 0.0f, 0.0f);
}

//����ʵ�ַ���

void triangle::computeGColor(Vector3 light_dir)		//���ݹ�Դ������ɫ
{
	for (int i = 0; i < 3; i++)		//���������������ɫ
	{
		float intensity = max(0.f, normal[i].normalize() * light_dir.normalize());		//�������ǿ��
		if (intensity < 0)		//����ǿ��С��0
			intensity = 0.f;
		color[i] = Vector3(255, 255, 255) * intensity;
	}
}	

void triangle::computeFcolor(Vector3 light_dir)		//������ɫ
{
	//���������η���
	Vector3 normal_ = (Vector3(v[0].x, v[0].y, v[0].z) - Vector3(v[1].x, v[1].y, v[1].z)) ^ (Vector3(v[2].x, v[2].y, v[2].z) - Vector3(v[1].x, v[1].y, v[1].z));
	normal_.normalize();		//��һ��
	float intensity = normal_.normalize() * light_dir.normalize();
	if (intensity < 0)intensity = 0.f;	
	for (int i = 0; i < 3; i++)
	{
		color[i] = Vector3(255, 255, 255) * intensity;
	}
}

