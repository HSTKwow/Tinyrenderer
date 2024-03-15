#pragma once

#include "Gmath.h"
#include "Texture.h"


struct fragment_shader_payload          //片元着色器数据
{
    fragment_shader_payload()       
    {
        texture = nullptr;
    }
    fragment_shader_payload(const Vector3& color, const Vector3& normal, const Vector2& texCoord, Texture* tex) :
        color(color), normal(normal), tex_coords(texCoord), texture(tex) {}

    Vector3 color;              //颜色
    Vector3 view_pos;           //观察点
    Vector3 normal;             //法线
    Vector2 tex_coords;         //纹理坐标
    Texture* texture;           //纹理文件名
};

struct vertex_shader_payload        //顶点着色器数据
{
    Vector3 position;
};









