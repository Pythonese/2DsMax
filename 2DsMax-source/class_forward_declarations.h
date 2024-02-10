#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <corecrt_math_defines.h>
#include <corecrt_math.h>
#include <list>
#include "array2D.h"
namespace Level1
{
	struct String;
	struct Hex;

	enum MouseButton;
	enum Mode;

	class Bitset64;
	class KeyInfo;

	struct StrokeWidth;
	struct Point;
	struct PointText;
	struct BrushInfo;
	struct Transform;
	struct TransformText;
	struct TransformTextBoxes;

	class Pixel;
	class PixelMap;

	class ITouchObj;
	class Line;
	class ITransformable;
	class Polyline;
	class Shape;
	class TouchMap;

	class Frame;
	class Animation;
}