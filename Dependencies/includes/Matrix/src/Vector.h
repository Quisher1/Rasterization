#pragma once

#include <iostream>

template<typename>
class Matrix;

typedef unsigned int uint;

template<typename T>
class Vector
{
public:
	Vector<T>();
	Vector<T>(const uint size, const T* source);
	Vector<T>(const uint size, T defaultValue = 0);

	Vector<T>(const Vector<T>& vec);

	~Vector<T>();

	T& operator()(const uint index) const;
	inline uint size() const { return m_size; }


	Vector<T>& multiply(const double value);
	Vector<T>& divide(const double value);
	Vector<T>& add(const Vector<T>& other);
	Vector<T>& subtract(const Vector<T>& other);
	Vector<T>& multiply(const Vector<T>& other);
	Vector<T>& divide(const Vector<T>& other);

	template<typename T>
	friend Vector<T> operator+(Vector<T> left, const Vector<T>& right);// { return left.add(right); }
	template<typename T>
	friend Vector<T> operator-(Vector<T> left, const Vector<T>& right);// { return left.subtract(right); }
	template<typename T>
	friend Vector<T> operator*(Vector<T> left, const Vector<T>& right);// { return left.multiply(right); }
	template<typename T>
	friend Vector<T> operator/(Vector<T> left, const Vector<T>& right);// { return left.divide(right); }


	template<typename T>
	friend Vector<T> operator*(Vector<T> vec, const double value);
	template<typename T>
	friend Vector<T> operator/(Vector<T> vec, const double value);
	template<typename T>
	friend Vector<T> operator*(const double value, Vector<T> vec);
	template<typename T>
	friend Vector<T> operator/(const double value, Vector<T> vec);


	Vector<T> operator=(const Vector<T>& vec);

	Vector<T>& operator*=(const float value);
	Vector<T>& operator/=(const float value);
	Vector<T>& operator+=(const Vector<T>& vec);
	Vector<T>& operator-=(const Vector<T>& vec);
	Vector<T>& operator*=(const Vector<T>& vec);
	Vector<T>& operator/=(const Vector<T>& vec);


	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const Vector<T>& vec);

	template<typename>
	friend class Matrix;


private:
	uint m_size = 0;
	T *m_elements = nullptr;
};


template<typename T>
std::ostream& operator<< (std::ostream& os, const Vector<T>& vec)
{
	if (vec.m_size == 0)
		return os;
	for (int i = 0; i < vec.m_size - 1; ++i)
	{
		os << vec.m_elements[i] << " ";
	}
	os << vec.m_elements[vec.m_size - 1];
	return os;
}


template<typename T>
Vector<T> operator+(Vector<T> left, const Vector<T>& right)
{
	return left.add(right);
}
template<typename T>
Vector<T> operator-(Vector<T> left, const Vector<T>& right)
{
	return left.subtract(right);
}
template<typename T>
Vector<T> operator*(Vector<T> left, const Vector<T>& right)
{
	return left.multiply(right);
}
template<typename T>
Vector<T> operator/(Vector<T> left, const Vector<T>& right)
{
	return left.divide(right);
}



template<typename T>
Vector<T> operator*(Vector<T> vec, const double value)
{
	return vec.multiply(value);
}
template<typename T>
Vector<T> operator/(Vector<T> vec, const double value)
{
	return vec.divide(value);
}
template<typename T>
Vector<T> operator*(const double value, Vector<T> vec)
{
	return vec.multiply(value);
}
template<typename T>
Vector<T> operator/(const double value, Vector<T> vec)
{
	return vec.divide(value);
}

typedef Vector<int> veci;
typedef Vector<float> vecf;
typedef Vector<double> vecd;
