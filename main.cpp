
#include"Gmath.h"
#include"Model.h"
#include"Rasterizer.h"
#include"Texture.h"

Vector3 light_dir(0., 0., -1.f);			//��ֱ��Դ
Vector3 cameraPos(0, 0, 3);					//���λ��
const int width = 600;						//��Ļ���
const int height = 600;						//��Ļ�߶�

Matrix modelMatrix()						//ģ��ת������
{
	return Matrix::identity(4);
}

Matrix viewMatrix()							//��ͼת������
{
	return Matrix::identity(4);
}

Matrix projectionMatrix()					//ͶӰת������
{
	Matrix projection = Matrix::identity(4);
	projection[3][2] = -1.0f / cameraPos.z;
	return projection;
}

Vector3 vertex_shader(const vertex_shader_payload& payload)		//������ɫ��
{
	return payload.position;
}

//����ƬԪ��ɫ��

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

    TGAImage image(width, height, TGAImage::Format::RGB);       //����һ��TGAͼ�񣬴�СΪwidth*height����ʽΪRGB

    //�洢������Ҫ���Ƶ���������Ƭ


    //������դ������
    rst::rasterizer r(width, height);

    //������������
    Texture tex("obj\\african_head\\african_head_diffuse.tga");
    r.set_texture(tex);

    //���֡�����zBuffer
    r.clear(rst::Buffers::Color);
    r.clear(rst::Buffers::Depth);

    //����MVP����
    r.set_model(modelMatrix());
    r.set_view(viewMatrix());
    r.set_projection(projectionMatrix());

    //���ö�����ɫ����ƬԪ��ɫ��
    r.set_vertexShader(vertex_shader);
    r.set_fragmentShader(normal_fragment_shader);

    //����ģ��
    r.draw(model->triangleList);

    //��֡�����е���ɫֵд��image��
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Vector3 color = r.frame_buffer[j * width + i];
            image.set(i, j, TGAColor(color.x, color.y, color.z, 255));
        }
    }

    image.flip_vertically();                //TGAͼ���Ǵ��µ��ϵģ�������Ҫ��ת
    image.write_tga_file("output.tga");     //����ͼ��
}



//��һ����ʾ��ʽ

//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//
//
//int WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,LPSTR lpCmdline, int nCmdShow)
//{
//	static TCHAR szAppName[] = TEXT("HSTK");
//	HWND hwnd;				//���ھ��
//	MSG msg;				//��Ϣ�ṹ
//	WNDCLASS wndclass;		//������
//
//	//�������ʼ��
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
//	//ע�ᴰ��
//	if (!RegisterClass(&wndclass))
//	{
//		MessageBox(NULL, TEXT("This progrma requires Windows NOT !"),
//			szAppName, MB_ICONERROR);
//		return 0;
//	}
//
//	// �������ڲ���ȡ���ھ��
//	hwnd = CreateWindow(szAppName,		// window class name
//		TEXT("Graphic"),				// window caption(����)
//		WS_OVERLAPPEDWINDOW,			// window style
//		CW_USEDEFAULT,					// initial x position
//		CW_USEDEFAULT,					// initial y position
//		width,							// initial x size
//		height,							// initial y size
//		NULL,							// parent window handle
//		NULL,							// window menu handle
//		hInstance,						// program instance handle
//		NULL);							// crea
//	//��ʾ���ڲ�����
//	ShowWindow(hwnd, nCmdShow);
//	UpdateWindow(hwnd);
//	//��Ϣ����
//	while (GetMessage(&msg, NULL, 0, 0))		//��Ϣѭ��
//	{
//		TranslateMessage(&msg);			//��Ϣ����
//		DispatchMessage(&msg);			//��Ϣ�ַ�
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
//		r.set_texture(texture);								//��������
//		r.clear(rst::Buffers::Color);						//��ʼ��֡����
//		r.clear(rst::Buffers::Depth);						//��ʼ��zBuffer
//		r.set_model(modelMatrix());							//����ģ��ת������
//		r.set_view(viewMatrix());							//������ͼת������
//		r.set_projection(projectionMatrix());				//����ͶӰת������
//		r.set_fragmentShader(ground_fragment_shader);		//ƬԪ��ɫ��
//		r.set_vertexShader(vertex_shader);					//������ɫ��
//		r.draw(model.triangleList);							//ģ�ͻ���
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




