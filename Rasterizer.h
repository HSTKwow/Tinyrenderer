#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include <functional>
#include "Gmath.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"

namespace rst			//Rasterizer
{
	enum class Buffers			//������
	{
		Color = 1,
		Depth = 2
	};
	enum class Primitive		//����ͼԪ
	{
		Line,
		Triangle
	};
	class rasterizer			//��դ��
	{
	private:
		Matrix modelMatrix;						//ģ�ͱ任����
		Matrix viewMatrix;						//��ͼ�任����
		Matrix projectionMatrix;				//͸��ͶӰ�任����
		vector<float> zBuffer;					//��Ȼ���
		int width, height;						//�߶�&���
		std::optional <Texture> texture;		//����ͼ(optional���п���)

		std::function<Vector3(fragment_shader_payload)> fragmentShader;		//ƬԪ��ɫ��
		std::function<Vector3(vertex_shader_payload)> vertexShader;			//������ɫ��

		void draw_line();
		void rasterizer_triangle(triangle t);			//��դ��������

	public:
		vector<Vector3> frame_buffer;						//֡����|��ɫͼ
		rasterizer(int w, int h) :width(w), height(h)		//���캯��
		{
			frame_buffer.resize(w * h);
			zBuffer.resize(w * h);
			texture = nullopt;
		}
		void set_model(const Matrix& m);			//����ģ�ͱ任����
		void set_view(const Matrix& v);				//������ͼ�任����
		void set_projection(const Matrix& p);		//����ͶӰ�任����
		void set_texture(Texture tex);				//��������	

		void set_vertexShader(std::function<Vector3(vertex_shader_payload)> vert_shader);			//���ö�����ɫ��
		void set_fragmentShader(std::function<Vector3(fragment_shader_payload)> frag_shader);		//����ƬԪ��ɫ��

		void clear(Buffers buf);									//���֡�������Ȼ���
		void set_pixel(Vector2& point, Vector3& color);				//����ĳ������ɫ
		void draw(vector<triangle> triangleList);					//����������
	};
}

