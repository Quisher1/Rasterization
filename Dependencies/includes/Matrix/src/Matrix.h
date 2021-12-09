#pragma once

#include <iostream>

#include "Vector.h"


typedef unsigned int uint;

template<typename T>
class Matrix
{
public:
	Matrix<T>();
	Matrix<T>(uint width, uint height, const T* source);
	Matrix<T>(uint width, uint height, T defaultValue = 0);

	Matrix<T>(const Matrix<T>& vmatec);
	Matrix<T> operator=(const Matrix<T>& mat);

	~Matrix<T>();

	inline T& operator()(uint x, uint y) const;
	//Vector<T>& (uint rowIndex) const;

	inline uint width() const { return m_width; }
	inline uint height() const { return m_height; }


	Matrix<T>& multiply(const double value);
	Matrix<T>& divide(const double value);
	Matrix<T>& add(const Matrix<T>& other);
	Matrix<T>& subtract(const Matrix<T>& other);


	Matrix<T> multiply(const Matrix<T>& mat);
	Vector<T> multiply(const Vector<T>& vec);


	template<typename T>
	friend Matrix<T> operator+(Matrix<T> left, const Matrix<T>& right);
	template<typename T>
	friend Matrix<T> operator-(Matrix<T> left, const Matrix<T>& right);
	/*template<typename T>
	friend Matrix<T> operator*(Matrix<T> left, const Matrix<T>& right);
	template<typename T>
	friend Matrix<T> operator/(Matrix<T> left, const Matrix<T>& right);*/


	template<typename T>
	friend Matrix<T> operator*(Matrix<T> mat, const double value);
	template<typename T>
	friend Matrix<T> operator/(Matrix<T> mat, const double value);
	template<typename T>
	friend Matrix<T> operator*(const double value, Matrix<T> mat);
	template<typename T>
	friend Matrix<T> operator/(const double value, Matrix<T> mat);



	Matrix<T> operator*(const Matrix<T>& mat);
	Vector<T> operator*(const Vector<T>& vec);

	/*template<typename T>
	friend Vector<T> operator*(Matrix<T> mat, const Vector<T>& vec);*/


	Matrix<T>& operator*=(const float value);
	Matrix<T>& operator/=(const float value);
	Matrix<T>& operator+=(const Matrix<T>& mat);
	Matrix<T>& operator-=(const Matrix<T>& mat);
	/*Matrix<T>& operator*=(const Matrix<T>& mat);
	Matrix<T>& operator/=(const Matrix<T>& mat);*/



	void identity();
	Matrix<T> transpose();



	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat);

	template<typename T>
	friend Matrix<T> elementwiseMultiplication(const Matrix<T>& a, const Matrix<T>& b);

	/*template<typename>
	friend class Vector;*/

private:

	uint m_width = 0, m_height = 0;
	uint m_size = 0;


	T* m_elements = nullptr;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat)
{
	for (int i = 0; i < mat.height(); ++i)
	{
		for (int j = 0; j < mat.width(); ++j)
		{
			os << mat.m_elements[i * mat.m_width + j] << " ";
		}
		os << '\n';
	}
	return os;
}



template<typename T>
Matrix<T> operator+(Matrix<T> left, const Matrix<T>& right)
{
	return left.add(right);
}
template<typename T>
Matrix<T> operator-(Matrix<T> left, const Matrix<T>& right)
{
	return left.subtract(right);
}
template<typename T>
Matrix<T> operator*(Matrix<T> left, const Matrix<T>& right)
{
	return left.multiply(right);
}
template<typename T>
Matrix<T> operator/(Matrix<T> left, const Matrix<T>& right)
{
	return left.divide(right);
}

template<typename T>
Matrix<T> operator*(Matrix<T> mat, const double value)
{
	return mat.multiply(value);
}
template<typename T>
Matrix<T> operator/(Matrix<T> mat, const double value)
{
	return mat.divide(value);
}
template<typename T>
Matrix<T> operator*(const double value, Matrix<T> mat)
{
	return mat.multiply(value);
}
template<typename T>
Matrix<T> operator/(const double value, Matrix<T> mat)
{
	return mat.divide(value);
}


template<typename T>
Matrix<T> elementwiseMultiplication(const Matrix<T>& a, const Matrix<T>& b)
{
	if (a.m_width != b.m_width && a.m_height != b.m_height)
		throw std::runtime_error("matrix sizes don't match");

	Matrix<T> out(a.width(), a.height());

	for (int i = 0; i < out.m_height; ++i)
	{
		for (int j = 0; j < out.m_width; ++j)
		{
			out(j, i) = a(j, i) * b(j, i);
		}
	}
	return out;
}


typedef Matrix<int> mati;
typedef Matrix<float> matf;
typedef Matrix<double> matd;
