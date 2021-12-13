#include <iostream>
#include <Windows.h>
#include <algorithm>
//#include <src/bmp.h>

int width;
int height;

int ClientWidth;
int ClientHeight;

bool gloabalRunning = true;

BITMAPINFO bitmap_info;

void* memory;


const float EPSILON = std::numeric_limits<float>::epsilon();

BYTE clamp(int color)
{
	return color > 255 ? 255 : (color < 0 ? 0 : color);
}

void setPixel(int x, int y, DWORD color)
{
	DWORD* pixels = (DWORD*)memory;
	pixels[y * ClientWidth + x] = color;
}

void drawLine(int x1, int y1, int x2, int y2, DWORD color) {
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

void ClearScreen(DWORD color)
{
	DWORD* pixels = (DWORD*)memory;
	for (int i = 0; i < ClientHeight * ClientWidth; ++i) {
		*pixels = color;
		++pixels;
	}
}



LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (uMsg)
	{
	case WM_SIZE:
	{
		width = LOWORD(lParam);
		height = HIWORD(lParam);

		//RECT rect;
		//GetClientRect(hwnd, &rect);
		//
		//ClientWidth = rect.right - rect.left;
		//ClientHeight = rect.bottom - rect.top;

		std::cout << width << " " << height << " " << ClientWidth << " " << ClientHeight << std::endl;

	}break;

	case WM_CLOSE:
	{
		gloabalRunning = false;
	}break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}break;

	/*case WM_PAINT:
	{
		std::cout << "paint" << std::endl;






	}break;*/

	default:
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}

	return result;
}



void drawTriangle(const float* vertices, const float* color)
{
	DWORD* pixels = (DWORD*)memory;
	float P1x = vertices[0], P1y = vertices[1];
	float P2x = vertices[3], P2y = vertices[4];
	float P3x = vertices[6], P3y = vertices[7];
	float W1, W2, W3;

	float d = (P2y - P3y)*(P1x - P3x) + (P3x - P2x)*(P1y - P3y);

	for (int i = 0; i < ClientHeight; ++i)
	{
		for (int j = 0; j < ClientWidth; ++j)
		{
			W1 = float((P2y - P3y)*(j - P3x) + (P3x - P2x)*(i - P3y)) / d;
			W2 = float((P3y - P1y)*(j - P3x) + (P1x - P3x)*(i - P3y)) / d;
			W3 = 1 - W1 - W2;


			if (W1 > -EPSILON &&
				W2 > -EPSILON &&
				W3 > -EPSILON) {
				pixels[i * ClientWidth + j] = DWORD(clamp((float(W1 * color[0]) + float(W2 * color[3]) + float(W3 * color[6])) / float(W1 + W2 + W3)) << 16 |
					clamp((float(W1 * color[1]) + float(W2 * color[4]) + float(W3 * color[7])) / float(W1 + W2 + W3)) << 8 |
					clamp((float(W1 * color[2]) + float(W2 * color[5]) + float(W3 * color[8])) / float(W1 + W2 + W3)));
			}
		}
	}
}



void drawTriangles(const float* vertices, const float* color, unsigned int vertexCount)
{
	DWORD* pixels = (DWORD*)memory;
	int i, j, k;
	int x, y;
	float left, right, bottom, top;

	float d;
	float W1, W2, W3;

	float wsum;

	if (vertexCount % 3 != 0)
		return;

	for (k = 0; k < vertexCount * 3; k += 9)
	{
		left = std::fminf(vertices[k + 0], std::fminf(vertices[k + 3], vertices[k + 6]));
		right = std::fmaxf(vertices[k + 0], std::fmaxf(vertices[k + 3], vertices[k + 6]));
		bottom = std::fminf(vertices[k + 1], std::fminf(vertices[k + 4], vertices[k + 7]));
		top = std::fmaxf(vertices[k + 1], std::fmaxf(vertices[k + 4], vertices[k + 7]));

		// x 1 = +0
		// x 2 = +3
		// x 3 = +6

		// y 1 = +1
		// y 2 = +4
		// y 3 = +7

		// (P2y - P3y)*(P1x - P3x) + (P3x - P2x)*(P1y - P3y);

		d = (vertices[k + 4] - vertices[k + 7]) * (vertices[k + 0] - vertices[k + 6]) + (vertices[k + 6] - vertices[k + 3]) * (vertices[k + 1] - vertices[k + 7]);

		for (y = bottom; y <= top; ++y)
		{
			for (x = left; x <= right; ++x)
			{
				//W1 = float((P2y - P3y)*(j - P3x) + (P3x - P2x)*(i - P3y)) / d;
				//W2 = float((P3y - P1y)*(j - P3x) + (P1x - P3x)*(i - P3y)) / d;

				W1 = float((vertices[k + 4] - vertices[k + 7]) * (x - vertices[k + 7]) + (vertices[k + 6] - vertices[k + 3]) * (y - vertices[k + 7])) / d;
				W2 = float((vertices[k + 7] - vertices[k + 1]) * (x - vertices[k + 7]) + (vertices[k + 0] - vertices[k + 6]) * (y - vertices[k + 7])) / d;
				W3 = 1 - W1 - W2;


				if (W1 > -EPSILON && W2 > -EPSILON && W3 > -EPSILON)
				{
					wsum = W1 + W2 + W3;
					pixels[y * ClientWidth + x] = DWORD(clamp((float(W1 * color[k + 0]) + float(W2 * color[k + 3]) + float(W3 * color[k + 6])) / wsum) << 16 |
						clamp((float(W1 * color[k + 1]) + float(W2 * color[k + 4]) + float(W3 * color[k + 7])) / wsum) << 8 |
						clamp((float(W1 * color[k + 2]) + float(W2 * color[k + 5]) + float(W3 * color[k + 8])) / wsum));
				}
			}
		}
	}


	//int left = std::fminf(0.1f, 0.2f) , right, top, bottom;
	//
	//for(i = 0; i < )
}


