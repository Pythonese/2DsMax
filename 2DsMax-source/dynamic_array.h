#pragma once
template <class T>
class DynamicArray
{
	T* data = nullptr;
	size_t size = 0;
public:
	DynamicArray() {}
	DynamicArray(size_t size)
	{
		data = new T[size];
	}
	DynamicArray(const DynamicArray& arr)
	{
		delete[] data;
		data = new T[arr.size];
		size = arr.size;
		for (size_t i = 0; i < size; i++)
		{
			data[i] = arr[i];
		}
	}
	T& operator[](size_t i)
	{
		return data[i];
	}
	void add(size_t i, const T& t)
	{
		T* oldData = data;
		data = new T[size + 1];
		for (size_t j = 0; j < i; j++)
		{
			data[j] = oldData[j];
		}
		data[i] = t;
		for (size_t j = i; j < size; j++)
		{
			data[j + 1] = oldData[j];
		}
		size++;
		delete[] oldData;
	}
	void remove(size_t i)
	{

	}
};