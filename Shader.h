#pragma once

#include "Gmath.h"
#include "Texture.h"


struct fragment_shader_payload          //ƬԪ��ɫ������
{
    fragment_shader_payload()       
    {
        texture = nullptr;
    }
    fragment_shader_payload(const Vector3& color, const Vector3& normal, const Vector2& texCoord, Texture* tex) :
        color(color), normal(normal), tex_coords(texCoord), texture(tex) {}

    Vector3 color;              //��ɫ
    Vector3 view_pos;           //�۲��
    Vector3 normal;             //����
    Vector2 tex_coords;         //��������
    Texture* texture;           //�����ļ���
};

struct vertex_shader_payload        //������ɫ������
{
    Vector3 position;
};









