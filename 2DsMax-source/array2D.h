#pragma once
template <class T>
class Array2D
{
protected:
	T* data;
	UINT w, h, size;
	float diagonal;
public:
	static class Iterator
	{
		T* item;
	public:
		Iterator(T* t)
		{
			item = t;
		}
		Iterator operator++()
		{
			Iterator res = *this;
			item++;
			return res;
		}
		bool operator!=(Iterator iter)
		{
			return item != iter.item;
		}
		T& operator*()
		{
			return *item;
		}
	};
	Array2D(UINT w, UINT h)
	{
		size = w * h;
		data = new T[size];
		this->w = w;
		this->h = h;
		diagonal = sqrt(w * w + h * h);
	}
	~Array2D()
	{
		delete[] data;
	}
	T& operator[](UINT i)
	{
		return data[i];
	}
	T& get(long x, long y)
	{
		return data[x + y * w];
	}
	UINT getIndex(UINT x, UINT y)
	{
		return x + y * w;
	}
	T* getArr()
	{
		return data;
	}
	UINT getW() const
	{
		return w;
	}
	UINT getH() const
	{
		return h;
	}
	UINT getSize() const
	{
		return size;
	}
	float getDiagonal() const
	{
		return diagonal;
	}
	Iterator begin()
	{
		return Iterator(data);
	}
	Iterator end()
	{
		return Iterator(data + size);
	}
};