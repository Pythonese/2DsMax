#include "method_forward_declarations.h"
#include "functions.h"
char HexChars[8] = "HexCha";
char PointTextChars[9] = "";
//HWND midFrameStatic, midFrameEdit;

Level1::String::String()
{
	n = 0;
	data = nullptr;
}

Level1::String::String(size_t n, const char* s)
{
	this->n = n;
	data = new BYTE[n];
	for (size_t i = 0; i < n; i++)
	{
		data[i] = s[i];
	}
}

Level1::String::String(size_t n)
{
	data = new BYTE[n] {};
	this->n = n;
}

Level1::String::String(const String& s)
{
	n = s.size();
	data = new BYTE[s.size()];
	for (size_t i = 0; i < n; i++)
		data[i] = s.data[i];
}

Level1::String::String(String&& s)
{
	delete[] data;
	data = s.data;
	s.data = nullptr;
	n = s.n;
}

Level1::String::~String()
{
	delete[] data;
}

BYTE* Level1::String::begin()
{
	return data;
}

BYTE* Level1::String::end()
{
	return data + n;
}

size_t Level1::String::size() const
{
	return n;
}

BYTE& Level1::String::operator[](size_t i)
{
	return data[i];
}

const BYTE& Level1::String::operator[](size_t i) const
{
	return data[i];
}

void Level1::Hex::updateChars()
{
	HexChars[0] = data[0];
	HexChars[1] = data[1];
	HexChars[2] = data[2];
	HexChars[3] = data[3];
	HexChars[4] = data[4];
	HexChars[5] = data[5];
	HexChars[6] = data[6];
	HexChars[7] = data[7];
}

Level1::Hex::Hex() {}


Level1::Hex::Hex(wchar_t a0, wchar_t a1, wchar_t a2, wchar_t a3, wchar_t a4, wchar_t a5, wchar_t a6, wchar_t a7)
{
	data[0] = a0;
	data[1] = a1;
	data[2] = a2;
	data[3] = a3;
	data[4] = a4;
	data[5] = a5;
	data[6] = a6;
	data[7] = a7;
}

void Level1::Bitset64::set(BYTE i, bool b)
{
	if (b) data |= 1ULL << i;
	else data &= ~(1ULL << i);
}

bool Level1::Bitset64::get(BYTE i)
{
	return ((data >> i) & 1) == 1;
}

bool Level1::operator==(const String& a, const String& b)
{
	if (a.size() != b.size())
		return false;
	for (size_t i = 0; i < a.size(); i++)
		if (a[i] != b[i])
			return false;
	return true;
}

bool Level1::operator==(const Hex& a, const Hex& b)
{
	return a.data[0] == b.data[0]
		and a.data[1] == b.data[1]
		and a.data[2] == b.data[2]
		and a.data[3] == b.data[3]
		and a.data[4] == b.data[4]
		and a.data[5] == b.data[5]
		and a.data[6] == b.data[6]
		and a.data[7] == b.data[7];
}

Level1::StrokeWidth Level1::operator/(StrokeWidth width, float f)
{
	return StrokeWidth(toFloat(width) / f);
}

float Level1::toFloat(StrokeWidth width)
{
	return (float)width.u + (float)width.b / 255.0;
}

bool Level1::operator==(const StrokeWidth& a, const StrokeWidth& b)
{
	return a.b == b.b and a.u == b.u;
}

Level1::StrokeWidth Level1::mid(double m, const StrokeWidth& a, const StrokeWidth& b)
{
	return toFloat(a) * m + toFloat(b) * (1 - m);
}

Level1::StrokeWidth Level1::mid(const StrokeWidth& a, const StrokeWidth& b)
{
	return (toFloat(a) + toFloat(b)) / 2;
}

void Level1::StrokeWidth::operator/=(float f)
{
	(*this) = StrokeWidth(toFloat(*this) / f);
}

Level1::Point Level1::operator+(const Point& a, const Point& b)
{
	return Point(a.x + b.x, a.y + b.y);
}

Level1::Point Level1::operator-(const Point& a, const Point& b)
{
	return Point(a.x - b.x, a.y - b.y);
}

Level1::Point Level1::operator*(const Point& a, const Point& b)
{
	return Point(a.x * b.x, a.y * b.y);
}

Level1::Point Level1::operator/(const Point& a, const Point& b)
{
	return Point(a.x / b.x, a.y / b.y);
}

Level1::Point Level1::mid(const Point& a, const Point& b)
{
	return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
}

Level1::Point Level1::mid(double m, const Point a, const Point b)
{
	return Point(a.x * m + b.x * (1 - m), a.y * m + b.y * (1 - m));
}

float Level1::distance(Point a, const Point& b)
{
	a -= b;
	return sqrt(a.x * a.x + a.y * a.y);
}

float Level1::distance(Point a, const Point& b, Point dif)
{
	dif *= a - b;
	return sqrt(dif.x * dif.x + dif.y * dif.y);
}

void Level1::Point::operator+=(const Point& a)
{
	x += a.x;
	y += a.y;
}

void Level1::Point::operator-=(const Point& a)
{
	x -= a.x;
	y -= a.y;
}

void Level1::Point::operator*=(const Point& a)
{
	x *= a.x;
	y *= a.y;
}

void Level1::Point::operator/=(const Point& a)
{
	x /= a.x;
	y /= a.y;
}

void Level1::Point::mid(const Point& a)
{
	x = (a.x + x) / 2;
	y = (a.y + y) / 2;
}

void Level1::Point::set(HWND hwnd)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hwnd, &p);
	x = p.x;
	y = p.y;
}

void Level1::Point::get(POINT& p)
{
	p.x = x;
	p.y = y;
}

bool Level1::operator==(const Point& a, const Point& b)
{
	return (a.x == b.x) and (a.y == b.y);
}

bool Level1::operator!=(const Point& a, const Point& b)
{
	return a.x != b.x or a.y != b.y;
}

void Level1::PointText::set(Point p)
{
	wchar_t wc[4]{};
	UintToWchars(wc, round(p.x));
	size_t count = 0;
	while (wc[count] != 0 and count < 4)
	{
		data[count] = wc[count];
		count++;
	}
	data[count] = L' ';
	count++;
	wc[0] = 0;
	wc[1] = 0;
	wc[2] = 0;
	wc[3] = 0;
	UintToWchars(wc, round(p.y));
	size_t i = 0;
	while (wc[i] != 0 and i < 4)
	{
		data[count + i] = wc[i];
		i++;
	}
	data[count + i] = L' ';
}

Level1::Point Level1::PointText::get()
{
	Point p;
	p.x = _wtoi(data);
	size_t i = 0;
	while (data[i] != L' ')
		i++;
	i++;
	p.y = _wtoi(data + i);
	return p;
}

void Level1::PointText::updateChars()
{
	PointTextChars[0] = data[0];
	PointTextChars[1] = data[1];
	PointTextChars[2] = data[2];
	PointTextChars[3] = data[3];
	PointTextChars[4] = data[4];
	PointTextChars[5] = data[5];
	PointTextChars[6] = data[6];
	PointTextChars[7] = data[7];
	PointTextChars[8] = data[8];
}

Level1::Transform Level1::operator+(const Transform& a, const Transform& b)
{
	Transform t;
	t.brush.color = a.brush.color + b.brush.color;
	t.brush.width = toFloat(a.brush.width) + toFloat(b.brush.width);
	t.location = a.location + b.location;
	t.locationOffset = a.locationOffset + b.locationOffset;
	t.rotation = a.rotation + b.rotation;
	t.rotationOffset = a.rotationOffset + b.rotationOffset;
	t.scale = a.scale + b.scale;
	t.scaleMultiplier = a.scaleMultiplier + b.scaleMultiplier;
	return t;
}

