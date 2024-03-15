#pragma once

#include "iostream"
#include "tgaimage.h"

class Texture
{
private:
	TGAImage image_data;			//纹理图是一个TGAImage对象
public:
	int width, height;
	Texture(const char* filename)
	{
		image_data.read_tga_file(filename);
		image_data.flip_vertically();
		width = image_data.get_width();
		height = image_data.get_height();
	}
	TGAColor getColor(float u, float v)
	{
		return image_data.get(u, v);
	}
};
