/*#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <math.h>
#include <forward_list>
#include "array2D.h"
struct PointInfo
{
	Gdiplus::Color color;
	ULONG polyline;
	PointInfo()
	{
		polyline = 0;
	}
	PointInfo(Gdiplus::Color color, ULONG polyline)
	{
		this->color = color;
		this->polyline = polyline;
	}
	friend bool operator==(const PointInfo& a, const PointInfo& b)
	{
		return a.color.GetValue() == b.color.GetValue() and a.polyline == b.polyline;
	}
	friend bool operator!=(const PointInfo& a, const PointInfo& b)
	{
		return a.color.GetValue() != b.color.GetValue() or a.polyline != b.polyline;
	}
};
struct Pixel : std::forward_list<PointInfo>
{
	Gdiplus::Color getColor()
	{
		Gdiplus::Color res = (*begin()).color;
		res = Gdiplus::Color::MakeARGB(255, res.GetR(), res.GetG(), res.GetB());
		for (PointInfo& pi : *this)
		{
			if (pi.color.GetA() == 255)
			{
				return res;
			}
			double a = res.GetA() / 255.0;
			double a1 = pi.color.GetA() / 255.0;
			res = Gdiplus::Color::MakeARGB(
				255,
				(size_t)(res.GetR() * a + pi.color.GetR() * a1) >> 1,
				(size_t)(res.GetG() * a + pi.color.GetG() * a1) >> 1,
				(size_t)(res.GetB() * a + pi.color.GetB() * a1) >> 1
			);
		}
		return res;
	}
	void push(const PointInfo& p)
	{
		for (PointInfo pi : *this)
		{
			if (pi.polyline == p.polyline)
				return;
		}
		push_front(p);
	}
};
struct Data
{
	size_t pixelCount;
	UINT32 pressure;
	size_t polyline;

	DOUBLE doubleWidth;
	UINT32 intWidth;
	BYTE byteWidth;

	POINT start;
	POINT end;
	DOUBLE distance;

	Gdiplus::Color color;

	void startSet(const HWND& hwnd)
	{
		GetCursorPos(&start);
		ScreenToClient(hwnd, &start);
	}
	void set(const HWND& hwnd, size_t polyline, Gdiplus::Color color)
	{
		doubleWidth = 9;
		POINTER_PEN_INFO pointerPenInfp;
		for (UINT32 i = 200; i != 0; i--)
			if (GetPointerPenInfo(i, &pointerPenInfp))
			{
				doubleWidth = pointerPenInfp.pressure / 32.0;
				break;
			}
		intWidth = doubleWidth;
		byteWidth = (doubleWidth - intWidth) * 255;
		GetCursorPos(&end);
		ScreenToClient(hwnd, &end);
		distance = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
		pixelCount = distance * M_PI * (doubleWidth * doubleWidth / 4);
		this->polyline = polyline;
		this->color = color;
	}
};*/