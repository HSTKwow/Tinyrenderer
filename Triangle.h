#pragma once
#include "Gmath.h"
#include "Texture.h"

class triangle
{
	public:
		Vector4 v[3];					//�������
		Vector3 normal[3];				//����
		Vector2 texCoords[3];			//��������		
		Vector3 color[3];				//��ɫ
		triangle();						//��ʼ��
		void computeGColor(Vector3 light_dir);
		void computeFcolor(Vector3 light_dir);
};