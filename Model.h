#pragma once

#include "Gmath.h"				//��ѧ��
#include "tgaimage.h"
#include "Texture.h"			
#include "Triangle.h"			

class Model
{
private:
	Texture* texture = nullptr;					//��������
	int vertNum;								//������
	int faceNum;								//����

public:
	vector<triangle> triangleList;				//�����α�
	Model(const char*);							//�����ļ�����ģ�ͺ���
	~Model();	
	int nVerts();								//���ض�������ĺ���
	int nFaces();								//�������������ĺ���
};

