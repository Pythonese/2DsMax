#pragma once
#include "class_forward_declarations.h"
#include <string>
namespace Level1
{
	struct String
	{
		String();
		String(size_t n, const char* s);
		String(size_t n);
		String(const String& s);
		String(String&& s);
		~String();
		friend bool operator==(const String& a, const String& b);
		BYTE* begin();
		BYTE* end();
		BYTE* data;
		size_t size() const;
		BYTE& operator[](size_t i);
		const BYTE& operator[](size_t i) const;
		size_t n;
	};
	struct Hex
	{
		void updateChars();
		wchar_t data[8];
		Hex();
		Hex(wchar_t a0, wchar_t a1, wchar_t a2, wchar_t a3, wchar_t a4, wchar_t a5, wchar_t a6, wchar_t a7);
		friend bool operator==(const Hex& a, const Hex& b);
	};
	enum MouseButton
	{
		LEFT, MIDDLE, RIGHT
	};
	enum EditMode
	{
		LOCATION, ROTATION, SCALE,
		ITEM_LOCATION, ITEM_ROTATION, ITEM_SCALE,
		CENTER_LOCATION, CENTER_ROTATION, CENTER_SCALE
	};
	class Bitset64
	{
	protected:
		size_t data = 0;
	public:
		void set(BYTE i, bool b);
		bool get(BYTE i);
	};
	class KeyInfo : public Bitset64
	{
	public:
		enum Key
		{
			ALT, CTRL, SHIFT,
		};
	};
	struct StrokeWidth
	{
		UINT16 u;
		UINT8 b;
		StrokeWidth() : u(0), b(0) {}
		StrokeWidth(float f) : u(f), b((f - u) * 255) {}
		friend StrokeWidth operator/(StrokeWidth width, float f);
		void operator/=(float f);
		friend float toFloat(StrokeWidth width);
		friend bool operator==(const StrokeWidth& a, const StrokeWidth& b);
		friend StrokeWidth mid(double m, const StrokeWidth& a, const StrokeWidth& b);
		friend StrokeWidth mid(const StrokeWidth& a, const StrokeWidth& b);
	};
	struct Point
	{
		float x, y;
		Point() : x(0), y(0) {}
		Point(float x, float y) : x(x), y(y) {}
		Point(POINT p) : x(p.x), y(p.y) {}
		friend Point operator+(const Point& a, const Point& b);
		friend Point operator-(const Point& a, const Point& b);
		friend Point operator*(const Point& a, const Point& b);
		friend Point operator/(const Point& a, const Point& b);
		friend Point mid(const Point& a, const Point& b);
		friend Point mid(double m, const Point a, const Point b);
		void operator+=(const Point& a);
		void operator-=(const Point& a);
		void operator*=(const Point& a);
		void operator/=(const Point& a);
		void mid(const Point& a);
		friend float distance(Point a, const Point& b);
		friend float distance(Point a, const Point& b, Point dif);
		void set(HWND hwnd);
		void get(POINT& p);
		friend bool operator==(const Point& a, const Point& b);
		friend bool operator!=(const Point& a, const Point& b);
	};
	struct PointText
	{
		wchar_t data[9];
		void set(Point p);
		Point get();
		void updateChars();
	};
	struct BrushInfo
	{
		StrokeWidth width;
		Gdiplus::Color color;
		enum Type
		{
			NONE, SELECTION, MAIN_SELECTION
		};
		Type type = NONE;
	};
	struct Transform
	{
		Point location, locationOffset;
		double rotation = 0, rotationOffset = 0;
		Point scale, scaleMultiplier = Point(1, 1);
		BrushInfo brush;
		friend Transform operator+(const Transform& a, const Transform& b);
		friend Transform operator-(const Transform& a, const Transform& b);
		friend Transform mid(const Transform& a, const Transform& b);
		void operator+=(const Transform& a);
		void operator-=(const Transform& a);
		void mid(const Transform& a);
	};
	struct TransformText
	{
		PointText worldLocation, localLocation;
		wchar_t worldRotation[3], localRotation[3];
		PointText worldScale, localScale;
		Hex color;
		wchar_t width[3];
		void set(const Transform& t);
		void set(const TransformTextBoxes& textBoxes);
		Transform get();
		void get(TransformTextBoxes& textBoxes);
	};
	struct TransformTextBoxes
	{
		HWND colorStatic = NULL,
			widthStatic = NULL,
			worldLocationStatic = NULL,
			localLocationStatic = NULL,
			worldRotationStatic = NULL,
			localRotationStatic = NULL,
			worldScaleStatic = NULL,
			localScaleStatic = NULL,

