#include "Vector.h"

#include <stdexcept>

template<typename T>
Vector<T>::Vector<T>()
{
}

template<typename T>
Vector<T>::Vector<T>(uint size, const T* source)
	: m_size(size)
{
	m_elements = new T[size];
	for (int i = 0; i < size; ++i)
		m_elements[i] = source[i];
	//memcpy(m_elements, source, size * sizeof(T));
}

template<typename T>
Vector<T>::Vector<T>(uint size, T defaultValue)
	: m_size(size)
{
	m_elements = new T[size];
	for (int i = 0; i < size; ++i)
		m_elements[i] = defaultValue;
}

template<typename T>
Vector<T>::Vector<T>(const Vector<T>& vec)
{
	m_size = vec.m_size;
	m_elements = new T[m_size];
	for (int i = 0; i < m_size; ++i)
		m_elements[i] = vec.m_elements[i];
}

template<typename T>
Vector<T>::~Vector()
{
	if (m_elements != nullptr)
	{
		delete[] m_elements;
		m_elements = nullptr;
	}
}



template<typename T>
T& Vector<T>::operator()(const uint index) const
{
	if (index < m_size)
		return m_elements[index];
	throw std::runtime_error("index out of range");
}

template<typename T>
Vector<T>& Vector<T>::multiply(const double value)
{
	for (int i = 0; i < m_size; ++i)
		m_elements[i] *= value;
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::divide(const double value)
{
	for (int i = 0; i < m_size; ++i)
		m_elements[i] /= value;
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::add(const Vector<T>& other)
{
	if (m_size != other.m_size)
		throw std::runtime_error("vectors sizes don\'t match");

	for (int i = 0; i < m_size; ++i)
		m_elements[i] += other.m_elements[i];
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::subtract(const Vector<T>& other)
{
	if (m_size != other.m_size)
		throw std::runtime_error("vectors sizes don\'t match");


	for (int i = 0; i < m_size; ++i)
		m_elements[i] -= other.m_elements[i];
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::multiply(const Vector<T>& other)
{
	if (m_size != other.m_size)
		throw std::runtime_error("vectors sizes don\'t match");

	for (int i = 0; i < m_size; ++i)
		m_elements[i] *= other.m_elements[i];
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::divide(const Vector<T>& other)
{
	if (m_size != other.m_size)
		throw std::runtime_error("vectors sizes don\'t match");

	for (int i = 0; i < m_size; ++i)
		m_elements[i] /= other.m_elements[i];
	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator*=(const float value)
{
	return multiply(value);
}
template<typename T>
Vector<T>& Vector<T>::operator/=(const float value)
{
	return divide(value);
}
template<typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& vec)
{
	return add(vec);
}
template<typename T>
Vector<T>& Vector<T>::operator-=(const Vector<T>& vec)
{
	return subtract(vec);
}
template<typename T>
Vector<T>& Vector<T>::operator*=(const Vector<T>& vec)
{
	return multiply(vec);
}
template<typename T>
Vector<T>& Vector<T>::operator/=(const Vector<T>& vec)
{
	return divide(vec);
}




template<typename T>
Vector<T> Vector<T>::operator=(const Vector<T>& other)
{
	if (this == &other)
		return *this;

	if (m_size != other.m_size)
	{
		m_size = other.m_size;
		delete[] m_elements;
		m_elements = new T[m_size];

	}
	for (int i = 0; i < m_size; ++i)
		m_elements[i] = other.m_elements[i];
	return *this;
}


template class Vector<char>;
template class Vector<unsigned char>;
template class Vector<int>;
template class Vector<unsigned int>;
template class Vector<float>;
template class Vector<double>;