Level1::Transform Level1::operator-(const Transform& a, const Transform& b)
{
	Transform t;
	t.brush.color = a.brush.color - b.brush.color;
	t.brush.width = toFloat(a.brush.width) - toFloat(b.brush.width);
	t.location = a.location - b.location;
	t.locationOffset = a.locationOffset - b.locationOffset;
	t.rotation = a.rotation - b.rotation;
	t.rotationOffset = a.rotationOffset - b.rotationOffset;
	t.scale = a.scale - b.scale;
	t.scaleMultiplier = a.scaleMultiplier - b.scaleMultiplier;
	return t;
}

Level1::Transform Level1::mid(const Transform& a, const Transform& b)
{
	Transform t;
	t.brush.color = Mid(a.brush.color, b.brush.color);
	t.brush.width = (toFloat(a.brush.width) + toFloat(b.brush.width)) / 2;
	t.location = mid(a.location, b.location);
	t.locationOffset = mid(a.locationOffset, b.locationOffset);
	t.rotation = (a.rotation + b.rotation) / 2;
	t.rotationOffset = (a.rotationOffset + b.rotationOffset) / 2;
	t.scale = mid(a.scale, b.scale);
	t.scaleMultiplier = mid(a.scaleMultiplier, b.scaleMultiplier);
	return t;
}

void Level1::Transform::operator+=(const Transform& a)
{
	brush.color = a.brush.color + brush.color;
	brush.width = toFloat(a.brush.width) + toFloat(brush.width);
	location = a.location + location;
	locationOffset = a.locationOffset + locationOffset;
	rotation = a.rotation + rotation;
	rotationOffset = a.rotationOffset + rotationOffset;
	scale = a.scale + scale;
	scaleMultiplier = a.scaleMultiplier + scaleMultiplier;
}

void Level1::Transform::operator-=(const Transform& a)
{
	brush.color = a.brush.color - brush.color;
	brush.width = toFloat(a.brush.width) - toFloat(brush.width);
	location = a.location - location;
	locationOffset = a.locationOffset - locationOffset;
	rotation = a.rotation - rotation;
	rotationOffset = a.rotationOffset - rotationOffset;
	scale = a.scale - scale;
	scaleMultiplier = a.scaleMultiplier - scaleMultiplier;
}

void Level1::Transform::mid(const Transform& a)
{
	brush.color = Mid(a.brush.color, brush.color);
	brush.width = (toFloat(a.brush.width) + toFloat(brush.width)) / 2;
	location = Level1::mid(a.location, location);
	locationOffset = Level1::mid(a.locationOffset, locationOffset);
	rotation = (a.rotation + rotation) / 2;
	rotationOffset = (a.rotationOffset + rotationOffset) / 2;
	scale = Level1::mid(a.scale, scale);
	scaleMultiplier = Level1::mid(a.scaleMultiplier, scaleMultiplier);
}

void Level1::TransformText::set(const Transform& t)
{
	worldLocation.set(t.location - t.locationOffset);
	localLocation.set(t.location);
	UintToWchars(worldRotation, ((int)round((t.rotation - t.rotationOffset) * 180 / M_PI) + 360) % 360);
	UintToWchars(localRotation, ((int)round(t.rotation * 180 / M_PI) + 360) % 360);
	worldScale.set(t.scale / t.scaleMultiplier);
	localScale.set(t.scale);
	color = ColorToHex(t.brush.color);
	UintToWchars(width, t.brush.width.u);
}

void Level1::TransformText::set(const TransformTextBoxes& t)
{
	SendMessage(t.colorEdit, WM_GETTEXT, 80, LPARAM(color.data));
	SendMessage(t.widthEdit, WM_GETTEXT, 80, LPARAM(width));
	SendMessage(t.worldLocationEdit, WM_GETTEXT, 80, LPARAM(worldLocation.data));
	SendMessage(t.localLocationEdit, WM_GETTEXT, 80, LPARAM(localLocation.data));
	SendMessage(t.worldRotationEdit, WM_GETTEXT, 80, LPARAM(worldRotation));
	SendMessage(t.localRotationEdit, WM_GETTEXT, 80, LPARAM(localRotation));
	SendMessage(t.worldScaleEdit, WM_GETTEXT, 80, LPARAM(worldScale.data));
	SendMessage(t.localScaleEdit, WM_GETTEXT, 80, LPARAM(localScale.data));
}

Level1::Transform Level1::TransformText::get()
{
	Transform t;
	t.location = localLocation.get();
	t.locationOffset = t.location - worldLocation.get();
	t.rotation = _wtoi(localRotation) * M_PI / 180;
	t.rotationOffset = t.rotation - (_wtoi(worldRotation) * M_PI / 180);
	t.scale = localScale.get();
	t.scaleMultiplier = t.scale / worldScale.get();
	t.brush.color = ToColor(color);
	t.brush.width = _wtoi(width);
	return t;
}

void Level1::TransformText::get(TransformTextBoxes& textBoxes)
{
	color.updateChars();
	SetWindowTextA(textBoxes.colorEdit, HexChars);
	{
		char c[3];
		c[0] = width[0];
		c[1] = width[1];
		c[2] = width[2];
		SetWindowTextA(textBoxes.widthEdit, c);
	}
	worldLocation.updateChars();
	SetWindowTextA(textBoxes.worldLocationEdit, PointTextChars);
	localLocation.updateChars();
	SetWindowTextA(textBoxes.localLocationEdit, PointTextChars);
	{
		char c[3];
		c[0] = worldRotation[0];
		c[1] = worldRotation[1];
		c[2] = worldRotation[2];
		SetWindowTextA(textBoxes.worldRotationEdit, c);
		c[0] = localRotation[0];
		c[1] = localRotation[1];
		c[2] = localRotation[2];
		SetWindowTextA(textBoxes.localRotationEdit, c);
	}
	worldScale.updateChars();
	SetWindowTextA(textBoxes.worldScaleEdit, PointTextChars);
	localScale.updateChars();
	SetWindowTextA(textBoxes.localScaleEdit, PointTextChars);
}