			colorEdit = NULL,
			widthEdit = NULL,
			worldLocationEdit = NULL,
			localLocationEdit = NULL,
			worldRotationEdit = NULL,
			localRotationEdit = NULL,
			worldScaleEdit = NULL,
			localScaleEdit = NULL;
		void create(HWND& hwnd, POINT p);
		bool isValid();
		void destroy();
	};
	class Pixel : public std::list<Line>
	{
	public:
		Gdiplus::Color fastGetColor() const;
		Gdiplus::Color getColor() const;
		void push(const Line& p);
		void del(Polyline* parent);
	};
	class PixelMap : public Array2D<Pixel>
	{
	public:
		PixelMap(ULONG x, ULONG y) : Array2D<Pixel>(x, y) {}
		void drawLine(Gdiplus::Bitmap* b, ULONG y);
		void fastDraw(Gdiplus::Bitmap* b, ULONG step);
		void deepDraw(Gdiplus::Bitmap* b);
	};

	class ITouchObj
	{
	public:
		virtual void fastDraw(PixelMap* pmap) = 0;
		virtual void deepDraw(PixelMap* pmap) = 0;
		virtual void deepClear(PixelMap* pmap) = 0;
		virtual void fastClear(PixelMap* pmap) = 0;
	};
	class Line : public ITouchObj
	{
	public: // No moveToNewParent(std::list<Line*> lines)
		Line(Polyline* parent, const Point& start, const Point& end, const Gdiplus::Color& color, StrokeWidth width)
			: parent(parent), start(start), end(end), color(color), width(width) {}
		Polyline* parent;
		Point start, end;
		Gdiplus::Color color;
		StrokeWidth width;
		void fastDraw(PixelMap* pmap);
		void deepDraw(PixelMap* pmap);
		void deepClear(PixelMap* pmap);
		void fastClear(PixelMap* pmap);
		friend bool operator==(const Line& a, const Line& b);

		friend void mid(double m, Polyline* p, const Line& a, const Line& b);
		friend void mid(Polyline* p, const Line& a, const Line& b);
	};
	class ITransformable : public ITouchObj
	{
	public:
		Transform vt;
		virtual void update(const Transform& t) = 0;
		virtual void setWidth(StrokeWidth sw) = 0;
		virtual void setColor(Gdiplus::Color color) = 0;
		virtual void updateTransform() = 0;

		virtual void addLocation(LONG x, LONG y) = 0;
		virtual void addRotation(double angel) = 0;
		virtual void addScale(double angel, double force) = 0;

		virtual void addItemLocation(LONG x, LONG y, POINT p, StrokeWidth width) = 0;
		virtual void addItemRotation(double angel, POINT p, StrokeWidth width) = 0;
		virtual void addItemScale(double angel, double force, POINT p, StrokeWidth width) = 0;

		virtual void addCenterLocation(LONG x, LONG y) = 0;
		virtual void addCenterRotation(double angel) = 0;
		virtual void addCenterScale(double angel, double force) = 0;

		virtual void setDefaultCenterLocation() = 0;
		virtual void setDefaultCenterRotation() = 0;
		virtual void setDefaultCenterScale() = 0;
	};
	class Polyline : public ITransformable, public std::list<Line>
	{
	public:
		Shape* parent;
		Polyline(Shape* parent) : parent(parent) {}
		void update(const Transform& t);
		void setWidth(StrokeWidth sw);
		void setColor(Gdiplus::Color color);
		void updateTransform();

		void fastDraw(PixelMap* pmap);
		void deepDraw(PixelMap* pmap);
		void deepClear(PixelMap* pmap);
		void fastClear(PixelMap* pmap);

		void addLocation(LONG x, LONG y);
		void addRotation(double angel);
		void addScale(double angel, double force);

