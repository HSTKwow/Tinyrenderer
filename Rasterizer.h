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
	enum class Buffers			//缓冲区
	{
		Color = 1,
		Depth = 2
	};
	enum class Primitive		//基本图元
	{
		Line,
		Triangle
	};
	class rasterizer			//光栅化
	{
	private:
		Matrix modelMatrix;						//模型变换矩阵
		Matrix viewMatrix;						//视图变换矩阵
		Matrix projectionMatrix;				//透视投影变换矩阵
		vector<float> zBuffer;					//深度缓冲
		int width, height;						//高度&宽度
		std::optional <Texture> texture;		//纹理图(optional可有可无)

		std::function<Vector3(fragment_shader_payload)> fragmentShader;		//片元着色器
		std::function<Vector3(vertex_shader_payload)> vertexShader;			//顶点着色器

		void draw_line();
		void rasterizer_triangle(triangle t);			//光栅化三角形

	public:
		vector<Vector3> frame_buffer;						//帧缓冲|颜色图
		rasterizer(int w, int h) :width(w), height(h)		//构造函数
		{
			frame_buffer.resize(w * h);
			zBuffer.resize(w * h);
			texture = nullopt;
		}
		void set_model(const Matrix& m);			//设置模型变换矩阵
		void set_view(const Matrix& v);				//设置视图变换矩阵
		void set_projection(const Matrix& p);		//设置投影变换矩阵
		void set_texture(Texture tex);				//设置纹理	

		void set_vertexShader(std::function<Vector3(vertex_shader_payload)> vert_shader);			//设置顶点着色器
		void set_fragmentShader(std::function<Vector3(fragment_shader_payload)> frag_shader);		//设置片元着色器

		void clear(Buffers buf);									//清除帧缓冲和深度缓冲
		void set_pixel(Vector2& point, Vector3& color);				//设置某个点颜色
		void draw(vector<triangle> triangleList);					//绘制三角形
	};
}

