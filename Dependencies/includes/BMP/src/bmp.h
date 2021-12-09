#pragma once
#include <iostream>
#include <string>
#include "src/math.h"

typedef unsigned char BYTE;		// 8 bit
typedef unsigned short WORD;	// 16 bit
typedef unsigned int DWORD;		// 32 bit
typedef int LONG;				// 32 bit

enum BMPFormat
{
	BMP8 = 8,
	BMP16 = 16,
	BMP24 = 24,
	BMP32 = 32
};

struct rgb
{
	rgb(BYTE r = 0, BYTE g = 0, BYTE b = 0)
		: r(r), g(g), b(b)
	{
	}
	BYTE r = 0, g = 0, b = 0;

	static const rgb RED;
	static const rgb GREEN;
	static const rgb BLUE;
	static const rgb YELLOW;
	static const rgb PINK;
	static const rgb CYAN;
	static const rgb WHITE;
	static const rgb BLACK;
	static const rgb GRAY;
};


struct rgba : public rgb
{
	rgba(BYTE r = 0, BYTE g = 0, BYTE b = 0, BYTE a = 255)
		: rgb(r, g, b), a(a)
	{

	}
	BYTE a = 255;
};


// BMP 16 24 32
class BMP
{
public:
	BMP();
	BMP(std::string filename);
	BMP(DWORD width, DWORD height, rgb fillcolor, std::string filename, BMPFormat format = BMP24);
	

	BMP(const Matrix<BYTE>& grayscale, std::string filename = "default", BMPFormat format = BMP24);
	BMP(const Matrix<BYTE>& r, const Matrix<BYTE>& g, const Matrix<BYTE>& b, std::string filename, BMPFormat format = BMP24);

	~BMP();

	BMP(const BMP& other);
	BMP& operator=(const BMP& other);

	void load(std::string filename);
	void saveAs(std::string newfilename);
	void save();

	const DWORD& width = info.m_width;
	const DWORD& height = info.m_height;
	const uint& channels = m_channels;

	//friend std::ostream& operator<<(std::ostream& os, const BMP& bmp);
	
	rgb  getPixel(uint x, uint y);
	void setPixel(uint x, uint y, BYTE r = 0, BYTE g = 0, BYTE b = 0);

	Matrix<BYTE>* at(uint channel);

private:
	void fileToStructs(std::string filename);
	void structsToFile(std::string filename);



private:
	std::string m_filename;

	//uint data_stride = 0;
	int data_line_size = 0;
	int padding_size = 0;

	uint m_channels = 0;
	uint m_matrix_channels = 0;

	//uint header_size = 14;


	// Size 14 bytes
	struct
	{
		WORD bmp_type = 0;			// 0x00
		DWORD file_size = 0;		// 0x02
		WORD reserv_1 = 0;			// 0x06
		WORD reserv_2 = 0;			// 0x08
		DWORD data_position = 0;	// 0x0A
	} header;



	// version 3 - 32bit  info field
	struct
	{
		DWORD info_structure_size = 0;	// 0x00
		DWORD m_width = 0;				// 0x04
		DWORD m_height = 0;				// 0x08
		WORD planes = 1;				// 0x0C
		WORD bits_per_pixel = 0;		// 0x0E
		DWORD compression = 0;			// 0x10
		DWORD size_image = 0;			// 0x14
		LONG pixel_per_meter_X = 0;		// 0x18
		LONG pixel_per_meter_Y = 0;		// 0x1C
		DWORD color_table_size = 0;		// 0x20
		DWORD color_important = 0;		// 0x24
	} info;

	//data matrices
	Matrix<BYTE> *mat = nullptr;

	// mat of 4 is r g b a
	// mat of 3 is rgb888 or rgb555
};