#pragma once
#include <string>
namespace Level1
{
	Line* mainStroke = new Line(nullptr, POINT(), POINT(), Gdiplus::Color::Yellow, 0);
	Line* baseStroke = new Line(nullptr, POINT(), POINT(), Gdiplus::Color::Orange, 0);
	UINT randSeed = 0;
	inline Gdiplus::Color operator+(Gdiplus::Color a, Gdiplus::Color b)
	{
		return Gdiplus::Color::MakeARGB(
			a.GetA() + b.GetA(),
			a.GetR() + b.GetR(),
			a.GetG() + b.GetG(),
			a.GetB() + b.GetB()
		);
	}
	inline Gdiplus::Color operator-(Gdiplus::Color a, Gdiplus::Color b)
	{
		return Gdiplus::Color::MakeARGB(
			a.GetA() - b.GetA(),
			a.GetR() - b.GetR(),
			a.GetG() - b.GetG(),
			a.GetB() - b.GetB()
		);
	}
	inline Gdiplus::Color Mid(Gdiplus::Color a, Gdiplus::Color b)
	{
		return Gdiplus::Color::MakeARGB(
			(a.GetA() + b.GetA()) >> 1,
			(a.GetR() + b.GetR()) >> 1,
			(a.GetG() + b.GetG()) >> 1,
			(a.GetB() + b.GetB()) >> 1
		);
	}
	inline Gdiplus::Color Mid(double m, Gdiplus::Color a, Gdiplus::Color b)
	{
		double m1 = 1 - m;
		return Gdiplus::Color::MakeARGB(
			a.GetA() * m + b.GetA() * m1,
			a.GetR() * m + b.GetR() * m1,
			a.GetG() * m + b.GetG() * m1,
			a.GetB() * m + b.GetB() * m1
		);
	}

	inline void UintToWchars(wchar_t* wc, UINT i)
	{
		if (i >= 1000)
		{
			wc[0] = i / 1000 + L'0';
			wc[1] = ((i / 100) % 10) + L'0';
			wc[2] = ((i / 10) % 10) + L'0';
			wc[3] = (i % 10) + L'0';
		}
		else if (i >= 100)
		{
			wc[0] = i / 100 + L'0';
			wc[1] = ((i / 10) % 10) + L'0';
			wc[2] = (i % 10) + L'0';
			wc[3] = 0;
		}
		else if (i >= 10)
		{
			wc[0] = i / 10 + L'0';
			wc[1] = (i % 10) + L'0';
			wc[2] = 0;
			wc[3] = 0;
		}
		else
		{
			wc[0] = i + L'0';
			wc[1] = 0;
			wc[2] = 0;
			wc[3] = 0;
		}
	}

	inline wchar_t ByteToHexItem(BYTE a)
	{
		return (a <= 9) ? a + L'0' : a - 10 + L'a';
	}

	inline Hex ColorToHex(Gdiplus::Color color)
	{
		return Hex(
			ByteToHexItem(color.GetR() >> 4),
			ByteToHexItem(color.GetR() & 15),

			ByteToHexItem(color.GetG() >> 4),
			ByteToHexItem(color.GetG() & 15),

			ByteToHexItem(color.GetB() >> 4),
			ByteToHexItem(color.GetB() & 15),

			ByteToHexItem(color.GetA() >> 4),
			ByteToHexItem(color.GetA() & 15)
		);
	}

	inline BYTE HexItemToBYTE(wchar_t wc)
	{
		return (wc <= L'9') ? wc - L'0' : (wc <= L'A') ? wc - L'A' + 10 : wc - L'a' + 10;
	}


	inline Gdiplus::Color ToColor(const Hex& hex)
	{
		return Gdiplus::Color::MakeARGB(
			(HexItemToBYTE(hex.data[6]) << 4) | HexItemToBYTE(hex.data[7]) & 15,
			(HexItemToBYTE(hex.data[0]) << 4) | HexItemToBYTE(hex.data[1]) & 15,
			(HexItemToBYTE(hex.data[2]) << 4) | HexItemToBYTE(hex.data[3]) & 15,
			(HexItemToBYTE(hex.data[4]) << 4) | HexItemToBYTE(hex.data[5]) & 15);
	}

