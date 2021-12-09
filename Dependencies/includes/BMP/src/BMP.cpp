#include "BMP.h"

const rgb rgb::RED(255, 0, 0);
const rgb rgb::GREEN(0, 255, 0);
const rgb rgb::BLUE(0, 0, 255);
const rgb rgb::YELLOW(255, 255, 0);
const rgb rgb::PINK(255, 0, 255);
const rgb rgb::CYAN(0, 255, 255);
const rgb rgb::WHITE(255, 255, 255);
const rgb rgb::BLACK(0, 0, 0);
const rgb rgb::GRAY(128, 128, 128);



BMP::BMP()
{
	
}

BMP::BMP(std::string filename)
	: m_filename(filename)
{
	fileToStructs(filename);
}

BMP::BMP(const BMP& other)
{
	m_filename = other.m_filename;
	data_line_size = other.data_line_size;
	padding_size = other.padding_size;

	m_channels = other.m_channels;
	m_matrix_channels = other.m_matrix_channels;


	header = other.header;
	info = other.info;

	mat = new Matrix<BYTE>[m_matrix_channels];

	for (int k = 0; k < m_matrix_channels; ++k)
		mat[k] = other.mat[k];
}


BMP& BMP::operator=(const BMP& other)
{
	if(this == &other)
		return *this;

	m_filename = other.m_filename;
	data_line_size = other.data_line_size;
	padding_size = other.padding_size;

	m_channels = other.m_channels;
	m_matrix_channels = other.m_matrix_channels;


	header = other.header;
	info = other.info;

	if (mat != nullptr)
		delete[] mat;
	mat = new Matrix<BYTE>[m_matrix_channels];

	for (int k = 0; k < m_matrix_channels; ++k)
		mat[k] = other.mat[k];
}



BMP::BMP(DWORD width, DWORD height, rgb fillcolor, std::string filename, BMPFormat format)
	:	m_filename(filename)
{
	m_channels = int(format) / 8;
	if (format == 16)
		m_matrix_channels = 3;
	else
		m_matrix_channels = m_channels;

	data_line_size = (((int(format) * width) + 31) & ~31) >> 3;
	padding_size = data_line_size - width * m_channels;

	header.bmp_type = 0x4D42;
	header.file_size = 14 + 50 + data_line_size * height;
	header.reserv_1 = 0;
	header.reserv_2 = 0;
	header.data_position = 0x36;

	info.info_structure_size = 0x28;
	info.m_width = width;
	info.m_height = height;
	info.planes = 1;
	info.bits_per_pixel = format;
	info.compression = 0;
	info.size_image = data_line_size * height + 2/*file ending*/;
	info.pixel_per_meter_X = 2834;
	info.pixel_per_meter_Y = 2834;
	info.color_table_size = 0;
	info.color_important = 0;

	BYTE arr[4] = { fillcolor.r, fillcolor.g, fillcolor.b, 0 };

	mat = new Matrix<BYTE>[m_matrix_channels];
	for (int i = 0; i < m_matrix_channels; ++i)
		mat[i] = Matrix<BYTE>(info.m_width, info.m_height, arr[i]);
}


// TODO: function to load basic info instead of copying stuff

BMP::BMP(const Matrix<BYTE>& grayscale, std::string filename, BMPFormat format)
	: m_filename(filename)
{
	m_channels = int(format) / 8;
	if (format == 16)
		m_matrix_channels = 3;
	else
		m_matrix_channels = m_channels;

	uint width = grayscale.width();
	uint height = grayscale.height();


	data_line_size = (((int(format) * width) + 31) & ~31) >> 3;
	padding_size = data_line_size - width * m_channels;

	header.bmp_type = 0x4D42;
	header.file_size = 14 + 50 + data_line_size * height;
	header.reserv_1 = 0;
	header.reserv_2 = 0;
	header.data_position = 0x36;

	info.info_structure_size = 0x28;
	info.m_width = width;
	info.m_height = height;
	info.planes = 1;
	info.bits_per_pixel = format;
	info.compression = 0;
	info.size_image = data_line_size * height + 2/*file ending*/;
	info.pixel_per_meter_X = 2834;
	info.pixel_per_meter_Y = 2834;
	info.color_table_size = 0;
	info.color_important = 0;


	mat = new Matrix<BYTE>[m_matrix_channels];
	mat[0] = Matrix<BYTE>(width, height);
	mat[1] = Matrix<BYTE>(width, height);
	mat[2] = Matrix<BYTE>(width, height);
	if (m_matrix_channels == 4)
		mat[3] = Matrix<BYTE>(width, height);

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			mat[0](j, i) = grayscale(j, i);
			mat[1](j, i) = grayscale(j, i);
			mat[2](j, i) = grayscale(j, i);
			if(m_matrix_channels == 4)
				mat[3](j, i) = 255;
		}
	}
}