int main()
{

	LPCSTR CLASS_NAME = "Sample Window Class";

	WNDCLASS wc = {};
	wc.lpfnWndProc = wndProc;
	wc.hInstance = GetModuleHandle(0);
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND window = CreateWindowEx(0,
		CLASS_NAME,
		"Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(0),
		NULL);

	if (window == NULL)
		return 1;

	ShowWindow(window, SW_SHOW);

	HDC hdc = GetDC(window);

	RECT rect;
	GetClientRect(window, &rect);

	ClientWidth = rect.right - rect.left;
	ClientHeight = rect.bottom - rect.top;

	memory = VirtualAlloc(0, ClientHeight * ClientWidth * sizeof(unsigned int), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);



	/*DWORD* pixels = (DWORD*)memory;
	for (int i = 0; i < ClientHeight * ClientWidth; i+=3) {
		*pixels = 0xFF0000;
		++pixels;
		*pixels = 0x00FF00;
		++pixels;
		*pixels = 0x0000FF;
		++pixels;
	}*/

	//*pixels = 0x0000FF;

	DWORD* pixels = (DWORD*)memory;
	for (int i = 0; i < ClientHeight; ++i) {
		for (int j = 0; j < ClientWidth; ++j) {
			pixels[i * ClientWidth + j] = 0x0f0f0f;
			//++pixels;
		}
	}

	float vertices[] = // 3D
	{
		//-0.5f, -0.5f, 0.0f,
		// 0.5f, -0.5f, 0.0f,
		// 0.0f,  0.5f, 0.0f

		100, 100, 0,		// 0 1 2
		100, 200, 0,		// 3 4 5
		200, 200, 0,		// 6 7 8

		100, 100, 0,		// 0 1 2
		200, 100, 0,		// 3 4 5
		200, 200, 0			// 6 7 8
	};
	float colors[] =
	{
		255, 0, 0,			// 0 1 2
		0, 255, 0,			// 3 4 5
		255, 255, 255,		// 6 7 8


		255, 0, 0,			// 0 1 2
		0, 255, 0,			// 3 4 5
		255, 255, 255		// 6 7 8

	};

	/*float P1x = vertices[0], P1y = vertices[1];
	float P2x = vertices[3], P2y = vertices[4];
	float P3x = vertices[6], P3y = vertices[7];
	const float EPSILON = std::numeric_limits<float>::epsilon();
	float W1, W2, W3;

	float d = (P2y - P3y)*(P1x - P3x) + (P3x - P2x)*(P1y - P3y);

	for (int i = 0; i < ClientHeight; ++i)
	{
		for (int j = 0; j < ClientWidth; ++j)
		{
			W1 = float((P2y - P3y)*(j - P3x) + (P3x - P2x)*(i - P3y)) / d;
			W2 = float((P3y - P1y)*(j - P3x) + (P1x - P3x)*(i - P3y)) / d;
			W3 = 1 - W1 - W2;


			if (W1 > -EPSILON &&
				W2 > -EPSILON &&
				W3 > -EPSILON) {
				pixels[i * ClientWidth + j] = DWORD(clamp((float(W1 * colors[0]) + float(W2 * colors[3]) + float(W3 * colors[6])) / float(W1 + W2 + W3)) << 16 |
											   clamp((float(W1 * colors[1]) + float(W2 * colors[4]) + float(W3 * colors[7])) / float(W1 + W2 + W3)) << 8 |
											   clamp((float(W1 * colors[2]) + float(W2 * colors[5]) + float(W3 * colors[8])) / float(W1 + W2 + W3)));
			}
		}
	}*/




	bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
	bitmap_info.bmiHeader.biWidth = ClientWidth;
	bitmap_info.bmiHeader.biHeight = ClientHeight;
	bitmap_info.bmiHeader.biPlanes = 1;
	bitmap_info.bmiHeader.biBitCount = 32;
	bitmap_info.bmiHeader.biCompression = BI_RGB;
	while (gloabalRunning)
	{
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		drawTriangles(&vertices[0], &colors[0], 6);

		StretchDIBits(hdc, 0, 0, width, height, 0, 0, ClientWidth, ClientHeight, memory, &bitmap_info, DIB_RGB_COLORS, SRCCOPY);
		ClearScreen(0x1F1F1F);
	}


	return 0;
}