	inline double DistanceDifference(POINT start, POINT end, POINT center)
	{
		POINT dif1;
		dif1.x = start.x - center.x;
		dif1.y = start.y = center.y;
		POINT dif2;
		dif2.x = end.x - center.x;
		dif2.y = end.y = center.y;
		return sqrt(dif2.x * dif2.x + dif2.y * dif2.y) - sqrt(dif1.x * dif1.x + dif1.y * dif1.y);
	}
	void DeepDraw(PixelMap* pmap, ULONG x, ULONG y, Line l)
	{
		float step = M_PI_2 / l.width.u;
		l.width /= 2;
		if (l.width.b == 0)
		{
			for (float i = 0; i < M_PI_2; i += step)
			{
				auto s = sinf(i), c = cosf(i);
				for (UINT j = 1; j <= l.width.u; j++)
				{
					int deltaX = j * s,
						deltaY = j * c;
					int x1 = x + deltaX,
						x2 = x - deltaX,
						y1 = y + deltaY,
						y2 = y - deltaY;
					pmap->get(x1, y1).push(l);
					pmap->get(x1, y2).push(l);
					pmap->get(x2, y1).push(l);
					pmap->get(x2, y2).push(l);
				}
			}
		}
		else
		{
			ULONG32 u1 = l.width.u + 1;
			Gdiplus::Color color1 = l.color;
			ULONG b = l.width.b << 24;
			(*(Gdiplus::ARGB*)(&color1)) |= b;
			(*(Gdiplus::ARGB*)(&color1)) &= b;
			for (float i = 0; i < M_PI_2; i += step)
			{
				auto s = sinf(i), c = cosf(i);
				for (UINT j = 1; j <= l.width.u; j++)
				{
					int deltaX = j * s,
						deltaY = j * c;
					int x1 = x + deltaX,
						x2 = x - deltaX,
						y1 = y + deltaY,
						y2 = y - deltaY;
					pmap->get(x1, y1).push(l);
					pmap->get(x1, y2).push(l);
					pmap->get(x2, y1).push(l);
					pmap->get(x2, y2).push(l);
				}
				int deltaX = u1 * s,
					deltaY = u1 * c;
				int x1 = x + deltaX,
					x2 = x - deltaX,
					y1 = y + deltaY,
					y2 = y - deltaY;
				pmap->get(x1, y1).push(l);
				pmap->get(x1, y2).push(l);
				pmap->get(x2, y1).push(l);
				pmap->get(x2, y2).push(l);
			}
		}
	}
	void FastDraw(PixelMap* pmap, ULONG x, ULONG y, Line l)
	{
		srand(randSeed);
		for (ULONG i = 0; i < l.width.u; i++)
		{
			pmap->get(x + rand() * l.width.u / RAND_MAX - (l.width.u >> 1), y + rand() * l.width.u / RAND_MAX - (l.width.u >> 1)).push(l);
		}
		/*l.width.u >>= 1;
		float uSin30 = l.width.u * sinf(M_PI / 6);
		float uSin60 = l.width.u * sinf(M_PI / 3);
		UINT yPlusUSin30 = y + uSin30;
		UINT yMinusUSin30 = y - uSin30;
		UINT xPlusUSin60 = x + uSin60;
		UINT xMinusUSin60 = x - uSin60;
		if (l.width.u >= 1)
		{
			pmap->get(x, y).push(l);
			pmap->get(x, y + l.width.u).push(l);
			pmap->get(xPlusUSin60, yPlusUSin30).push(l);
			pmap->get(xPlusUSin60, yMinusUSin30).push(l);
			pmap->get(x, y - l.width.u).push(l);
			pmap->get(xMinusUSin60, yMinusUSin30).push(l);
			pmap->get(xMinusUSin60, yPlusUSin30).push(l);
		}
		else
		{
			ULONG32 u1 = l.width.u + 1;
			Gdiplus::Color color1 = l.color;
			ULONG b = l.width.b << 24;
			(*(Gdiplus::ARGB*)(&color1)) |= b;
			(*(Gdiplus::ARGB*)(&color1)) &= b;
			pmap->get(x, y).push(l);
		}*/
	}
	void DeepClear(PixelMap* pmap, ULONG x, ULONG y, Line l)
	{
		float step = M_PI_2 / l.width.u;
		l.width.u = (l.width.u >> 1) + 2;
		for (float i = 0; i < M_PI_2; i += step)
		{
			auto s = sinf(i), c = cosf(i);
			for (UINT j = 1; j <= l.width.u; j++)
			{
				int deltaX = j * s,
					deltaY = j * c;
				int x1 = x + deltaX,
					x2 = x - deltaX,
					y1 = y + deltaY,
					y2 = y - deltaY;
				pmap->get(x1, y1).del(l.parent);
				pmap->get(x1, y2).del(l.parent);
				pmap->get(x2, y1).del(l.parent);
				pmap->get(x2, y2).del(l.parent);
			}
		}
	}
	void FastClear(PixelMap* pmap, ULONG x, ULONG y, Line l)
	{
		srand(randSeed);
		for (ULONG i = 0; i < l.width.u; i++)
		{
			pmap->get(x + rand() * l.width.u / RAND_MAX - (l.width.u >> 1), y + rand() * l.width.u / RAND_MAX - (l.width.u >> 1)).del(l.parent);
		}
		/*l.width.u >>= 1;
		float uSin30 = l.width.u * sinf(M_PI / 6);
		float uSin60 = l.width.u * sinf(M_PI / 3);
		UINT yPlusUSin30 = y + uSin30;
		UINT yMinusUSin30 = y - uSin30;
		UINT xPlusUSin60 = x + uSin60;
		UINT xMinusUSin60 = x - uSin60;
		if (l.width.u >= 1)
		{
			pmap->get(x, y).del(l.parent);
			pmap->get(x, y + l.width.u).del(l.parent);
			pmap->get(xPlusUSin60, yPlusUSin30).del(l.parent);
			pmap->get(xPlusUSin60, yMinusUSin30).del(l.parent);
			pmap->get(x, y - l.width.u).del(l.parent);
			pmap->get(xMinusUSin60, yMinusUSin30).del(l.parent);
			pmap->get(xMinusUSin60, yPlusUSin30).del(l.parent);
		}
		else
		{
			pmap->get(x, y).del(l.parent);
		}*/
	}
	void DrawLine(PixelMap* pmap, Line l,
		void (*f) (PixelMap* pmap, ULONG x, ULONG y, Line l))
	{
		POINT uStart;
		l.start.get(uStart);
		POINT uEnd;
		l.end.get(uEnd);
		if (uStart.x == uEnd.x)
		{
			if (uStart.y < uEnd.y)
			{
				while (uStart.y < uEnd.y)
				{
					f(pmap, uStart.x, uStart.y, l);
					uStart.y++;
				}
			}
			else
			{
				while (uStart.y > uEnd.y)
				{
					f(pmap, uStart.x, uStart.y, l);
					uStart.y--;
				}
			}
		}
		if (uStart.y == uEnd.y)
		{
			if (uStart.x < uEnd.x)
			{
				while (uStart.x < uEnd.x)
				{
					f(pmap, uStart.x, uStart.y, l);
					uStart.x++;
				}
			}
			else
			{
				while (uStart.x > uEnd.x)
				{
					f(pmap, uStart.x, uStart.y, l);
					uStart.x--;
				}
			}
		}
		else
		{
			float differenceX = l.end.x - l.start.x;
			float differenceY = l.end.y - l.start.y;
			float absX = abs(differenceX);
			float absY = abs(differenceY);
			if (absX > absY)
			{
				const float y = differenceY / absX;
				if (uStart.x < uEnd.x)
				{
					while (uStart.x < uEnd.x)
					{
						f(pmap, uStart.x, l.start.y, l);
						uStart.x++;
						l.start.y += y;
					}
				}
				else
				{
					while (uStart.x > uEnd.x)
					{
						f(pmap, uStart.x, l.start.y, l);
						uStart.x--;
						l.start.y += y;
					}
				}
			}
			else
			{
				const float x = differenceX / absY;
				if (uStart.y < uEnd.y)
				{
					while (uStart.y < uEnd.y)
					{
						f(pmap, l.start.x, uStart.y, l);
						l.start.x += x;
						uStart.y++;
					}
				}
				else
				{
					while (uStart.y > uEnd.y)
					{
						f(pmap, l.start.x, uStart.y, l);
						l.start.x += x;
						uStart.y--;
					}
				}
			}
		}
	}
	inline void Rotate(Point& p, Point center, double angel)
	{
		Point p1;
		p1.x = p.x - center.x;
		p1.y = p.y - center.y;
		angel += atan2(p1.y, p1.x);
		double len = sqrt(p1.x * p1.x + p1.y * p1.y);
		p.x = len * cos(angel) + center.x;
		p.y = len * sin(angel) + center.y;
	}
	inline double Distance(Point a, Point b, double angel)
	{
		Rotate(a, b, angel);
		return abs(a.x - b.x);
	}
	inline double MaxDistance(Polyline& pl, Point p, double angel)
	{
		double maxDistance = 0;
		for (Line& l : pl)
		{
			double distance = Distance(l.start, p, angel);
			if (maxDistance < distance)
				maxDistance = distance;
		}
		return maxDistance;
	}
	inline double MinDistance(Polyline& pl, Point p)
	{
		double minDistance = 999999;
		for (Line& l : pl)
		{
			double distance1 = distance(l.start, p);
			if (minDistance > distance1)
				minDistance = distance1;
		}
		return minDistance;
	}
	inline double MaxDistance(Polyline& pl, Point p)
	{
		double maxDistance = 0;
		for (Line& l : pl)
		{
			double distance1 = distance(l.start, p);
			if (maxDistance < distance1)
				maxDistance = distance1;
		}
		return maxDistance;
	}
	String ToString(const Line& l)
	{
		String s(15);
		UINT16 u16;
		u16 = l.start.x;
		s[0] = u16;
		s[1] = u16 >> 8;

		u16 = l.start.y;
		s[2] = u16;
		s[3] = u16 >> 8;

		u16 = l.end.x;
		s[4] = u16;
		s[5] = u16 >> 8;

		u16 = l.end.y;
		s[6] = u16;
		s[7] = u16 >> 8;

		s[8] = l.width.u;
		s[9] = l.width.u >> 8;

		s[10] = l.width.b;

		s[11] = l.color.GetA();
		s[12] = l.color.GetR();
		s[13] = l.color.GetG();
		s[14] = l.color.GetB();
		return s;
	}
	const char PolylineEnd1[15] = { (char)0, (char)1, (char)2,(char)3, (char)4, (char)5, (char)6, (char)7, (char)8,
		(char)9, (char)10, (char)0, (char)0, (char)13,(char)14 };
	String PolylineEnd(15, PolylineEnd1);
	const char ShapeEnd1[15] = {(char)0, (char)1, (char)2,(char)3, (char)4, (char)5, (char)6, (char)7, (char)8,
		(char)9, (char)10, (char)0, (char)1, (char)13,(char)14 };
	String ShapeEnd(15, ShapeEnd1);
	const char FrameEnd1[15] = { (char)0, (char)1, (char)2,(char)3, (char)4, (char)5, (char)6, (char)7, (char)8,
		(char)9, (char)10, (char)0, (char)2, (char)13,(char)14 };
	String FrameEnd(15, FrameEnd1);
	Line ToLine(String s, Polyline* p)
	{
		StrokeWidth sw;
		sw.u = s[8] | (s[9] << 8);
		sw.b = s[10];
		return Line(p, Point((UINT32)s[0] | ((UINT32)s[1] << 8), (UINT32)s[2] | ((UINT32)s[3] << 8)),
			Point((UINT32)s[4] | ((UINT32)s[5] << 8), (UINT32)s[6] | ((UINT32)s[7] << 8)),
			Gdiplus::Color::MakeARGB(s[11], s[12], s[13], s[14]),
			sw);
	}
	String ToString(const Polyline& p)
	{
		String s(p.size() * 15);
		size_t i = 0;
		for (const Line& l : p)
		{
			String s1 = ToString(l);
			for (char c : s1)
			{
				s[i] = c;
				i++;
			}
		}
		return s;
	}
	void ToPolyline(String s, Shape* sh)
	{
		sh->push_front(Polyline(sh));
		Polyline& p = *sh->begin();
		String s1(15);
		for (size_t i = 0; i < s.size(); i += 15)
		{
			for (size_t j = 0; j < 15; j++)
				s1[j] = s[i + j];
			p.push_front(ToLine(s1, &p));
		}
	}
	String ToString(const Shape& sh)
	{
		size_t size = 0;
		for (const Polyline& p : sh)
			size += p.size() * 15 + 15;
		String s(size);
		size_t i = 0;
		for (const Polyline& p : sh)
		{
			String s1 = ToString(p);
			for (char c : s1)
			{
				s[i] = c;
				i++;
			}
			for (char c : PolylineEnd)
			{
				s[i] = c;
				i++;
			}
		}
		return s;
	}
	void ToShape(String s, TouchMap* t)
	{
		t->push_front(Shape());
		Shape& sh = *t->begin();
		String s1(60000);
		size_t k = 0;
		for (size_t i = 0; i < s.size(); i += 15)
		{
			if ((s[i + 11] == 0) and (s[i + 12] == 0))
			{
				s1.n = k;
				ToPolyline(s1, &sh);
				k = 0;
			}
			else
			{
				for (size_t j = 0; j < 15; j++)
				{
					s1[k] = s[i + j];
					k++;
				}
			}
		}
	}
	String ToString(const TouchMap& t)
	{
		size_t size = 0;
		for (const Shape& sh : t)
		{
			for (const Polyline& p : sh)
				size += p.size() * 15 + 15;
			size += 15;
		}
		String s(size);
		size_t i = 0;
		for (const Shape& sh : t)
		{
			String s1 = ToString(sh);
			for (char c : s1)
			{
				s[i] = c;
				i++;
			}
			for (char c : ShapeEnd)
			{
				s[i] = c;
				i++;
			}
		}
		return s;
	}
	TouchMap ToTouchMap(String s)
	{
		TouchMap t;
		String s1(60000);
		size_t k = 0;
		for (size_t i = 0; i < s.size(); i += 15)
		{
			if ((s[i + 11] == 0) and (s[i + 12] == 1))
			{
				s1.n = k;
				ToShape(s1, &t);
				k = 0;
			}
			else
			{
				for (size_t j = 0; j < 15; j++)
				{
					s1[k] = s[i + j];
					k++;
				}
			}
		}
		return t;
	}
	String ToString(const Animation* const a)
	{
		size_t size = 4;
		for (const Frame& f : *a)
		{
			for (const Shape& sh : f.tm)
			{
				for (const Polyline& p : sh)
					size += p.size() * 15 + 15;
				size += 15;
			}
			size += 15;
		}
		String s(size);
		s[0] = a->w;
		s[1] = a->w >> 8;
		s[2] = a->h;
		s[3] = a->h >> 8;
		size_t i = 4;
		for (const Frame& f : *a)
		{
			String s1 = ToString(f.tm);
			for (char c : s1)
			{
				s[i] = c;
				i++;
			}
			for (char c : FrameEnd)
			{
				s[i] = c;
				i++;
			}
		}
		return s;
	}
	Animation* ToAnimation(String s)
	{
		Animation* a = new Animation((ULONG)s[0] | ((ULONG)s[1] << 8), (ULONG)s[2] | ((ULONG)s[3] << 8));
		a->clear();
		String s1(60000);
		size_t k = 0;
		for (size_t i = 4; i < s.size(); i += 15)
		{
			if ((s[i + 11] == 0) and (s[i + 12] == 2))
			{
				s1.n = k;
				a->push_back(Frame(a->w, a->h, Gdiplus::Color::Red));
				Frame& f = *a->rbegin();
				f.tm = ToTouchMap(s1);
				f.tm.deepDraw(f.pm);
				f.pm->deepDraw(f.b);
				k = 0;
			}
			else
			{
				for (size_t j = 0; j < 15; j++)
				{
					s1[k] = s[i + j];
					k++;
				}
			}
		}
		return a;
	}
}