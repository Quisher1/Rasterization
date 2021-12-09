#include <iostream>

#include "src/bmp.h"
#include "src/utils.h"

Matrix<BYTE> *screen = new Matrix<BYTE>[3];

void setPixel(int x, int y, rgb color)
{
	screen[0](x, y) = color.r;
	screen[1](x, y) = color.g;
	screen[2](x, y) = color.b;
}


void drawLine(int x1, int y1, int x2, int y2, rgb color) {
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
	int error = deltaX - deltaY;
	setPixel(x2, y2, color);

	int x = x1, y = y1;

	while (x != x2 || y != y2)
	{
		setPixel(x, y, color);
		int error2 = error * 2;
		if (error2 > -deltaY)
		{
			error -= deltaY;
			x += signX;
		}
		if (error2 < deltaX)
		{
			error += deltaX;
			y += signY;
		}
	}
}

int main()
{
	uint width = 1000,
		height = 1000;

	//Matrix<BYTE> *screen = new Matrix<BYTE>[3];
	for (int i = 0; i < 3; ++i)
		screen[i] = Matrix<BYTE>(width, height);


	// vertex pos 
	float vertices[] = // 3D
	{
		//-0.5f, -0.5f, 0.0f,
		// 0.5f, -0.5f, 0.0f,
		// 0.0f,  0.5f, 0.0f

		518, 746, 0,		// 0 1 2
		637, 24, 0,			// 3 4 5
		801, 751, 0			// 6 7 8
	};
	float colors[] = 
	{
		255, 0, 0,			// 0 1 2
		0, 255, 0,			// 3 4 5
		0, 0, 255			// 6 7 8

	};


	/*drawLine(vertices[0], vertices[1], vertices[3], vertices[4], rgb(colors[0], colors[1], colors[2]));
	drawLine(vertices[3], vertices[4], vertices[6], vertices[7], rgb(colors[3], colors[4], colors[5]));
	drawLine(vertices[6], vertices[7], vertices[0], vertices[1], rgb(colors[6], colors[7], colors[8]));*/
	
	float W1, W2, W3;

	float P1x = vertices[0], P1y = vertices[1];
	float P2x = vertices[3], P2y = vertices[4];
	float P3x = vertices[6], P3y = vertices[7];

	float d = (P2y - P3y)*(P1x - P3x) + (P3x - P2x)*(P1y - P3y);

	const float EPSILON = std::numeric_limits<float>::epsilon();

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			W1 = float((P2y - P3y)*(j - P3x) + (P3x - P2x)*(i - P3y)) / d;
			W2 = float((P3y - P1y)*(j - P3x) + (P1x - P3x)*(i - P3y)) / d;
			W3 = 1 - W1 - W2;

		
			if (W1 > -EPSILON &&
				W2 > -EPSILON &&
				W3 > -EPSILON ) {
				screen[0](j, i) = utils::clamp((float(W1 * colors[0]) + float(W2 * colors[3]) + float(W3 * colors[6])) / float(W1 + W2 + W3)); 
				screen[1](j, i) = utils::clamp((float(W1 * colors[1]) + float(W2 * colors[4]) + float(W3 * colors[7])) / float(W1 + W2 + W3));
				screen[2](j, i) = utils::clamp((float(W1 * colors[2]) + float(W2 * colors[5]) + float(W3 * colors[8])) / float(W1 + W2 + W3));
			}
			
			//std::cout << (int)screen[0](j, i) << " "  << W1 << " " << W2 << " " << W3  << " " << " | " << float(W1 * colors[0]) + float(W2 * colors[3]) + float(W3 * colors[6]) << " " << float(W1 * colors[1]) + float(W2 * colors[4]) + float(W3 * colors[7]) << " " << float(W1 * colors[2]) + float(W2 * colors[5]) + float(W3 * colors[8]) << std::endl;;

		}
	}




	//for (float t = 0.0f; t <= 1.0f; t += 0.1f)
	//{
	//	/*for (int ch = 0; ch < 3; ++ch)
	//		screen[ch]*/

	//	screen[0](vertices[0] * (1 - t) + vertices[3] * t,		vertices[1] * (1 - t) + vertices[4] * t) = 255;
	//	screen[1](vertices[0] * (1 - t) + vertices[3] * t,		vertices[1] * (1 - t) + vertices[4] * t) = 0;
	//	screen[2](vertices[0] * (1 - t) + vertices[3] * t,		vertices[1] * (1 - t) + vertices[4] * t) = 0;
	//	
	//	screen[0](vertices[3] * (1 - t) + vertices[6] * t,		vertices[4] * (1 - t) + vertices[7] * t) = 0;
	//	screen[1](vertices[3] * (1 - t) + vertices[6] * t,		vertices[4] * (1 - t) + vertices[7] * t) = 255;
	//	screen[2](vertices[3] * (1 - t) + vertices[6] * t,		vertices[4] * (1 - t) + vertices[7] * t) = 0;

	//	screen[0](vertices[6] * (1 - t) + vertices[0] * t,		vertices[7] * (1 - t) + vertices[1] * t) = 0;
	//	screen[1](vertices[6] * (1 - t) + vertices[0] * t,		vertices[7] * (1 - t) + vertices[1] * t) = 0;
	//	screen[2](vertices[6] * (1 - t) + vertices[0] * t,		vertices[7] * (1 - t) + vertices[1] * t) = 255;


	//	//std::cout << vertices[0] * (1 - t) + vertices[3] * t << " " << vertices[1] * (1 - t) + vertices[3] * t << " | " << vertices[0] * (1 - t) + vertices[6] * t << " " << vertices[1] * (1 - t) + vertices[7] * t << std::endl;

	//}

	







	BMP(screen[0], screen[1], screen[2], "default").save();
	
	delete[] screen;

	return 0;
}