		void addItemLocation(LONG x, LONG y, POINT p, StrokeWidth width);
		void addItemRotation(double angel, POINT p, StrokeWidth width);
		void addItemScale(double angel, double force, POINT p, StrokeWidth width);

		void addCenterLocation(LONG x, LONG y);
		void addCenterRotation(double angel);
		void addCenterScale(double angel, double force);

		void setDefaultCenterLocation();
		void setDefaultCenterRotation();
		void setDefaultCenterScale();

		friend void mid(double multiplier, Shape* parent, const Polyline& a, const Polyline& b);
		friend void mid(Shape* parent, const Polyline& a, const Polyline& b);
	};
	class Shape : public ITransformable, public std::list<Polyline>
	{
	public:
		void update(const Transform& t);
		void setWidth(StrokeWidth sw);
		void setColor(Gdiplus::Color color);
		void updateTransform();

		void fastDraw(PixelMap* pmap);
		void deepDraw(PixelMap* pmap);
		void deepClear(PixelMap* pmap);
		void fastClear(PixelMap* pmap);

		void addLocation(LONG x, LONG y);
		void addRotation(double angel);
		void addScale(double angel, double force);

		void addItemLocation(LONG x, LONG y, POINT p, StrokeWidth width);
		void addItemRotation(double angel, POINT p, StrokeWidth width);
		void addItemScale(double angel, double force, POINT p, StrokeWidth width);

		void addCenterLocation(LONG x, LONG y);
		void addCenterRotation(double angel);
		void addCenterScale(double angel, double force);

		void setDefaultCenterLocation();
		void setDefaultCenterRotation();
		void setDefaultCenterScale();

		friend void mid(double multiplier, TouchMap* parent, const Shape& a, const Shape& b);
		friend void mid(TouchMap* parent, const Shape& a, const Shape& b);
	};
	class TouchMap : public ITouchObj, public std::list<Shape>
	{
	public:
		void fastDraw(PixelMap* pmap);
		void deepDraw(PixelMap* pmap);
		void deepClear(PixelMap* pmap);
		void fastClear(PixelMap* pmap);
		friend TouchMap mid(double multiplier, const TouchMap& a, const TouchMap& b);
		friend TouchMap mid(const TouchMap& a, const TouchMap& b);
	};
	class Frame
	{
	public:
		bool isLastSelectionPolyline = true;
		Gdiplus::Bitmap* b;
		PixelMap* pm;
		TouchMap tm;
		std::list<ITransformable*> selection;
		std::list<Pixel*> oneSelection;
		Polyline* backRound;
		Frame(ULONG w, ULONG h, Gdiplus::Color color);
		~Frame();
		void deselect();
		void startSelecting(POINT p, KeyInfo ki, HDC hdc);
		void select(POINT p);
		void endSelecting(KeyInfo ki);
		void startDrawing();
		void startEditing();
		void draw(Gdiplus::Color color, POINT start, POINT end, StrokeWidth width, HDC hdc);
		void edit(EditMode mode, POINT start, POINT end, StrokeWidth width, HDC hdc);
		void endDrawing();
		void endEditing();
		friend Frame mid(double multiplier, const Frame& a, const Frame& b);
		friend Frame mid(const Frame& a, const Frame& b);
	};
	class Animation : public std::list<Frame>
	{
		MouseButton mb = LEFT;
		struct Params
		{
			Gdiplus::Color color = Gdiplus::Color::Blue;
			StrokeWidth width = 9;
			EditMode mode = LOCATION;
			bool isDrawingMode = true;
			POINT start, end;
			bool hasStarted = false;
			size_t frameIndex = 0;
			KeyInfo key;
		};
	public:
		Params params;
		Pixel* selectedPixel;
		TransformText text;
		TransformTextBoxes textBoxes;
		ULONG w, h;
		Animation(ULONG w, ULONG h);
		void paint(HDC hdc);
		void mouseDown(MouseButton mb, HWND& hwnd);
		void mouseMove(HWND& hwnd);
		void mouseUp(HWND& hwnd);
		Frame& get();
		void next(HWND& hwnd);
		void previous(HWND& hwnd);
		void addMid(HWND& hwnd, ULONG i);
		void add(HWND& hwnd);
		void copy(HWND& hwnd);
		void del(HWND& hwnd);
	};
}