void Level1::TransformTextBoxes::create(HWND& hwnd, POINT p)
{
	constexpr int w = 200, h = 25;
	colorEdit = CreateWindow(
		L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL,
		p.x, p.y, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 0);
	colorStatic = CreateWindow(
		L"Static", L"цвет", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		p.x + w, p.y, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

	widthEdit = CreateWindow(
		L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL,
		p.x, p.y + h, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 0);
	widthStatic = CreateWindow(
		L"Static", L"толщина", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		p.x + w, p.y + h, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

	//

	worldLocationEdit = CreateWindow(
		L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL,
		p.x, p.y + h*2, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 0);
	worldLocationStatic = CreateWindow(
		L"Static", L"глобальное расположение", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		p.x + w, p.y + h*2, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

	localLocationEdit = CreateWindow(
		L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL,
		p.x, p.y + h*3, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 0);
	localLocationStatic = CreateWindow(
		L"Static", L"локальное расположение", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		p.x + w, p.y + h*3, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

	//

	worldRotationEdit = CreateWindow(
		L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL,
		p.x, p.y + h * 4, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 0);
	worldRotationStatic = CreateWindow(
		L"Static", L"глобальный поворот", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		p.x + w, p.y + h * 4, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

	localRotationEdit = CreateWindow(
		L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL,
		p.x, p.y + h * 5, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 0);
	localRotationStatic = CreateWindow(
		L"Static", L"локальный поворот", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		p.x + w, p.y + h * 5, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

	//

	worldScaleEdit = CreateWindow(
		L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL,
		p.x, p.y + h * 6, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 0);
	worldScaleStatic = CreateWindow(
		L"Static", L"глобальный размер", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		p.x + w, p.y + h * 6, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

	localScaleEdit = CreateWindow(
		L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL,
		p.x, p.y + h * 7, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 0);
	localScaleStatic = CreateWindow(
		L"Static", L"локальный размер", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		p.x + w, p.y + h * 7, w, h,
		hwnd, 0, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
}

bool Level1::TransformTextBoxes::isValid()
{
	return colorStatic != NULL;
}

void Level1::TransformTextBoxes::destroy()
{
	DestroyWindow(colorStatic);
	DestroyWindow(widthStatic);
	DestroyWindow(worldLocationStatic);
	DestroyWindow(localLocationStatic);
	DestroyWindow(worldRotationStatic);
	DestroyWindow(localRotationStatic);
	DestroyWindow(worldScaleStatic);
	DestroyWindow(localScaleStatic);

	DestroyWindow(colorEdit);
	DestroyWindow(widthEdit);
	DestroyWindow(worldLocationEdit);
	DestroyWindow(localLocationEdit);
	DestroyWindow(worldRotationEdit);
	DestroyWindow(localRotationEdit);
	DestroyWindow(worldScaleEdit);
	DestroyWindow(localScaleEdit);

	colorStatic = NULL;
	widthStatic = NULL;
	worldLocationStatic = NULL;
	localLocationStatic = NULL;
	worldRotationStatic = NULL;
	localRotationStatic = NULL;
	worldScaleStatic = NULL;
	localScaleStatic = NULL;

	colorEdit = NULL;
	widthEdit = NULL;
	worldLocationEdit = NULL;
	localLocationEdit = NULL;
	worldRotationEdit = NULL;
	localRotationEdit = NULL;
	worldScaleEdit = NULL;
	localScaleEdit = NULL;
}

Gdiplus::Color Level1::Pixel::fastGetColor() const
{
	if (empty())
		return Gdiplus::Color::White;
	switch (begin()->parent->parent->vt.brush.type)
	{
	case BrushInfo::NONE:
		switch (begin()->parent->vt.brush.type)
		{
		case BrushInfo::NONE:
			return begin()->color;
		case BrushInfo::SELECTION:
			return Gdiplus::Color::Orange;
		case BrushInfo::MAIN_SELECTION:
			return Gdiplus::Color::Yellow;
		}
		break;
	case BrushInfo::SELECTION:
		return Gdiplus::Color::Orange;
	case BrushInfo::MAIN_SELECTION:
		return Gdiplus::Color::Yellow;
	}
}

Gdiplus::Color Level1::Pixel::getColor() const
{
	if (empty())
		return Gdiplus::Color::White;
	Gdiplus::Color res;
	for (auto i = crbegin(); i != crend(); i++)
	{
		Gdiplus::Color color;
		switch (i->parent->parent->vt.brush.type)
		{
		case BrushInfo::NONE:
			switch (i->parent->vt.brush.type)
			{
			case BrushInfo::NONE:
			{
				color = i->color;
				break;
			}
			case BrushInfo::SELECTION:
				color = Gdiplus::Color::Orange;
				break;
			case BrushInfo::MAIN_SELECTION:
				color = Gdiplus::Color::Yellow;
				break;
			}
			break;
		case BrushInfo::SELECTION:
			color = Gdiplus::Color::Orange;
			break;
		case BrushInfo::MAIN_SELECTION:
			color = Gdiplus::Color::Yellow;
			break;
		}

		float a = color.GetA() / 255.0;
		float a1 = 1 - a;
		res = Gdiplus::Color::MakeARGB(
			255,
			color.GetR() * a + res.GetR() * a1,
			color.GetG() * a + res.GetG() * a1,
			color.GetB() * a + res.GetB() * a1
		);
	}
	return res;
}

void Level1::Pixel::push(const Line& p)
{
	for (const Line& po : *this)
	{
		if (po.parent == p.parent)
		{
			if (po.color.GetA() < p.color.GetA())
			{
				remove(po);
				push_front(p);
			}
			return;
		}
	}
	push_front(p);
}

void Level1::Pixel::del(Polyline* parent)
{
	for (const Line& po : *this)
	{
		if (po.parent == parent)
		{
			remove(po);
			return;
		}
	}
}

void Level1::PixelMap::drawLine(Gdiplus::Bitmap* b, ULONG y)
{
	UINT i = y * w;
	UINT x = 0;
	while (x < w) // должно делиться на 32
	{
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;

		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;

		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;

		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
		b->SetPixel(x, y, data[i].getColor()); x++; i++; b->SetPixel(x, y, data[i].getColor()); x++; i++;
	}
}

void Level1::PixelMap::fastDraw(Gdiplus::Bitmap* b, ULONG step)
{
	for (size_t i = 0; i < size; i += 2)
		b->SetPixel(i % w, i / w, data[i].fastGetColor());
}

void Level1::PixelMap::deepDraw(Gdiplus::Bitmap* b)
{
	UINT y = 0;
	while (y < h) // должно делиться на 10
	{
		drawLine(b, y); y++;
		drawLine(b, y); y++;
		drawLine(b, y); y++;
		drawLine(b, y); y++;
		drawLine(b, y); y++;

		drawLine(b, y); y++;
		drawLine(b, y); y++;
		drawLine(b, y); y++;
		drawLine(b, y); y++;
		drawLine(b, y); y++;
	}
}

void Level1::Line::fastDraw(PixelMap* pmap)
{
	DrawLine(pmap, *this, FastDraw);
}

void Level1::Line::deepDraw(PixelMap* pmap)
{
	DrawLine(pmap, *this, DeepDraw);
}

void Level1::Line::deepClear(PixelMap* pmap)
{
	DrawLine(pmap, *this, DeepClear);
}

void Level1::Line::fastClear(PixelMap* pmap)
{
	DrawLine(pmap, *this, FastClear);
}

bool Level1::operator==(const Line& a, const Line& b)
{
	return a.parent == b.parent and a.start == b.start and a.end == b.end
		and a.color.GetValue() == b.color.GetValue() and a.width == b.width;
}

void Level1::mid(double m, Level1::Polyline* p, const Level1::Line& a, const Level1::Line& b)
{
	p->push_front(Level1::Line(p, mid(m, a.start, b.start), mid(m, a.end, b.end), Level1::Mid(m, a.color, b.color), mid(m, a.width, b.width)));
}

void Level1::mid(Level1::Polyline* p, const Level1::Line& a, const Level1::Line& b)
{
	p->push_front(Level1::Line(p, mid(a.start, b.start), mid(a.end, b.end), Level1::Mid(a.color, b.color), mid(a.width, b.width)));
}

void Level1::Polyline::update(const Transform& t)
{
	if (t.scale / t.scaleMultiplier != vt.scale / vt.scaleMultiplier)
	{
		Point p = (t.scale / t.scaleMultiplier) - (vt.scale / vt.scaleMultiplier);
		addScale(0, -p.x);
		addScale(M_PI_2, -p.y);
	}
	if (t.rotation - t.rotationOffset != vt.rotation - vt.rotationOffset)
	{
		addRotation((vt.rotation - vt.rotationOffset) - (t.rotation - t.rotationOffset));
	}
	if (t.location - t.locationOffset != vt.location - vt.locationOffset)
	{
		Point p = (t.location - t.locationOffset) - (vt.location - vt.locationOffset);
		addLocation(p.x, p.y);
	}
	
	setWidth(t.brush.width);
	setColor(t.brush.color);
	vt.scaleMultiplier = t.scaleMultiplier;
	vt.rotationOffset = t.rotationOffset;
	vt.locationOffset = t.locationOffset;
}

void Level1::Polyline::setWidth(StrokeWidth sw)
{
	float dif = toFloat(vt.brush.width) - toFloat(sw);
	for (Line& l : *this)
	{
		l.width = toFloat(l.width) - dif;
	}
	vt.brush.width = sw;
}

void Level1::Polyline::setColor(Gdiplus::Color color)
{
	Gdiplus::Color dif = Gdiplus::Color::MakeARGB(
		vt.brush.color.GetA() - color.GetA(),
		vt.brush.color.GetR() - color.GetR(),
		vt.brush.color.GetG() - color.GetG(),
		vt.brush.color.GetB() - color.GetB()
	);
	for (Line& l : *this)
	{
		l.color = Gdiplus::Color::MakeARGB(
			l.color.GetA() - dif.GetA(),
			l.color.GetR() - dif.GetR(),
			l.color.GetG() - dif.GetG(),
			l.color.GetB() - dif.GetB()
		);
	}
	vt.brush.color = color;
}

void Level1::Polyline::updateTransform()
{
	size_t count = 0;
	size_t a = 0, r = 0, g = 0, b = 0;
	double width = 0;
	double sumXDistance = 0, sumYDistance = 0, sumMidX = 0, sumMidY = 0;
	for (const Line& line : *this)
	{
		a += line.color.GetA();
		r += line.color.GetR();
		g += line.color.GetG();
		b += line.color.GetB();
		width += toFloat(line.width);
		count++;
		double maxDistance = 0;
		Point midPoint;
		double maxXDivYDistance = 0, maxYDivXDistance = 0;
		float xDistance = 0, yDistance = 0;
		for (const Line& line1 : *this)
		{
			Point dif(abs(line.start.x - line1.start.x), abs(line.start.y - line1.start.y));
			double distance = sqrt(dif.x * dif.x + dif.y * dif.y);
			if (maxDistance < distance)
			{
				maxDistance = distance;
				midPoint = mid(line.start, line1.start);
			}
			double xDivYDistance = dif.x * (1 / dif.y);
			double yDivYDistance = dif.y * (1 / dif.x);
			if (maxXDivYDistance < xDivYDistance)
			{
				maxXDivYDistance = xDivYDistance;
				xDistance = dif.x;
			}
			if (maxYDivXDistance < yDivYDistance)
			{
				maxYDivXDistance = yDivYDistance;
				yDistance = dif.y;
			}
		}
		sumMidX += midPoint.x;
		sumMidY += midPoint.y;
		sumXDistance += xDistance;
		sumYDistance += yDistance;
	}
	vt.brush.color = Gdiplus::Color::MakeARGB(a / count, r / count, g / count, b / count);
	vt.brush.width = width / count;
	vt.location.x = sumMidX / count + vt.locationOffset.x;
	vt.location.y = sumMidY / count + vt.locationOffset.y;
	vt.scale.x = sumXDistance / count * vt.scaleMultiplier.x;
	vt.scale.y = sumYDistance / count * vt.scaleMultiplier.y;
}

void Level1::Polyline::fastDraw(PixelMap* pmap)
{
	for (Line& l : *this)
		l.fastDraw(pmap);
}

void Level1::Polyline::deepDraw(PixelMap* pmap)
{
	for (Line& l : *this)
		l.deepDraw(pmap);
}

void Level1::Polyline::deepClear(PixelMap* pmap)
{
	for (Line& l : *this)
		l.deepClear(pmap);
}

void Level1::Polyline::fastClear(PixelMap* pmap)
{
	for (Line& l : *this)
		l.fastClear(pmap);
}

void Level1::Polyline::addLocation(LONG x, LONG y)
{
	for (Line& l : *this)
	{
		l.start.x += x;
		l.start.y += y;
		l.end.x += x;
		l.end.y += y;
	}
	updateTransform();
}

void Level1::Polyline::addRotation(double angel)
{
	Point location = vt.location;
	for (Line& l : *this)
	{
		Rotate(l.start, location, angel);
		Rotate(l.end, location, angel);
	}
	vt.rotation += angel;
	updateTransform();
}

void Level1::Polyline::addScale(double angel, double force)
{
	Point multiplier(cos(angel), sin(angel));
	Point forceVector(multiplier.x * force, multiplier.y * force);
	Point location(vt.location - vt.locationOffset);
	double maxDistance = MaxDistance(*this, location, angel);
	for (Line& l : *this)
	{
		Point p = l.start;
		Rotate(p, location, angel);
		p.x = (p.x - location.x) / maxDistance;
		l.start.x -= forceVector.x * p.x;
		l.start.y += forceVector.y * p.x;

		p = l.end;
		Rotate(p, location, angel);
		p.x = (p.x - location.x) / maxDistance;
		l.end.x -= forceVector.x * p.x;
		l.end.y += forceVector.y * p.x;
	}
	updateTransform();
}

void Level1::Polyline::addItemLocation(LONG x, LONG y, POINT p, StrokeWidth width)
{
	double minDistance = MinDistance(*this, p);
	double maxDistance = MaxDistance(*this, p) * toFloat(width);
	for (Line& l : *this)
	{
		double distance1 = distance(p, l.start);
		double mult = (1 - distance1 / maxDistance) * (minDistance / distance1);
		l.start.x += x * mult;
		l.start.y += y * mult;
		distance1 = distance(p, l.end);
		mult = (1 - distance1 / maxDistance) * (minDistance / distance1);
		l.end.x += x * mult;
		l.end.y += y * mult;
	}
	updateTransform();
}

void Level1::Polyline::addItemRotation(double angel, POINT p, StrokeWidth width)
{
	if (angel > M_PI) angel -= M_PI * 2;
	else if (angel < -M_PI) angel += M_PI * 2;
	double minDistance = MinDistance(*this, p);
	double maxDistance = MaxDistance(*this, p) * toFloat(width);
	Point location = vt.location;
	size_t count = 0;
	double rotation = 0;
	for (Line& l : *this)
	{
		double distance1 = distance(p, l.start);
		double add = angel * (1 - distance1 / maxDistance) * (minDistance / distance1);
		Rotate(l.start, location, add);
		rotation += add;
		distance1 = distance(p, l.end);
		add = angel * (1 - distance1 / maxDistance) * (minDistance / distance1);
		Rotate(l.end, location, add);
		rotation += add;
		count++;
	}
	vt.rotation += rotation / (count << 1);
	updateTransform();
}

void Level1::Polyline::addItemScale(double angel, double force, POINT p, StrokeWidth width)
{
	Point multiplier(cos(angel), sin(angel));
	Point forceVector(multiplier.x * force, multiplier.y * force);
	Point location(vt.location);
	double minDistance = MinDistance(*this, p);
	double maxDistance = MaxDistance(*this, p) * toFloat(width);
	double maxDistance1 = MaxDistance(*this, location, angel);
	for (Line& l : *this)
	{
		Point p1 = l.start;
		Rotate(p1, location, angel);
		double distance1 = distance(p, l.start);
		double mult = (p1.x - location.x) / maxDistance1 * (1 - distance1 / maxDistance) * (minDistance / distance1);
		l.start.x -= forceVector.x * mult;
		l.start.y += forceVector.y * mult;

		p1 = l.end;
		Rotate(p1, location, angel);
		distance1 = distance(p, l.end);
		mult = (p1.x - location.x) / maxDistance1 * (1 - distance1 / maxDistance) * (minDistance / distance1);
		l.end.x -= forceVector.x * mult;
		l.end.y += forceVector.y * mult;
	}
	updateTransform();
}

void Level1::Polyline::addCenterLocation(LONG x, LONG y)
{
}

void Level1::Polyline::addCenterRotation(double angel)
{
}

void Level1::Polyline::addCenterScale(double angel, double force)
{
}

void Level1::Polyline::setDefaultCenterLocation()
{
}

void Level1::Polyline::setDefaultCenterRotation()
{
}

void Level1::Polyline::setDefaultCenterScale()
{
}

void Level1::mid(double multiplier, Level1::Shape* parent, const Level1::Polyline& a, const Level1::Polyline& b)
{
	size_t midSize = a.size() * multiplier + b.size() * (1 - multiplier);
	double multiplier1 = a.size() / (double)midSize;
	double multiplier2 = b.size() / (double)midSize;
	parent->push_front(Level1::Polyline(parent));
	Level1::Polyline& p = *parent->begin();
	long i1 = a.size() - 1, i2 = b.size() - 1;
	auto iter1 = a.crbegin();
	auto iter2 = b.crbegin();
	Point midPoint = mid(multiplier, iter1->end, iter2->end);
	while (midSize--)
	{
		long i11 = midSize * multiplier1;
		long i22 = midSize * multiplier2;
		for (long j = i1; j > i11; j--)
			iter1++;
		for (long j = i2; j > i22; j--)
			iter2++;
		i1 = i11;
		i2 = i22;
		Point midStart = mid(multiplier, iter1->start, iter2->start);
		p.push_front(Level1::Line(&p, midStart, midPoint, Level1::Mid(multiplier, iter1->color, iter2->color), mid(multiplier, iter1->width, iter2->width)));
		midPoint = midStart;
	}
}

void Level1::mid(Level1::Shape* parent, const Level1::Polyline& a, const Level1::Polyline& b)
{
	size_t size1 = 0, size2 = 0;
	for (const Level1::Line& l : a)
		size1++;
	for (const Level1::Line& l : b)
		size2++;
	size_t midSize = (size1 + size2) >> 1;
	double multiplier1 = size1 / (double)midSize;
	double multiplier2 = size2 / (double)midSize;
	parent->push_front(Level1::Polyline(parent));
	Level1::Polyline& p = *parent->begin();
	long i1 = 0, i2 = 0;
	auto iter1 = a.cbegin();
	auto iter2 = b.cbegin();
	Point po1;
	for (const Line& l : a)
	{
		po1 = l.end;
	}
	Point po2;
	for (const Line& l : b)
	{
		po2 = l.end;
	}
	Point midPoint = mid(po1, po2);
	while (midSize--)
	{
		long i11 = midSize * multiplier1;
		long i22 = midSize * multiplier2;
		iter1 = a.cbegin();
		for (long j = 0; j < i11; j++)
			iter1++;
		iter2 = b.cbegin();
		for (long j = 0; j < i22; j++)
			iter2++;
		i1 = i11;
		i2 = i22;
		Point midStart = mid(iter1->start, iter2->start);
		p.push_front(Level1::Line(&p, midStart, midPoint, Level1::Mid(iter1->color, iter2->color), mid(iter1->width, iter2->width)));
		midPoint = midStart;
	}
}

void Level1::Shape::update(const Transform& t)
{
	Transform dif = t - vt;
	for (Polyline& p : *this)
		p.update(t);
}

void Level1::Shape::setWidth(StrokeWidth sw)
{
}

void Level1::Shape::setColor(Gdiplus::Color color)
{
	vt.brush.color = color;
	for (Polyline& p : *this)
	{
		p.setColor(color);
	}
}

void Level1::Shape::updateTransform()
{
	size_t count = 0;
	size_t a = 0, r = 0, g = 0, b = 0;
	double width = 0;
	double sumXDistance = 0, sumYDistance = 0, sumMidX = 0, sumMidY = 0;
	for (Polyline& po : *this)
	{
		po.updateTransform();
	}
	for (Polyline& po : *this)
	{
		a += po.vt.brush.color.GetA();
		r += po.vt.brush.color.GetR();
		g += po.vt.brush.color.GetG();
		b += po.vt.brush.color.GetB();
		width += toFloat(po.vt.brush.width);
		count++;
		double maxDistance = 0;
		Point midPoint;
		double maxXDivYDistance = 0, maxYDivXDistance = 0;
		float xDistance = 0, yDistance = 0;
		for (Polyline& po1 : *this)
		{
			Point dif(abs(po.vt.location.x - po1.vt.location.x), abs(po.vt.location.y - po1.vt.location.y));
			double distance = sqrt(dif.x * dif.x + dif.y * dif.y);
			if (maxDistance < distance)
			{
				maxDistance = distance;
				midPoint = mid(po.vt.location, po1.vt.location);
			}
			double xDivYDistance = dif.x * (1 / dif.y);
			double yDivYDistance = dif.y * (1 / dif.x);
			if (maxXDivYDistance < xDivYDistance)
			{
				maxXDivYDistance = xDivYDistance;
				xDistance = dif.x;
			}
			if (maxYDivXDistance < yDivYDistance)
			{
				maxYDivXDistance = yDivYDistance;
				yDistance = dif.y;
			}
		}
		sumMidX += midPoint.x;
		sumMidY += midPoint.y;
		sumXDistance += xDistance;
		sumYDistance += yDistance;
	}
	if (count > 0)
	{
		vt.brush.color = Gdiplus::Color::MakeARGB(a / count, r / count, g / count, b / count);
		vt.brush.width = width / count;
		vt.location.x = sumMidX / count + vt.locationOffset.x;
		vt.location.y = sumMidY / count + vt.locationOffset.y;
		vt.scale.x = sumXDistance / count * vt.scaleMultiplier.x;
		vt.scale.y = sumYDistance / count * vt.scaleMultiplier.y;
	}
}

void Level1::Shape::fastDraw(PixelMap* pmap)
{
	for (Polyline& pl : *this)
		pl.fastDraw(pmap);
}

void Level1::Shape::deepDraw(PixelMap* pmap)
{
	for (Polyline& pl : *this)
		pl.deepDraw(pmap);
}

void Level1::Shape::deepClear(PixelMap* pmap)
{
	for (Polyline& pl : *this)
		pl.deepClear(pmap);
}

void Level1::Shape::fastClear(PixelMap* pmap)
{
	for (Polyline& pl : *this)
		pl.fastClear(pmap);
}

void Level1::Shape::addLocation(LONG x, LONG y)
{
	for (Polyline& pl : *this)
		pl.addLocation(x, y);
	vt.location.x += x;
	vt.location.y += y;
}

void Level1::Shape::addRotation(double angel)
{
	Point location = vt.location;
	for (Polyline& p : *this)
	{
		for (Line& l : p)
		{
			Rotate(l.start, location, angel);
			Rotate(l.end, location, angel);
		}
	}
	vt.rotation += angel;
	updateTransform();
}

void Level1::Shape::addScale(double angel, double force)
{
	Point multiplier(cos(angel), sin(angel));
	Point forceVector(multiplier.x * force, multiplier.y * force);
	Point location(vt.location - vt.locationOffset);
	double maxDistance = 0;
	for (Polyline& p : *this)
	{
		double distance = MaxDistance(p, location, angel);
		if (maxDistance < distance)
			maxDistance = distance;
	}
	for (Polyline& p : *this)
	{
		for (Line& l : p)
		{
			Point p = l.start;
			Rotate(p, location, angel);
			p.x = (p.x - location.x) / maxDistance;
			l.start.x -= forceVector.x * p.x;
			l.start.y += forceVector.y * p.x;

			p = l.end;
			Rotate(p, location, angel);
			p.x = (p.x - location.x) / maxDistance;
			l.end.x -= forceVector.x * p.x;
			l.end.y += forceVector.y * p.x;
		}
	}
	updateTransform();
}

void Level1::Shape::addItemLocation(LONG x, LONG y, POINT p, StrokeWidth width)
{
	double minDistance = 999999;
	for (Polyline& pl : *this)
	{
		double distance = MinDistance(pl, p);
		if (minDistance > distance)
			minDistance = distance;
	}
	double maxDistance = 0;
	for (Polyline& pl : *this)
	{
		double distance = MaxDistance(pl, p) * toFloat(width);
		if (maxDistance < distance)
			maxDistance = distance;
	}
	for (Polyline& pl : *this)
	{
		for (Line& l : pl)
		{
			double distance1 = distance(p, l.start);
			double mult = (1 - distance1 / maxDistance) * (minDistance / distance1);
			l.start.x += x * mult;
			l.start.y += y * mult;
			distance1 = distance(p, l.end);
			mult = (1 - distance1 / maxDistance) * (minDistance / distance1);
			l.end.x += x * mult;
			l.end.y += y * mult;
		}
	}
	updateTransform();
}

void Level1::Shape::addItemRotation(double angel, POINT p, StrokeWidth width)
{
	if (angel > M_PI) angel -= M_PI * 2;
	else if (angel < -M_PI) angel += M_PI * 2;
	double minDistance = 999999;
	for (Polyline& pl : *this)
	{
		double distance = MinDistance(pl, p);
		if (minDistance > distance)
			minDistance = distance;
	}
	double maxDistance = 0;
	for (Polyline& pl : *this)
	{
		double distance = MaxDistance(pl, p) * toFloat(width);
		if (maxDistance < distance)
			maxDistance = distance;
	}
	Point location = vt.location;
	size_t count = 0;
	double rotation = 0;
	for (Polyline& pl : *this)
	{
		for (Line& l : pl)
		{
			double distance1 = distance(p, l.start);
			double add = angel * (1 - distance1 / maxDistance) * (minDistance / distance1);
			Rotate(l.start, location, add);
			rotation += add;
			distance1 = distance(p, l.end);
			add = angel * (1 - distance1 / maxDistance) * (minDistance / distance1);
			Rotate(l.end, location, add);
			rotation += add;
			count++;
		}
	}
	vt.rotation += rotation / (count << 1);
	updateTransform();
}

void Level1::Shape::addItemScale(double angel, double force, POINT p, StrokeWidth width)
{
	Point multiplier(cos(angel), sin(angel));
	Point forceVector(multiplier.x * force, multiplier.y * force);
	Point location(vt.location);
	double minDistance = 999999;
	for (Polyline& pl : *this)
	{
		double distance = MinDistance(pl, p);
		if (minDistance > distance)
			minDistance = distance;
	}
	double maxDistance = 0;
	for (Polyline& pl : *this)
	{
		double distance = MaxDistance(pl, p) * toFloat(width);
		if (maxDistance < distance)
			maxDistance = distance;
	}
	double maxDistance1 = 0;
	for (Polyline& p : *this)
	{
		double distance = MaxDistance(p, location, angel);
		if (maxDistance1 < distance)
			maxDistance1 = distance;
	}
	for (Polyline& pl : *this)
	{
		for (Line& l : pl)
		{
			Point p1 = l.start;
			Rotate(p1, location, angel);
			double distance1 = distance(p, l.start);
			double mult = (p1.x - location.x) / maxDistance1 * (1 - distance1 / maxDistance) * (minDistance / distance1);
			l.start.x -= forceVector.x * mult;
			l.start.y += forceVector.y * mult;

			p1 = l.end;
			Rotate(p1, location, angel);
			distance1 = distance(p, l.end);
			mult = (p1.x - location.x) / maxDistance1 * (1 - distance1 / maxDistance) * (minDistance / distance1);
			l.end.x -= forceVector.x * mult;
			l.end.y += forceVector.y * mult;
		}
	}
	updateTransform();
}

void Level1::Shape::addCenterLocation(LONG x, LONG y)
{
}

void Level1::Shape::addCenterRotation(double angel)
{
}

void Level1::Shape::addCenterScale(double angel, double force)
{
}

void Level1::Shape::setDefaultCenterLocation()
{
}

void Level1::Shape::setDefaultCenterRotation()
{
}

void Level1::Shape::setDefaultCenterScale()
{
}

void Level1::mid(double multiplier, Level1::TouchMap* parent, const Level1::Shape& a, const Level1::Shape& b)
{
	size_t midSize = a.size() * multiplier + b.size() * (1 - multiplier);
	double multiplier1 = a.size() / (double)midSize;
	double multiplier2 = b.size() / (double)midSize;
	parent->push_front(Level1::Shape());
	Level1::Shape& p = *parent->begin();
	long i1 = a.size() - 1, i2 = b.size() - 1;
	auto iter1 = a.crbegin();
	auto iter2 = b.crbegin();
	while (midSize--)
	{
		long i11 = midSize * multiplier1;
		long i22 = midSize * multiplier2;
		for (long j = i1; j > i11; j--)
			iter1++;
		for (long j = i2; j > i22; j--)
			iter2++;
		i1 = i11;
		i2 = i22;
		mid(multiplier, &p, *iter1, *iter2);
	}
}

void Level1::mid(Level1::TouchMap* parent, const Level1::Shape& a, const Level1::Shape& b)
{
	size_t size1 = 0, size2 = 0;
	for (const Level1::Polyline& l : a)
		size1++;
	for (const Level1::Polyline& l : b)
		size2++;
	size_t minSize = min(size1, size2);
	size_t midSize = (size1 + size2) >> 1;
	double multiplier1 = size1 / (double)midSize;
	double multiplier2 = size2 / (double)midSize;
	parent->push_front(Level1::Shape());
	Level1::Shape& p = *parent->begin();
	long i1 = 0, i2 = 0;
	auto iter1 = a.cbegin();
	auto iter2 = b.cbegin();
	for (size_t i = 0; i < minSize; i++)
	{
		long i11 = i;
		long i22 = i;
		iter1 = a.cbegin();
		for (long j = 0; j < i11; j++)
			iter1++;
		iter2 = b.cbegin();
		for (long j = 0; j < i22; j++)
			iter2++;
		i1 = i11;
		i2 = i22;
		mid(&p, *iter1, *iter2);
	}
}

void Level1::TouchMap::fastDraw(PixelMap* pmap)
{
	for (Shape& s : *this)
	{
		s.fastDraw(pmap);
	}
}

void Level1::TouchMap::deepDraw(PixelMap* pmap)
{
	for (Shape& s : *this)
	{
		s.deepDraw(pmap);
	}
}

void Level1::TouchMap::deepClear(PixelMap* pmap)
{
	for (Shape& s : *this)
	{
		s.deepClear(pmap);
	}
}

void Level1::TouchMap::fastClear(PixelMap* pmap)
{
	for (Shape& s : *this)
	{
		s.fastClear(pmap);
	}
}

Level1::TouchMap Level1::mid(double multiplier, const TouchMap& a, const TouchMap& b)
{
	TouchMap t;
	size_t midSize = a.size() * multiplier + b.size() * (1 - multiplier);
	double multiplier1 = a.size() / (double)midSize;
	double multiplier2 = b.size() / (double)midSize;
	t.push_front(Level1::Shape());
	Level1::Shape& p = *t.begin();
	long i1 = a.size() - 1, i2 = b.size() - 1;
	auto iter1 = a.crbegin();
	auto iter2 = b.crbegin();
	while (midSize--)
	{
		long i11 = midSize * multiplier1;
		long i22 = midSize * multiplier2;
		for (long j = i1; j > i11; j--)
			iter1++;
		for (long j = i2; j > i22; j--)
			iter2++;
		i1 = i11;
		i2 = i22;
		mid(multiplier, &t, *iter1, *iter2);
	}
	return t;
}

Level1::TouchMap Level1::mid(const TouchMap& a, const TouchMap& b)
{
	TouchMap t;
	size_t size1 = 0, size2 = 0;
	for (const Level1::Shape& l : a)
		size1++;
	for (const Level1::Shape& l : b)
		size2++;
	size_t minSize = min(size1, size2);
	size_t midSize = (size1 + size2) >> 1;
	double multiplier1 = size1 / (double)midSize;
	double multiplier2 = size2 / (double)midSize;
	t.push_front(Level1::Shape());
	Level1::Shape& p = *t.begin();
	long i1 = 0, i2 = 0;
	auto iter1 = a.cbegin();
	auto iter2 = b.cbegin();
	for (size_t i = 0; i < minSize; i++)
	{
		long i11 = i;
		long i22 = i;
		for (long j = 0; j < i11; j++)
			iter1++;
		for (long j = 0; j < i22; j++)
			iter2++;
		i1 = i11;
		i2 = i22;
		mid(&t, *iter1, *iter2);
	}
	return t;
}

Level1::Frame::Frame(ULONG w, ULONG h, Gdiplus::Color color)
{
	b = new Gdiplus::Bitmap(w, h);
	pm = new PixelMap(w, h);

	/*tm.push_front(Shape());
	Shape& mainShape = *tm.begin();
	mainShape.vt.brush.type = BrushInfo::NONE;
	mainShape.push_front(Polyline(&mainShape));
	Polyline& mainPolyline = *mainShape.begin();
	backRound = &mainPolyline;
	mainPolyline.vt.brush.type = BrushInfo::NONE;
	POINT start;
	start.x = 0;
	start.y = 0;
	POINT end;
	end.x = w - 1;
	end.y = 0;
	for (Pixel& pi : *pm)
	{
		pi.push_front(Line(&mainPolyline, start, end, Gdiplus::Color::White, 1));
		start.y++;
		end.y++;
	}
	pm->deepDraw(b);
	deselect();*/
}

Level1::Frame::~Frame()
{
}

void Level1::Frame::deselect()
{
	for (auto i : selection)
		i->vt.brush.type = BrushInfo::NONE;
	selection.clear();
}

void Level1::Frame::startSelecting(POINT p, KeyInfo ki, HDC hdc)
{
	if (!pm->get(p.x, p.y).empty())
		oneSelection.push_front(&pm->get(p.x, p.y));
	if (!ki.get(KeyInfo::SHIFT))
		deselect();
	pm->deepDraw(b);
	Gdiplus::Graphics g(hdc);
	g.DrawImage(b, 0, 0);
}

void Level1::Frame::select(POINT p)
{
	if (!pm->get(p.x, p.y).empty())
		oneSelection.push_front(&pm->get(p.x, p.y));
}

void Level1::Frame::endSelecting(KeyInfo ki)
{
	if (!oneSelection.empty())
	{
		if (!selection.empty())
			(*selection.begin())->vt.brush.type = BrushInfo::SELECTION;
		Pixel* pi = *oneSelection.begin();
		for (Pixel* p : oneSelection)
		{
			if (toFloat(pi->begin()->width) > toFloat(p->begin()->width))
			{
				pi = p;
			}
		}
		Pixel& pixel = *pi;
		if (ki.get(KeyInfo::CTRL))
		{
			isLastSelectionPolyline = false;
			if (ki.get(KeyInfo::ALT))
			{
				for (Line& l : pixel)
				{
					l.parent->parent->vt.brush.type = BrushInfo::SELECTION;
					selection.push_front(l.parent->parent);
				}
				pixel.begin()->parent->parent->vt.brush.type = BrushInfo::MAIN_SELECTION;
			}
			else
			{
				selection.push_front(pixel.begin()->parent->parent);
				pixel.begin()->parent->parent->vt.brush.type = BrushInfo::MAIN_SELECTION;
			}
		}
		else
		{
			isLastSelectionPolyline = true;
			if (ki.get(KeyInfo::ALT))
			{
				for (Line& l : pixel)
				{
					l.parent->vt.brush.type = BrushInfo::SELECTION;
					selection.push_front(l.parent);
				}
				pixel.begin()->parent->vt.brush.type = BrushInfo::MAIN_SELECTION;
			}
			else
			{
				selection.push_front(pixel.begin()->parent);
				pixel.begin()->parent->vt.brush.type = BrushInfo::MAIN_SELECTION;
			}
		}
		oneSelection.clear();
	}
	pm->deepDraw(b);
}

void Level1::Frame::startDrawing()
{
	if (selection.empty())
	{
		tm.push_front(Shape());
		Shape& s = (*tm.begin());
		s.push_front(Polyline(&s));
	}
	else
	{
		if (isLastSelectionPolyline)
		{
			deselect();
			tm.push_front(Shape());
			Shape& s = (*tm.begin());
			s.push_front(Polyline(&s));
		}
		else
		{
			Shape& s = *(Shape*)*selection.begin();
			s.push_front(Polyline(&s));
		}
	}
}

void Level1::Frame::startEditing()
{
	for (ITransformable* tr : selection)
	{
		tr->deepClear(pm);
	}
}

void Level1::Frame::draw(Gdiplus::Color color, POINT start, POINT end, StrokeWidth width, HDC hdc)
{
	if (selection.empty())
	{
		POINT dif;
		dif.x = end.x - start.x;
		dif.y = end.y - start.y;
		Polyline& pol = *tm.begin()->begin();
		Line l = Line(&pol, start, end, color, width);
		pol.push_front(l);
		l.fastDraw(pm);
		pm->fastDraw(b, sqrt(dif.x * dif.x + dif.y * dif.y) + 1);
		Gdiplus::Graphics g(hdc);
		g.DrawImage(b, 0, 0);
	}
	else
	{
		POINT dif;
		dif.x = end.x - start.x;
		dif.y = end.y - start.y;
		Polyline& pol = *((Shape*)*selection.begin())->begin();
		Line l = Line(&pol, start, end, color, width);
		pol.push_front(l);
		l.fastDraw(pm);
		pm->fastDraw(b, sqrt(dif.x * dif.x + dif.y * dif.y) + 1);
		Gdiplus::Graphics g(hdc);
		g.DrawImage(b, 0, 0);
	}
}

void Level1::Frame::edit(EditMode mode, POINT start, POINT end, StrokeWidth width, HDC hdc)
{
	if (!selection.empty())
	{
		POINT dif;
		dif.x = end.x - start.x;
		dif.y = end.y - start.y;
		switch (mode)
		{
		case Level1::LOCATION:
			for (ITransformable* tr : selection)
				tr->addLocation(dif.x, dif.y);
			break;
		case Level1::ROTATION:
			for (ITransformable* tr : selection)
				tr->addRotation(atan2(end.y - (*selection.begin())->vt.location.y, end.x - (*selection.begin())->vt.location.x)
					- atan2(start.y - (*selection.begin())->vt.location.y, start.x - (*selection.begin())->vt.location.x));
			break;
		case Level1::SCALE:
			for (ITransformable* tr : selection)
				tr->addScale(-atan2((end.y + start.y) / 2.0 - (*selection.begin())->vt.location.y, (end.x + start.x) / 2.0 - (*selection.begin())->vt.location.x),
					distance(start, (*selection.begin())->vt.location) - distance(end, (*selection.begin())->vt.location));
			break;
		case Level1::ITEM_LOCATION:
			for (ITransformable* tr : selection)
				tr->addItemLocation(dif.x, dif.y, start, 1);
			break;
		case Level1::ITEM_ROTATION:
			for (ITransformable* tr : selection)
				tr->addItemRotation(atan2(end.y - (*selection.begin())->vt.location.y, end.x - (*selection.begin())->vt.location.x)
					- atan2(start.y - (*selection.begin())->vt.location.y, start.x - (*selection.begin())->vt.location.x), start, 1);
			break;
		case Level1::ITEM_SCALE:
			for (ITransformable* tr : selection)
				tr->addItemScale(-atan2((end.y + start.y) / 2.0 - (*selection.begin())->vt.location.y, (end.x + start.x) / 2.0 - (*selection.begin())->vt.location.x),
					distance(start, (*selection.begin())->vt.location) - distance(end, (*selection.begin())->vt.location),
					start, 1);
			break;
		case Level1::CENTER_LOCATION:
			break;
		case Level1::CENTER_ROTATION:
			break;
		case Level1::CENTER_SCALE:
			break;
		}
		for (ITransformable* tr : selection)
			tr->fastDraw(pm);
		pm->fastDraw(b, sqrt(dif.x * dif.x + dif.y * dif.y) + 1);
		for (ITransformable* tr : selection)
			tr->fastClear(pm);
		Gdiplus::Graphics g(hdc);
		g.DrawImage(b, 0, 0);
	}
}

void Level1::Frame::endDrawing()
{
	tm.begin()->deepDraw(pm);
	tm.begin()->updateTransform();
	pm->deepDraw(b);
}

void Level1::Frame::endEditing()
{
	for (ITransformable* tr : selection)
	{
		tr->deepDraw(pm);
	}
	pm->deepDraw(b);
}

Level1::Frame Level1::mid(double multiplier, const Frame& a, const Frame& b)
{
	Frame f(a.pm->getW(), a.pm->getH(), NULL);
	f.tm = mid(multiplier, a.tm, b.tm);
	return f;
}

Level1::Frame Level1::mid(const Frame& a, const Frame& b)
{
	Frame f(a.pm->getW(), a.pm->getH(), NULL);
	f.tm = mid(a.tm, b.tm);
	return f;
}

Level1::Animation::Animation(ULONG w, ULONG h)
{
	push_front(Frame(w, h, Gdiplus::Color::Red));
	this->w = w;
	this->h = h;
}

void Level1::Animation::paint(HDC hdc)
{
	Gdiplus::Graphics g(hdc);
	g.DrawImage(get().b, 0, 0);
}

void Level1::Animation::mouseDown(MouseButton mb, HWND& hwnd)
{
	if (params.hasStarted)
		mouseUp(hwnd);
	randSeed++;
	if (textBoxes.isValid())
	{
		text.set(textBoxes);
		textBoxes.destroy();
		Transform t = text.get();
		params.color = t.brush.color;
		params.width = t.brush.width;
		//selectedPixel->begin()->parent->setColor(params.color);
		Polyline* pl = selectedPixel->begin()->parent;
		if (pl != nullptr)
		{
			pl->deepClear(get().pm);
			pl->updateTransform();
			pl->update(t);
			pl->updateTransform();
			pl->deepDraw(get().pm);
		}
	}
	//SendMessage(widthText, WM_GETTEXT, 80, LPARAM(str));
	//params.width = _wtoi(str);
	params.hasStarted = true;
	GetCursorPos(&params.start);
	ScreenToClient(hwnd, &params.start);
	this->mb = mb;
	if (mb == Level1::LEFT)
	{
		if (params.isDrawingMode)
		{
			get().startDrawing();
		}
		else
		{
			get().startEditing();
		}
	}
	else
	{
		if (mb == MIDDLE)
		{
			HDC hdc = GetDC(hwnd);
			get().startSelecting(params.start, params.key, hdc);
			ReleaseDC(hwnd, hdc);
		}
		else
		{
			selectedPixel = &get().pm->get(params.start.x, params.start.y);
			if (selectedPixel->empty())
			{
				Transform t;
				t.brush.color = Gdiplus::Color::MakeARGB(255, 255, 0, 0);
				t.brush.width = 1;
				text.set(t);
				textBoxes.create(hwnd, params.start);
				text.get(textBoxes);
			}
			else
			{
				text.set(selectedPixel->begin()->parent->vt);
				textBoxes.create(hwnd, params.start);
				text.get(textBoxes);
			}
		}
	}
}

void Level1::Animation::mouseMove(HWND& hwnd)
{
	/*DestroyWindow(midFrameStatic);
	DestroyWindow(midFrameEdit);
	midFrameStatic = NULL;
	midFrameEdit = NULL;*/
	if (params.hasStarted)
	{
		HDC hdc = GetDC(hwnd);
		GetCursorPos(&params.end);
		ScreenToClient(hwnd, &params.end);
		UINT32 pressure = 512;
		POINTER_PEN_INFO ppi;
		for (UINT32 i = 0; i < 256; i++)
			if (GetPointerPenInfo(i, &ppi))
			{
				pressure = ppi.pressure;
				break;
			}
		StrokeWidth width = max(toFloat(params.width) * (pressure / 512.0), 0.5);
		if (mb == Level1::LEFT)
		{
			if (params.isDrawingMode)
			{
				get().draw(params.color, params.start, params.end, width, hdc);
			}
			else
			{
				get().edit(params.mode, params.start, params.end, width, hdc);
			}
		}
		else
		{
			if (mb == MIDDLE)
			{
				get().select(params.start);
			}
			else
			{
			}
		}
		params.start = params.end;
		ReleaseDC(hwnd, hdc);
	}
}

void Level1::Animation::mouseUp(HWND& hwnd)
{
	HDC hdc = GetDC(hwnd);
	params.hasStarted = false;
	if (mb == Level1::LEFT)
	{
		if (params.isDrawingMode)
		{
			get().endDrawing();
		}
		else
		{
			get().endEditing();
		}
	}
	else
	{
		if (mb == MIDDLE)
		{
			get().endSelecting(params.key);
		}
		else
		{
		}
	}
	Gdiplus::Graphics g(hdc);
	g.DrawImage(get().b, 0, 0);
	ReleaseDC(hwnd, hdc);
}

Level1::Frame& Level1::Animation::get()
{
	auto iter = begin();
	for (size_t i = 0; i < params.frameIndex; i++)
		iter++;
	return *iter;
}

void Level1::Animation::next(HWND& hwnd)
{
	HDC hdc = GetDC(hwnd);
	if (params.key.get(KeyInfo::SHIFT))
	{
		if (params.frameIndex < size() - 2)
		{
			params.frameIndex += 2;
			paint(hdc);
		}
		else if (params.frameIndex < size() - 1)
		{
			params.frameIndex++;
			paint(hdc);
		}
	}
	else
	{
		if (params.frameIndex < size() - 1)
		{
			params.frameIndex++;
			paint(hdc);
		}
	}
	ReleaseDC(hwnd, hdc);
}

void Level1::Animation::previous(HWND& hwnd)
{
	HDC hdc = GetDC(hwnd);
	if (params.key.get(KeyInfo::SHIFT))
	{
		if (params.frameIndex > 1)
		{
			params.frameIndex -= 2;
			paint(hdc);
		}
		else if (params.frameIndex > 0)
		{
			params.frameIndex--;
			paint(hdc);
		}
	}
	else
	{
		if (params.frameIndex > 0)
		{
			params.frameIndex--;
			paint(hdc);
		}
	}
	ReleaseDC(hwnd, hdc);
}

void Level1::Animation::addMid(HWND& hwnd, ULONG i)
{
	if (params.frameIndex < size() - 1)
	{
		if (params.key.get(KeyInfo::SHIFT))
		{
			HDC hdc = GetDC(hwnd);
			Frame& f1 = get();
			params.frameIndex++;
			Frame& f2 = get();
			auto iter = begin();
			for (size_t i = 0; i < params.frameIndex; i++)
				iter++;
			insert(iter, mid(1 / 3.0, f1, f2));
			get().tm.deepDraw(get().pm);
			get().pm->deepDraw(get().b);
			iter = begin();
			for (size_t i = 0; i < params.frameIndex; i++)
				iter++;
			insert(iter, mid(2 / 3.0, f1, f2));
			get().tm.deepDraw(get().pm);
			get().pm->deepDraw(get().b);
			paint(hdc);
			ReleaseDC(hwnd, hdc);
		}
		else
		{
			HDC hdc = GetDC(hwnd);
			Frame& f1 = get();
			params.frameIndex++;
			Frame& f2 = get();
			auto iter = begin();
			for (size_t i = 0; i < params.frameIndex; i++)
				iter++;
			insert(iter, mid(0.5, f1, f2));
			get().tm.deepDraw(get().pm);
			get().pm->deepDraw(get().b);
			paint(hdc);
			ReleaseDC(hwnd, hdc);
		}
	}
}

void Level1::Animation::add(HWND& hwnd)
{
	HDC hdc = GetDC(hwnd);
	auto iter = begin();
	for (size_t i = 0; i < params.frameIndex; i++)
		iter++;
	iter++;
	insert(iter, Frame(w, h, NULL));
	params.frameIndex++;
	get().pm->deepDraw(get().b);
	paint(hdc);
	ReleaseDC(hwnd, hdc);
}

void Level1::Animation::copy(HWND& hwnd)
{
	//String s = ToString(*get().tm.begin());
	//ToShape(s, &get().tm);
	//Shape sh = *get().tm.begin();
	//String s1 = ToString(sh);
	//if (s == s1)
	//	exit(0);
	/*if (IsClipboardFormatAvailable(CF_TEXT))
	{
		if (OpenClipboard(hwnd))
		{

		}
	}*/
}

void Level1::Animation::del(HWND& hwnd)
{
	if (get().isLastSelectionPolyline and !get().selection.empty())
	{
		((Polyline*)(*get().selection.begin()))->deepClear(get().pm);
		((Polyline*)(*get().selection.begin()))->parent->remove(*((Polyline*)(*get().selection.begin())));
		get().pm->deepDraw(get().b);
		HDC hdc = GetDC(hwnd);
		paint(hdc);
		ReleaseDC(hwnd, hdc);
	}
}
