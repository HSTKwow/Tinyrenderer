#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Model.h"
#include "Triangle.h"

//构造函数，输入参数是.obj文件路径
Model::Model(const char* filename) : vertNum(0), faceNum(0) 
{
    vector<Vector4>verts_;                  //顶点坐标      
    vector<vector<Vector3>>faces_;          //面数idx索引值
    vector<Vector3>norms_;                  //法线坐标
    vector<Vector2>uv_;                     //纹理坐标

    std::ifstream in;
    in.open(filename, std::ifstream::in);           //打开.obj文件
    if (in.fail()) return;
    string line;
    while (!in.eof())                               //没有到文件末尾的话
    {
        getline(in, line);                          //读入一行
        istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v "))              //顶点坐标
        {
            iss >> trash;
            Vector4 v;                              
            iss >> v.x >> v.y >> v.z;
            v.w = 1.f;
            verts_.push_back(v);                   
        }
        else if (!line.compare(0, 3, "vt "))        //纹理坐标
        {
            iss >> trash >> trash;
            Vector2 uv;
            iss >> uv.x >> uv.y;
            uv_.push_back(uv);
        }
        else if (!line.compare(0, 3, "vn "))        //法线
        {
            iss >> trash >> trash;
            Vector3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            norms_.push_back(normal);
        }
        else if (!line.compare(0, 2, "f "))         //三角形面片数据
        {
            vector<Vector3> f;
            Vector3 tmp;                             //idx是顶点索引，tmp用来读我们暂时用不到的纹理坐标和法线向量
            iss >> trash;                            //存放斜杠
            while (iss >> tmp.x >> trash >> tmp.y >> trash >> tmp.z) //读取x/x/x格式
            {
                tmp.x--, tmp.y--, tmp.z--;
                f.push_back(tmp);                   //读入三个顶点的idx到f中               
            }
            faces_.push_back(f);                    //把该面片加入模型的三角形面片集
        }
    }

    
    triangleList.resize(faces_.size());
    triangleList.clear();

    for (int i = 0; i < faces_.size(); i++)
    {
        triangle tmp;
        vector<Vector3> face = faces_[i];           //第i个面的索引值
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
        triangleList.push_back(tmp);                //放入三角形面片集
    }
    vertNum = verts_.size();
    faceNum = faces_.size();
}

Model::~Model() 
{

}

int Model::nVerts()         //返回顶点个数
{
    return (int)vertNum;
}

int Model::nFaces()        //返回面片个数
{
    return (int)faceNum;
}

