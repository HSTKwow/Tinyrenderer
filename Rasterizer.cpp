#include "Rasterizer.h"

Vector3 baryCentric(const Vector4* pts, float i,float j)	//计算重心
{
	float xa = pts[0].x;
	float ya = pts[0].y;
	float xb = pts[1].x;
	float yb = pts[1].y;
	float xc = pts[2].x;
	float yc = pts[2].y;
	float x = i;
	float y = j;
	float gamma = (float)((ya - yb) * x + (xb - xa) * y + xa * yb - xb * ya) / (float)((ya - yb) * xc + (xb - xa) * yc + xa * yb - xb * ya);
	float beta = (float)((ya - yc) * x + (xc - xa) * y + xa * yc - xc * ya) / (float)((ya - yc) * xb + (xc - xa) * yb + xa * yc - xc * ya);
	float alpha = 1 - gamma - beta;
	return Vector3(alpha, beta, gamma);
}

void rst::rasterizer::set_model(const Matrix& m)		//设置模型变换矩阵
{
	modelMatrix = m;
}

void rst::rasterizer::set_view(const Matrix& v)			//设置视图变换矩阵
{
	viewMatrix = v;
}

void rst::rasterizer::set_projection(const Matrix& p)	//设置投影变换矩阵
{
	projectionMatrix = p;
}

void rst::rasterizer::set_texture(Texture tex)			//设置纹理
{
	texture = tex;
}

void rst::rasterizer::clear(Buffers buf)				//清除帧缓冲和深度缓冲
{
	//初始化
	if (buf == Buffers::Color)
		fill(frame_buffer.begin(), frame_buffer.end(), Vector3(0.0f, 0.0f, 0.0f));		//初始化frame_buffer
	else if (buf == Buffers::Depth)
		fill(zBuffer.begin(), zBuffer.end(), -numeric_limits<float>::infinity());		//初始化zBuffer
}

void rst::rasterizer::set_pixel(Vector2& point, Vector3& color)		//设置某个点颜色
{
	int idx = point.y * width + point.x;		//全部像素一维数组存储，故要转化为一维数组下标
	frame_buffer[idx] = color;
}

void rst::rasterizer::set_vertexShader(std::function<Vector3(vertex_shader_payload)> vert_shader)		//设置顶点着色器
{
	vertexShader = vert_shader;
}

void rst::rasterizer::set_fragmentShader(std::function<Vector3(fragment_shader_payload)> frag_shader)	//设置片元着色器
{
	fragmentShader = frag_shader;
}

void rst::rasterizer::draw(vector<triangle> triangleList)
{
	float f1 = (255 - 0.0) / 2.0;				//深度
	float f2 = (255 + 0.0) / 2.0;	

	Matrix mvp = modelMatrix * viewMatrix * projectionMatrix;		//MVP变换矩阵

	int count = 0;

	for (const auto& t : triangleList)			//遍历三角形
	{
		int depth = 255;						//认为 near = 0.0f, far = 255.0f
		triangle n = t;							//当前三角形
		Vector4 v[3];							//三个顶点齐次坐标(存储MVP变换后的坐标)
		
		//两个计算颜色方法
		//n.computeFcolor(Vector3(0., 0., -1.));
		n.computeGColor(Vector3(0., 0., -1.));

		for (int i = 0; i < 3; i++)				//三个顶点转换为齐次坐标
			v[i] = t.v[i];

		for (auto& vec : v)						//MVP变换
		{
			Matrix tmp(4, 1);					//转换后的齐次坐标,规范视域体(CVV)
			tmp = mvp * vec;
			vec.x = tmp[0][0];
			vec.y = tmp[1][0];
			vec.z = tmp[2][0];
			vec.w = tmp[3][0];
		}

		for (auto& vec : v)						//透视除法(NDC坐标)
		{
			vec.x /=  vec.w;
			vec.y /=  vec.w;
			vec.z /=  vec.w;
		}

		for (auto& vec : v)						//视口变化
		{
			float w = width * 3.f / 4.f;
			float h = height * 3.f / 4.f;
			float x = width / 8.f;
			float y = height / 8.f;
			vec.x = w / 2.f * vec.x + w / 2.f + x;
			vec.y = h / 2.f * vec.y + h / 2.f + y;
			vec.z = vec.z * f1 + f2;
		}

		for (int i = 0; i < 3; i++)				//转换为screenCoord
			n.v[i] = v[i];
		
		rasterizer_triangle(n);					//光栅化
	}
}

void rst::rasterizer::rasterizer_triangle(triangle t)
{
	const Vector4* pts = t.v;			//三个顶点坐标

	//Bouding Box
	float min_x = min(t.v[0].x, min(t.v[1].x, t.v[2].x));
	float min_y = min(t.v[0].y, min(t.v[1].y, t.v[2].y));
	float max_x = max(t.v[0].x, max(t.v[1].x, t.v[2].x));
	float max_y = max(t.v[0].y, max(t.v[1].y, t.v[2].y));
	min_x = (int)floor(min_x);		//向下取整
	min_y = (int)floor(min_y);
	max_x = (int)ceil(max_x);		//向上取整
	max_y = (int)ceil(max_y);


	for (int i = min_x; i <= max_x; i++)
	{
		for (int j = min_y; j <= max_y; j++)
		{
			Vector2 point(i, j);
			Vector3 baryCoord = baryCentric(pts, i, j);

			t.computeFcolor(Vector3(0., 0., -1.));
			//不在三角形内
			if (baryCoord.x < 0 || baryCoord.y < 0 || baryCoord.z < 0)
				continue;
			float z_interpolation = pts[0].z * baryCoord.x + pts[1].z * baryCoord.y + pts[2].z * baryCoord.z;
			Vector2 uv_interpolation = t.texCoords[0] * baryCoord.x + t.texCoords[1] * baryCoord.y + t.texCoords[2] * baryCoord.z;
			Vector3 color_interpolation = t.color[0] * baryCoord.x + t.color[1] * baryCoord.y + t.color[2] * baryCoord.z;
			Vector3 normal_interpolation = t.normal[0] * baryCoord.x + t.normal[1] * baryCoord.y + t.normal[2] * baryCoord.z;

			fragment_shader_payload payload(color_interpolation, normal_interpolation.normalize(), uv_interpolation, texture ? &*texture : nullptr);

			if (z_interpolation > zBuffer[i + j * width])
			{
				//离观察点更近，更新zBuffer
				zBuffer[i + j * width] = z_interpolation;
				auto pixel_color = fragmentShader(payload);
				set_pixel(point, pixel_color);			//设置颜色
			}
		}
	}
}


