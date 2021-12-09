#pragma once

#include "Vector.h"
#include "Matrix.h"

#define PI 3.14159265

// TODO: translate 2D matrix to 1D matrix / vector


template<typename T>
static double dot(const Vector<T>& a, const Vector<T>& b)
{
	if (a.m_size != b.m_size)
		throw std::runtime_error("vectors sizes don\'t match");
	double out = 0;

	for (int i = 0; i < a.m_size; ++i)
		out += a.m_elements[i] * b.m_elements[i];
	return out;
}

template<typename T>
static Matrix<T> applyFunction(const Matrix<T>& mat, float(*func)(float))
{
	matf out(mat.width(), mat.height());
	for (int y = 0; y < mat.height(); ++y)
		for (int x = 0; x < mat.width(); ++x)
			out(x, y) = func(mat(x, y));
	return out;
}


static std::pair<matf, matf> DFT1D(const matf& data) {

	std::pair<matf, matf> Real_Imaginary;

	Real_Imaginary.first = matf(data.width(), data.height());
	Real_Imaginary.second = matf(data.width(), data.height());

	uint N = data.width();
	uint M = data.height();

	float Resum, Imsum;

	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			Resum = 0; Imsum = 0;
			for (int n = 0; n < N; ++n)
			{
				Resum += pow(-1, n) * data(n, i) * std::cosf(-(2 * PI * j * n) / N);
				Imsum += pow(-1, n) * data(n, i) * std::sinf(-(2 * PI * j * n) / N);
			}
			Real_Imaginary.first(j, i) = Resum / N;
			Real_Imaginary.second(j, i) = Imsum / N;
		}
	}
	return Real_Imaginary;
}


static std::pair<matf, matf> DFT2D(const matf& data) {
	std::pair<matf, matf> Real_Imaginary_1D = DFT1D(data);

	std::pair<matf, matf> Real_Imaginary_2D;
	Real_Imaginary_2D.first = matf(data.width(), data.height());
	Real_Imaginary_2D.second = matf(data.width(), data.height());

	uint N = data.width();
	uint M = data.height();

	float Resum, Imsum;
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < M; ++i)
		{
			Resum = 0; Imsum = 0;
			for (int m = 0; m < M; ++m)
			{
				Resum += pow(-1, m) * (Real_Imaginary_1D.first(j, m) * std::cosf((2 * PI * i * m) / M) - Real_Imaginary_1D.second(j, m) * std::sinf(-(2 * PI * i * m) / M));
				Imsum += pow(-1, m) * (Real_Imaginary_1D.first(j, m) * std::sinf(-(2 * PI * i * m) / M) + Real_Imaginary_1D.second(j, m) * std::cosf((2 * PI * i * m) / M));
			}
			Real_Imaginary_2D.first(j, i) = Resum / M;
			Real_Imaginary_2D.second(j, i) = Imsum / M;
		}
	}
	return Real_Imaginary_2D;
}

static matf IDFT1D(const std::pair<matf, matf>& data) {
	matf result = matf(data.first.width(), data.first.height());

	uint N = result.width();
	uint M = result.height();

	float Resum, Imsum;

	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			Resum = Imsum = 0;
			for (int n = 0; n < N; ++n)
			{
				Resum += data.first(n, i) * cosf(2 * PI * j * n / N) - data.second(n, i) * sinf(2 * PI * j * n / N);
				Imsum += data.first(n, i) * sinf(2 * PI * j * n / N) + data.second(n, i) * cosf(2 * PI * j * n / N);
			}
			result(j, i) = pow(-1, j) * (Resum + Imsum);
		}
	}
	return result;
}
static matf IDFT2D(const std::pair<matf, matf>& data) {
	matf result = matf(data.first.width(), data.first.height());

	uint N = result.width();
	uint M = result.height();

	float Resum, Imsum;


	std::pair<matf, matf> inverse_Real_Imaginary_1D;
	inverse_Real_Imaginary_1D.first = matf(data.first.width(), data.first.height());
	inverse_Real_Imaginary_1D.second = matf(data.first.width(), data.first.height());
	// inverse 1D calculations
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			Resum = Imsum = 0;
			for (int n = 0; n < N; ++n)
			{
				Resum += data.first(n, i) * cosf(2 * PI * j * n / N) - data.second(n, i) * sinf(2 * PI * j * n / N);
				Imsum += data.first(n, i) * sinf(2 * PI * j * n / N) + data.second(n, i) * cosf(2 * PI * j * n / N);
			}
			inverse_Real_Imaginary_1D.first(j, i) = Resum;
			inverse_Real_Imaginary_1D.second(j, i) = Imsum;
		}
	}
	////

	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < M; ++i)
		{
			Resum = Imsum = 0;
			for (int m = 0; m < M; ++m)
			{
				Resum += inverse_Real_Imaginary_1D.first(j, m) * cosf(2 * PI * i * m / M) - inverse_Real_Imaginary_1D.second(j, m) * sinf(2 * PI * i * m / M);
				Imsum += inverse_Real_Imaginary_1D.first(j, m) * sinf(2 * PI * i * m / M) + inverse_Real_Imaginary_1D.second(j, m) * cosf(2 * PI * i * m / M);
			}
			result(j, i) = pow(-1, j + i) * (Resum + Imsum);
		}
	}
	return result;
}