BMP::BMP(const Matrix<BYTE>& r, const Matrix<BYTE>& g, const Matrix<BYTE>& b, std::string filename, BMPFormat format)
	: m_filename(filename)
{
	m_channels = int(format) / 8;
	if (format == 16)
		m_matrix_channels = 3;
	else
		m_matrix_channels = m_channels;

	if (r.width() != g.width() && r.width() != g.width() ||
		r.height() != g.height() && r.height() != g.height())
		throw std::runtime_error("matrices are not equal רע sizes");

	uint width = r.width();
	uint height = r.height();


	data_line_size = (((int(format) * width) + 31) & ~31) >> 3;
	padding_size = data_line_size - width * m_channels;

	header.bmp_type = 0x4D42;
	header.file_size = 14 + 50 + data_line_size * height;
	header.reserv_1 = 0;
	header.reserv_2 = 0;
	header.data_position = 0x36;

	info.info_structure_size = 0x28;
	info.m_width = width;
	info.m_height = height;
	info.planes = 1;
	info.bits_per_pixel = format;
	info.compression = 0;
	info.size_image = data_line_size * height + 2/*file ending*/;
	info.pixel_per_meter_X = 2834;
	info.pixel_per_meter_Y = 2834;
	info.color_table_size = 0;
	info.color_important = 0;


	mat = new Matrix<BYTE>[m_matrix_channels];
	mat[0] = Matrix<BYTE>(width, height);
	mat[1] = Matrix<BYTE>(width, height);
	mat[2] = Matrix<BYTE>(width, height);
	if (m_matrix_channels == 4)
		mat[3] = Matrix<BYTE>(width, height);

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			mat[0](j, i) = r(j, i);
			mat[1](j, i) = g(j, i);
			mat[2](j, i) = b(j, i);
			if (m_matrix_channels == 4)
				mat[3](j, i) = 255;
		}
	}
}


BMP::~BMP()
{
	if(mat != nullptr)
		delete[] mat;
}


void BMP::load(std::string filename)
{
	fileToStructs(filename);
}
void BMP::saveAs(std::string newfilename)
{
	structsToFile(newfilename);
}
void BMP::save()
{
	structsToFile(m_filename);
}


rgb BMP::getPixel(uint x, uint y)
{
	return rgb(mat[0](x, y), mat[1](x, y), mat[2](x, y));
}
void BMP::setPixel(uint x, uint y, BYTE r, BYTE g, BYTE b)
{
	mat[0](x, y) = r;	mat[1](x, y) = g;	mat[2](x, y) = b;
}


Matrix<BYTE>* BMP::at(uint channel)
{
	if (channel < m_matrix_channels)
		return &mat[channel];
	return nullptr;
}



/////// private:

