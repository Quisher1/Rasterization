#include "Matrix.h"
#include "Vector.h"

template<typename T>
Matrix<T>::Matrix<T>()
{

}

template<typename T>
Matrix<T>::Matrix(uint width, uint height, const T* source)
	: m_width(width), m_height(height), m_size(width * height)
{
	m_elements = new T[width * height];

	memcpy(m_elements, source, sizeof(T) * width * height);
}


template<typename T>
Matrix<T>::Matrix(uint width, uint height, T defaultValue)
	: m_width(width), m_height(height), m_size(width * height)
{
	m_elements = new T[width * height];
	for (int i = 0; i < width * height; ++i)
		m_elements[i] = defaultValue;
}


template<typename T>
Matrix<T>::Matrix<T>(const Matrix<T>& mat)
{
	m_size = mat.m_size;
	m_width = mat.m_width;
	m_height = mat.m_height;
	m_elements = new T[m_size];
	//for (int i = 0; i < m_size; ++i)
	//	m_elements[i] = mat.m_elements[i];
	memcpy(m_elements, mat.m_elements, sizeof(T) * m_size);
}

template<typename T>
Matrix<T> Matrix<T>::operator=(const Matrix<T>& mat)
{
	if (this == &mat)
		return *this;

	if (m_height != mat.m_height || m_width != mat.m_width)
	{
		m_size = mat.m_size;
		m_height = mat.m_height;
		m_width = mat.m_width;
		if(m_elements != nullptr)
			delete[] m_elements;
		m_elements = new T[m_size];

	}
	//for (int i = 0; i < m_size; ++i)
	//	m_elements[i] = mat.m_elements[i];
	memcpy(m_elements, mat.m_elements, sizeof(T) * m_size);
	return *this;
}

template<typename T>
Matrix<T>::~Matrix<T>()
{
	if (m_elements != nullptr)
	{
		delete[] m_elements;
		m_elements = nullptr;
	}
}


template<typename T>
T& Matrix<T>::operator()(uint x, uint y) const
{
	if (x < m_width && y < m_height)
		return m_elements[y * m_width + x];
	throw std::runtime_error("index out of range");
}

template<typename T>
Matrix<T>& Matrix<T>::multiply(const double value)
{
	for (int i = 0; i < m_size; ++i)
		m_elements[i] *= value;
	return *this;
}
template<typename T>
Matrix<T>& Matrix<T>::divide(const double value)
{
	for (int i = 0; i < m_size; ++i)
		m_elements[i] /= value;
	return *this;
}
template<typename T>
Matrix<T>& Matrix<T>::add(const Matrix<T>& other)
{
	if (m_width != other.m_width || m_height != other.m_height)
		throw std::runtime_error("matrix sizes don\'t match");

	for (int i = 0; i < m_size; ++i)
		m_elements[i] += other.m_elements[i];
	return *this;
}
template<typename T>
Matrix<T>& Matrix<T>::subtract(const Matrix<T>& other)
{
	if (m_width != other.m_width || m_height != other.m_height)
		throw std::runtime_error("matrix sizes don\'t match");
	for (int i = 0; i < m_size; ++i)
		m_elements[i] -= other.m_elements[i];
	return *this;
}


template<typename T>
Matrix<T> Matrix<T>::multiply(const Matrix<T>& mat)
{
	if (m_width != mat.m_height)
		throw std::runtime_error("matrix sizes don\'t match");
	double sum = 0;
	uint i, j, k;

	Matrix<T> res(mat.m_width, m_height);

	for (i = 0; i < m_height; ++i)
	{
		for (j = 0; j < mat.m_width; ++j)
		{
			sum = 0;
			for (k = 0; k < mat.m_height; ++k)
			{
				sum += m_elements[i * m_width + k] * mat.m_elements[k * mat.m_width + j];
			}
			res.m_elements[i * res.m_width + j] = sum;
		}
	}
	return res;
}
/*template<typename T>
Matrix<T>& Matrix<T>::divide(const Matrix<T>& other)
{
	if (m_width != other.m_width || m_height != other.m_height)
		throw std::runtime_error("matrix sizes don\'t match");
	for (int i = 0; i < m_size; ++i)
		m_elements[i] /= other.m_elements[i];
	return *this;
}*/

template<typename T>
Vector<T> Matrix<T>::multiply(const Vector<T>& vec)
{
	if (m_width != vec.m_size)
		throw std::runtime_error("matrix sizes don\'t match with vector");

	Vector<T> out(m_height);
	double sum = 0;
	uint i, j, k;
	for (i = 0; i < m_height; ++i)
	{
		sum = 0;
		for (k = 0; k < vec.m_size; ++k)
		{
			sum += m_elements[i * m_width + k] * vec.m_elements[k];
		}
		out.m_elements[i] = sum;
	}
	return out;
	/*for (i = 0; i < vec.m_size; ++i) {
		sum = 0;
		for (j = 0; j < m_height; ++j)
		{
			sum += m_elements[i * m_width + j] * vec.m_elements[j];
		}
		out.m_elements[i] = sum;
	}*/
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const float value)
{
	return multiply(value);
}
template<typename T>
Matrix<T>& Matrix<T>::operator/=(const float value)
{
	return divide(value);
}
template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& mat)
{
	return add(mat);
}
template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& mat)
{
	return subtract(mat);
}
/*template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& mat)
{
	return multiply(mat);
}*/
/*template<typename T>
Matrix<T>& Matrix<T>::operator/=(const Matrix<T>& mat)
{
	return divide(mat);
}*/


template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& mat)
{
	return multiply(mat);
}
template<typename T>
Vector<T> Matrix<T>::operator*(const Vector<T>& vec)
{
	return multiply(vec);
}


//template<typename T>
//Vector<T> Matrix<T>::operator*(const Vector<T>& vec)
//{
//	/*if (m_width != vec.size())
//		throw std::runtime_error("matrix sizes don\'t match with vector");
//	Vector<T> out(vec.size());
//	double sum = 0;*/


//	/*for (int i = 0; i < vec.m_size; ++i) {
//		sum = 0;
//		for (int j = 0; j < m_height; ++j)
//		{
//			sum += m_elements[i * m_width + j] * vec.m_elements[j];
//		}
//		out.m_elements[i] = sum;
//	}*/
//	return Vector<T>(1);
//}


template<typename T>
void Matrix<T>::identity()
{
	for (int i = 0; i < m_height; ++i)
		for (int j = 0; j < m_width; ++j)
			if (i == j)
				m_elements[i * m_width + j] = 1;
			else
				m_elements[i * m_width + j] = 0;
}

template<typename T>
Matrix<T> Matrix<T>::transpose()
{
	Matrix<T> res(m_height, m_width, 0.0);

	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			//res.m_elements[j * m_height + i] = m_elements[i * m_width + j];
			res(i, j) = this->operator()(j, i);
		}
	}

	return res;
}


template class Matrix<char>;
template class Matrix<unsigned char>;
template class Matrix<int>;
template class Matrix<unsigned int>;
template class Matrix<float>;
template class Matrix<double>;
