/*#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include <gdiplus.h>
#include <forward_list>
#include "structures.h"
#include "gdiplusinit.h"
#include "dynamic_array.h"
namespace _2DsMax
{
	void AddLocation(Array2D<Pixel>& pixels, POINT location, POINT start, POINT end, size_t polyline, POINT delta);
	using TRANSFORM_CALLBACK = void(Array2D<Pixel>& pixels, POINT location, POINT start, POINT end, size_t polyline, POINT delta);
	enum Type
	{
		END, MOUSE_UP, DRAW, 
	};
	enum ClickType
	{
		NONE=0,
		S=1, C=1<<1, A=1<<2,
		SC=1+(1<<1), SA=1+(1<<2), CA=(1<<1)+(1<<2),
		SCA=7
	};
	struct Frame : Array2D<Pixel>
	{
		size_t polylines[1024]{};
		size_t shapes[1024]{};
		std::forward_list<size_t> selection;
		Frame* frameCopy = nullptr;
		Frame(size_t w, size_t h) : Array2D<Pixel>(w, h)
		{
			PointInfo p(Gdiplus::Color::White, 0);
			for (size_t i = 0; i < size; i++)
			{
				data[i].push(p);
			}
		}
		Frame* copy()
		{
			Frame* arr = new Frame(w >> 2, h >> 2);
			Array2D<Pixel>::Iterator iter = arr->begin();
			for (size_t i = 0; i < size; i += 4)
			{
				*iter = (*this)[i];
				++iter;
			}
			return arr;
		}
		void drawBitmap(Gdiplus::Bitmap* bitmap)
		{
			for (int i = 0; i < size; i++)
			{
				bitmap->SetPixel(i % w, i / w, data[i].getColor());
			}
		}
		void drawBitmap(Gdiplus::Bitmap* bitmap, size_t pixelCount)
		{
			static size_t lineIndex;
			++pixelCount;
			for (size_t i = ++lineIndex % pixelCount; i < size; i += pixelCount)
			{
				bitmap->SetPixel(i % w, i / w, data[i].getColor());
			}
		}
		void drawBitmapOfRects(Gdiplus::Bitmap* bitmap)
		{
			LONG w = bitmap->GetWidth(), h = bitmap->GetHeight();
			POINT p;
			for (p.y = 0; p.y < h; p.y++)
				for (p.x = 0; p.x < w; p.x++)
				{
					Gdiplus::Color color = get(p.x, p.y).getColor();
					p.y <<= 2;
					p.x <<= 2;
					bitmap->SetPixel(p.x, p.y, color);
					bitmap->SetPixel(p.x + 1, p.y, color);
					bitmap->SetPixel(p.x + 2, p.y, color);
					bitmap->SetPixel(p.x + 3, p.y, color);

					bitmap->SetPixel(p.x, p.y + 1, color);
					bitmap->SetPixel(p.x + 1, p.y + 1, color);
					bitmap->SetPixel(p.x + 2, p.y + 1, color);
					bitmap->SetPixel(p.x + 3, p.y + 1, color);

					bitmap->SetPixel(p.x, p.y + 2, color);
					bitmap->SetPixel(p.x + 1, p.y + 2, color);
					bitmap->SetPixel(p.x + 2, p.y + 2, color);
					bitmap->SetPixel(p.x + 3, p.y + 2, color);

					bitmap->SetPixel(p.x, p.y + 3, color);
					bitmap->SetPixel(p.x + 1, p.y + 3, color);
					bitmap->SetPixel(p.x + 2, p.y + 3, color);
					bitmap->SetPixel(p.x + 3, p.y + 3, color);
					p.y >>= 2;
					p.x >>= 2;
				}
		}
		void select(POINT p, ClickType type)
		{
			Pixel& pixel = get(p.x, p.y);
			if ((type & _2DsMax::S) == 0)
				deselect();
			switch (type & ~_2DsMax::S)
			{
			case _2DsMax::NONE:
				selection.push_front((*pixel.begin()).polyline);
				break;
			case _2DsMax::C:
			{
				size_t shape = shapes[(*pixel.begin()).polyline];
				for (Pixel& pi : *this)
				{
					for (PointInfo& po : pi)
					{
						if (shapes[po.polyline] == shape)
						{
							selection.push_front(po.polyline);
						}
					}
				}
				break;
			}
			case _2DsMax::A:
				for (PointInfo& po : pixel)
				{
					selection.push_front(po.polyline);
				}
				break;
			case _2DsMax::CA:
			{
				std::forward_list<size_t> shapes1;
				for (PointInfo& po : pixel)
				{
					shapes1.push_front(shapes[po.polyline]);
				}
				for (size_t shape : shapes1)
				{
					for (Pixel& pi : *this)
					{
						for (PointInfo& po : pi)
						{
							if (shapes[po.polyline] == shape)
							{
								selection.push_front(po.polyline);
							}
						}
					}
				}
				break;
			}
			default:
				throw "ClickType error";
			}
		}
		void drawCircle(const Data& data);
		void drawLine(const Data& data);
		void startTransform();
		void transform(const Data& data, TRANSFORM_CALLBACK callBack, size_t step);
		void endTransform();
	};
	struct Animation : DynamicArray<Frame*>
	{
		Gdiplus::Bitmap* bitmap;
		size_t frameIndex = 0;
		Animation(size_t w, size_t h)
		{
			bitmap = new Gdiplus::Bitmap(w, h);
			add(0, new Frame(w, h));
		}
		Frame& get()
		{
			return *((*this)[frameIndex]);
		}
		void show(HDC hdc)
		{
			get().drawBitmap(bitmap);
			Gdiplus::Graphics g(hdc);
			g.DrawImage(bitmap, 0, 0);
		}
		void show(HDC hdc, size_t pixelCount)
		{
			get().drawBitmap(bitmap, pixelCount);
			Gdiplus::Graphics g(hdc);
			g.DrawImage(bitmap, 0, 0);
		}
		void showLow(HDC hdc)
		{
			get().frameCopy->drawBitmapOfRects(bitmap);
			Gdiplus::Graphics g(hdc);
			g.DrawImage(bitmap, 0, 0);
		}
	};
}*/