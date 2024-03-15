
#include"Gmath.h"
#include"Model.h"
#include"Rasterizer.h"
#include"Texture.h"

Vector3 light_dir(0., 0., -1.f);			//垂直光源
Vector3 cameraPos(0, 0, 3);					//相机位置
const int width = 600;						//屏幕宽度
const int height = 600;						//屏幕高度

Matrix modelMatrix()						//模型转换矩阵
{
	return Matrix::identity(4);
}

Matrix viewMatrix()							//视图转换矩阵
{
	return Matrix::identity(4);
}

Matrix projectionMatrix()					//投影转换矩阵
{
	Matrix projection = Matrix::identity(4);
	projection[3][2] = -1.0f / cameraPos.z;
	return projection;
}

Vector3 vertex_shader(const vertex_shader_payload& payload)		//顶点着色器
{
	return payload.position;
}

//几种片元着色器

Vector3 normal_fragment_shader(const fragment_shader_payload& payload)		
{
	Vector3 normal_frag = payload.normal;
	Vector3 color = (normal_frag.normalize() + Vector3(1.0f, 1.0f, 1.0f)) * 0.5;
	return Vector3(color.x * 255, color.y * 255, color.z * 255);
}

Vector3 flat_fragment_shader(const fragment_shader_payload& payload)
{
	Vector3 color = payload.color;
	return color;
}


Model* model;
Texture* texture;

int main(int argc, char** argv)         
{
    if (2 == argc) 
    {
        model = new Model(argv[1]);
    }
    else
    {
        model = new Model("obj\\african_head\\african_head.obj");
    }

    cout << model->nFaces() << "  " << model->nVerts() << std::endl;

    TGAImage image(width, height, TGAImage::Format::RGB);       //创建一个TGA图像，大小为width*height，格式为RGB

    //存储所有需要绘制的三角形面片


    //创建光栅化对象
    rst::rasterizer r(width, height);

    //给定纹理并设置
    Texture tex("obj\\african_head\\african_head_diffuse.tga");
    r.set_texture(tex);

    //清空帧缓冲和zBuffer
    r.clear(rst::Buffers::Color);
    r.clear(rst::Buffers::Depth);

    //设置MVP矩阵
    r.set_model(modelMatrix());
    r.set_view(viewMatrix());
    r.set_projection(projectionMatrix());

    //设置顶点着色器和片元着色器
    r.set_vertexShader(vertex_shader);
    r.set_fragmentShader(normal_fragment_shader);

    //绘制模型
    r.draw(model->triangleList);

    //将帧缓冲中的颜色值写入image中
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Vector3 color = r.frame_buffer[j * width + i];
            image.set(i, j, TGAColor(color.x, color.y, color.z, 255));
        }
    }

    image.flip_vertically();                //TGA图像是从下到上的，所以需要翻转
    image.write_tga_file("output.tga");     //保存图像
}



//另一种显示方式

//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//
//
//int WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,LPSTR lpCmdline, int nCmdShow)
//{
//	static TCHAR szAppName[] = TEXT("HSTK");
//	HWND hwnd;				//窗口句柄
//	MSG msg;				//消息结构
//	WNDCLASS wndclass;		//窗口类
//
//	//窗口类初始化
//	wndclass.style = CS_HREDRAW | CS_VREDRAW;
//	wndclass.lpfnWndProc = WndProc;
//	wndclass.cbClsExtra = 0;
//	wndclass.cbWndExtra = 0;
//	wndclass.hInstance = hInstance;
//	wndclass.hIcon = NULL;
//	wndclass.hCursor = NULL;
//	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//	wndclass.lpszMenuName = NULL;
//	wndclass.lpszClassName = szAppName;
//
//	//注册窗口
//	if (!RegisterClass(&wndclass))
//	{
//		MessageBox(NULL, TEXT("This progrma requires Windows NOT !"),
//			szAppName, MB_ICONERROR);
//		return 0;
//	}
//
//	// 创建窗口并获取窗口句柄
//	hwnd = CreateWindow(szAppName,		// window class name
//		TEXT("Graphic"),				// window caption(标题)
//		WS_OVERLAPPEDWINDOW,			// window style
//		CW_USEDEFAULT,					// initial x position
//		CW_USEDEFAULT,					// initial y position
//		width,							// initial x size
//		height,							// initial y size
//		NULL,							// parent window handle
//		NULL,							// window menu handle
//		hInstance,						// program instance handle
//		NULL);							// crea
//	//显示窗口并更新
//	ShowWindow(hwnd, nCmdShow);
//	UpdateWindow(hwnd);
//	//消息处理
//	while (GetMessage(&msg, NULL, 0, 0))		//消息循环
//	{
//		TranslateMessage(&msg);			//消息翻译
//		DispatchMessage(&msg);			//消息分发
//	}
//
//	return msg.wParam;
//}
//
//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	switch (msg)
//	{
//	case WM_PAINT:
//	{
//		PAINTSTRUCT ps;
//		HDC hdc = BeginPaint(hwnd, &ps);
//
//		Model model("obj\\african_head\\african_head.obj");
//		Texture texture("obj\\african_head\\african_head_diffuse");
//
//		TGAImage image(width, height, TGAImage::Format::RGB);
//		
//		cout << model.nFaces() << endl;
//
//		rst::rasterizer r(width, height);
//		r.set_texture(texture);								//设置纹理
//		r.clear(rst::Buffers::Color);						//初始化帧缓冲
//		r.clear(rst::Buffers::Depth);						//初始化zBuffer
//		r.set_model(modelMatrix());							//设置模型转换矩阵
//		r.set_view(viewMatrix());							//设置视图转换矩阵
//		r.set_projection(projectionMatrix());				//设置投影转换矩阵
//		r.set_fragmentShader(ground_fragment_shader);		//片元着色器
//		r.set_vertexShader(vertex_shader);					//顶点着色器
//		r.draw(model.triangleList);							//模型绘制
//		
//
//		for (int i = 0; i < width; i++)
//		{
//			for (int j = 0; j < height; j++)
//			{
//				Vector3 color = r.frame_buffer[i + j * width];
//				SetPixel(hdc, i, height - j, RGB(color.x, color.y, color.z));
//			}
//			//RGB(255, 255, 255)
//			//RGB(color.x, color.y, color.z)
//		}
//
//
//
//		EndPaint(hwnd, &ps);
//		break;
//	}
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hwnd, msg, wParam, lParam);
//	}
//	return 0;
//}