void BMP::fileToStructs(std::string filename)
{
	std::string name = filename + ".bmp";
	FILE* file = fopen(name.c_str(), "rb");
	if (!file)
		throw std::runtime_error("can't open file");

	m_filename = filename;

	fread(&header.bmp_type, sizeof(WORD), 1, file);
	fread(&header.file_size, sizeof(DWORD), 1, file);
	fread(&header.reserv_1, sizeof(WORD), 1, file);
	fread(&header.reserv_2, sizeof(WORD), 1, file);
	fread(&header.data_position, sizeof(DWORD), 1, file);


	fread(&info.info_structure_size, sizeof(DWORD), 1, file);
	fread(&info.m_width, sizeof(DWORD), 1, file);
	fread(&info.m_height, sizeof(DWORD), 1, file);
	fread(&info.planes, sizeof(WORD), 1, file);
	fread(&info.bits_per_pixel, sizeof(WORD), 1, file);
	fread(&info.compression, sizeof(DWORD), 1, file);
	fread(&info.size_image, sizeof(DWORD), 1, file);
	fread(&info.pixel_per_meter_X, sizeof(LONG), 1, file);
	fread(&info.pixel_per_meter_Y, sizeof(LONG), 1, file);
	fread(&info.color_table_size, sizeof(DWORD), 1, file);
	fread(&info.color_important, sizeof(DWORD), 1, file);


	if (header.bmp_type == 0x4D42)
	{
		if (info.info_structure_size == 0x28)
		{
			// BMP ver. 3
		}
		else
		{
			// BMP not 3 ver.
		}

	}
	m_channels = info.bits_per_pixel / 8;


	if (info.bits_per_pixel == 16)
		m_matrix_channels = 3;
	else if (info.bits_per_pixel > 16)
		m_matrix_channels = m_channels;
	else
		throw std::runtime_error("images with this color depth are not supported");


	data_line_size = (((info.bits_per_pixel * width) + 31) & ~31) >> 3;
	
	padding_size = data_line_size - width * m_channels;



	mat = new Matrix<BYTE>[m_matrix_channels];
	for (int i = 0; i < m_matrix_channels; ++i)
		mat[i] = Matrix<BYTE>(info.m_width, info.m_height);


	fseek(file, header.data_position, SEEK_SET);

	DWORD color = 0;
	BYTE zero = 0;
	for (int i = 0; i < info.m_height; ++i)
	{
		for (int j = 0; j < (data_line_size - padding_size) / m_channels; ++j)
		{
			fread(&color, sizeof(BYTE), m_channels, file);

			if (info.bits_per_pixel == 16) {
				mat[0](j, i) = ((color >> 10) & 0x1F) << 3;
				mat[1](j, i) = ((color >> 5) & 0x1F) << 3;
				mat[2](j, i) = ((color >> 0) & 0x1F) << 3;
			}
			else
				for (int k = 0; k < m_channels; ++k)
					mat[k](j, i) = (color >> ((m_channels - 1) * 8 - k * 8)) & 0xFF;
			
		}
		for (int k = 0; k < padding_size; ++k)
			fread(&zero, sizeof(BYTE), 1, file);
	}
	fclose(file);
}
void BMP::structsToFile(std::string filename)
{
	std::string name = filename + ".bmp";
	FILE* file = fopen(name.c_str(), "wb");
	if (!file)
		throw std::runtime_error("can't open file");

	fwrite(&header.bmp_type, sizeof(WORD), 1, file);
	fwrite(&header.file_size, sizeof(DWORD), 1, file);
	fwrite(&header.reserv_1, sizeof(WORD), 1, file);
	fwrite(&header.reserv_2, sizeof(WORD), 1, file);
	fwrite(&header.data_position, sizeof(DWORD), 1, file);


	fwrite(&info.info_structure_size, sizeof(DWORD), 1, file);
	fwrite(&info.m_width, sizeof(DWORD), 1, file);
	fwrite(&info.m_height, sizeof(DWORD), 1, file);
	fwrite(&info.planes, sizeof(WORD), 1, file);
	fwrite(&info.bits_per_pixel, sizeof(WORD), 1, file);
	fwrite(&info.compression, sizeof(DWORD), 1, file);
	fwrite(&info.size_image, sizeof(DWORD), 1, file);
	fwrite(&info.pixel_per_meter_X, sizeof(LONG), 1, file);
	fwrite(&info.pixel_per_meter_Y, sizeof(LONG), 1, file);
	fwrite(&info.color_table_size, sizeof(DWORD), 1, file);
	fwrite(&info.color_important, sizeof(DWORD), 1, file);


	fseek(file, header.data_position, SEEK_SET);
	BYTE zero = 0x00;
	WORD color = 0x00;
	for (int i = 0; i < info.m_height; ++i)
	{
		for (int j = 0; j < (data_line_size - padding_size) / m_channels; ++j)
		{
			if (info.bits_per_pixel == 16) 
			{
				color = 0x00;
				color += ((mat[0](j, i) >> 3) & 0x1F) << 10;
				color += ((mat[1](j, i) >> 3) & 0x1F) << 5;
				color += ((mat[2](j, i) >> 3) & 0x1F) << 0;
				fwrite(&color, sizeof(WORD), 1, file);
			}
			else
			{
				for (int k = 0; k < 3; ++k)
					fwrite(&mat[3 - k - 1](j, i), sizeof(BYTE), 1, file);
				if(m_channels == 4)
					fwrite(&mat[m_channels - 1](j, i), sizeof(BYTE), 1, file);
			}
		}
		for(int k = 0; k < padding_size; ++k)
			fwrite(&zero, sizeof(BYTE), 1, file);
	}
	color = 0x00;
	for (int k = 0; k < 2; ++k)
		fwrite(&color, sizeof(BYTE), 1, file);

	fclose(file);
}