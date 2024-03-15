#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Model.h"
#include "Triangle.h"

//���캯�������������.obj�ļ�·��
Model::Model(const char* filename) : vertNum(0), faceNum(0) 
{
    vector<Vector4>verts_;                  //��������      
    vector<vector<Vector3>>faces_;          //����idx����ֵ
    vector<Vector3>norms_;                  //��������
    vector<Vector2>uv_;                     //��������

    std::ifstream in;
    in.open(filename, std::ifstream::in);           //��.obj�ļ�
    if (in.fail()) return;
    string line;
    while (!in.eof())                               //û�е��ļ�ĩβ�Ļ�
    {
        getline(in, line);                          //����һ��
        istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v "))              //��������
        {
            iss >> trash;
            Vector4 v;                              
            iss >> v.x >> v.y >> v.z;
            v.w = 1.f;
            verts_.push_back(v);                   
        }
        else if (!line.compare(0, 3, "vt "))        //��������
        {
            iss >> trash >> trash;
            Vector2 uv;
            iss >> uv.x >> uv.y;
            uv_.push_back(uv);
        }
        else if (!line.compare(0, 3, "vn "))        //����
        {
            iss >> trash >> trash;
            Vector3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            norms_.push_back(normal);
        }
        else if (!line.compare(0, 2, "f "))         //��������Ƭ����
        {
            vector<Vector3> f;
            Vector3 tmp;                             //idx�Ƕ���������tmp������������ʱ�ò�������������ͷ�������
            iss >> trash;                            //���б��
            while (iss >> tmp.x >> trash >> tmp.y >> trash >> tmp.z) //��ȡx/x/x��ʽ
            {
                tmp.x--, tmp.y--, tmp.z--;
                f.push_back(tmp);                   //�������������idx��f��               
            }
            faces_.push_back(f);                    //�Ѹ���Ƭ����ģ�͵���������Ƭ��
        }
    }

    
    triangleList.resize(faces_.size());
    triangleList.clear();

    for (int i = 0; i < faces_.size(); i++)
    {
        triangle tmp;
        vector<Vector3> face = faces_[i];           //��i���������ֵ
        Vector3 face_vertex_1 = face[0];
        Vector3 face_vertex_2 = face[1];
        Vector3 face_vertex_3 = face[2];
        tmp.v[0] = verts_[face_vertex_1.x];
        tmp.v[1] = verts_[face_vertex_2.x];
        tmp.v[2] = verts_[face_vertex_3.x];
        tmp.texCoords[0] = uv_[face_vertex_1.y];
        tmp.texCoords[1] = uv_[face_vertex_2.y];
        tmp.texCoords[2] = uv_[face_vertex_3.y];        
        tmp.normal[0] = norms_[face_vertex_1.z];
        tmp.normal[1] = norms_[face_vertex_2.z];
        tmp.normal[2] = norms_[face_vertex_3.z];
        triangleList.push_back(tmp);                //������������Ƭ��
    }
    vertNum = verts_.size();
    faceNum = faces_.size();
}

Model::~Model() 
{

}

int Model::nVerts()         //���ض������
{
    return (int)vertNum;
}

int Model::nFaces()        //������Ƭ����
{
    return (int)faceNum;
}

