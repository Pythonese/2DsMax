/*#include "_2DsMax.h"
void _2DsMax::AddLocation(Array2D<Pixel>& pixels, POINT location, POINT start, POINT end, size_t polyline, POINT delta)
{
    Pixel& pi = pixels.get(location.x, location.y);
    PointInfo* found = nullptr;
    for (PointInfo& po : pi)
        if (po.polyline == polyline)
            found = &po;
    if (found != nullptr)
    {
        pixels.get(location.x + delta.x, location.y + delta.y).push_front(*found);
        pi.remove(*found);
    }
}

void _2DsMax::Frame::drawCircle(const Data& data)
{
    double step = M_PI / 2 / data.doubleWidth;
    Gdiplus::Color color1 = Gdiplus::Color::MakeARGB(data.byteWidth, data.color.GetR(), data.color.GetG(), data.color.GetB());
    for (double i = 0; i < M_PI; i += step)
    {
        for (int j = 0; j < data.intWidth; j++)
        {
            get(data.start.x + j * sin(i), data.start.y + j * cos(i)).push(PointInfo(data.color, data.polyline));
            get(data.start.x + j * sin(i), data.start.y + j * -cos(i)).push(PointInfo(data.color, data.polyline));
            get(data.start.x + j * -sin(i), data.start.y + j * cos(i)).push(PointInfo(data.color, data.polyline));
            get(data.start.x + j * -sin(i), data.start.y + j * -cos(i)).push(PointInfo(data.color, data.polyline));
        }
    }
    if (data.byteWidth != 0)
    {
        for (double i = 0; i < M_PI; i += step)
        {
            get(data.start.x + data.intWidth * sin(i), data.start.y + data.intWidth * cos(i)).push(PointInfo(color1, data.polyline));
            get(data.start.x + data.intWidth * sin(i), data.start.y + data.intWidth * -cos(i)).push(PointInfo(color1, data.polyline));
            get(data.start.x + data.intWidth * -sin(i), data.start.y + data.intWidth * cos(i)).push(PointInfo(color1, data.polyline));
            get(data.start.x + data.intWidth * -sin(i), data.start.y + data.intWidth * -cos(i)).push(PointInfo(color1, data.polyline));
        }
    }
}

void _2DsMax::Frame::drawLine(const Data& data)
{
    Data dataCopy = data;
    if (data.start.x == data.end.x)
    {
        if (dataCopy.start.y < dataCopy.end.y)
        {
            while (dataCopy.start.y < dataCopy.end.y)
            {
                drawCircle(dataCopy);
                dataCopy.start.y++;
            }
        }
        else
        {
            while (dataCopy.start.y > dataCopy.end.y)
            {
                drawCircle(dataCopy);
                dataCopy.start.y--;
            }
        }
    }
    if (data.start.y == data.end.y)
    {
        if (dataCopy.start.x < dataCopy.end.x)
        {
            while (dataCopy.start.x < dataCopy.end.x)
            {
                drawCircle(dataCopy);
                dataCopy.start.x++;
            }
        }
        else
        {
            while (dataCopy.start.x > dataCopy.end.x)
            {
                drawCircle(dataCopy);
                dataCopy.start.x--;
            }
        }
    }
    else
    {
        int differenceX = dataCopy.end.x - dataCopy.start.x;
        int differenceY = dataCopy.end.y - dataCopy.start.y;
        int absX = abs(differenceX);
        int absY = abs(differenceY);
        if (absX > absY)
        {
            const double y = (double)differenceY / absX;
            double iterY = dataCopy.start.y;
            if (dataCopy.start.x < dataCopy.end.x)
            {
                while (dataCopy.start.x < dataCopy.end.x)
                {
                    drawCircle(dataCopy);
                    dataCopy.start.x++;
                    iterY += y;
                    dataCopy.start.y = iterY;
                }
            }
            else
            {
                while (dataCopy.start.x > dataCopy.end.x)
                {
                    drawCircle(dataCopy);
                    dataCopy.start.x--;
                    iterY += y;
                    dataCopy.start.y = iterY;
                }
            }
        }
        else
        {
            const double x = (double)differenceX / absY;
            double iterX = dataCopy.start.x;
            if (dataCopy.start.y < dataCopy.end.y)
            {
                while (dataCopy.start.y < dataCopy.end.y)
                {
                    drawCircle(dataCopy);
                    iterX += x;
                    dataCopy.start.x = iterX;
                    dataCopy.start.y++;
                }
            }
            else
            {
                while (dataCopy.start.y > dataCopy.end.y)
                {
                    drawCircle(dataCopy);
                    iterX += x;
                    dataCopy.start.x = iterX;
                    dataCopy.start.y--;
                }
            }
        }
    }
}

void _2DsMax::Frame::startTransform()
{
    frameCopy = this->copy();
}

void _2DsMax::Frame::transform(const Data& data, TRANSFORM_CALLBACK callBack, size_t step)
{
    POINT delta;
    delta.x = data.end.x - data.start.x;
    delta.y = data.end.y - data.start.y;
    POINT location;
    if (delta.x < 0)
    {
        if (delta.y < 0)
        {
            const LONG endX = getW(), endY = getH();
            for (size_t polyline : polylines)
                for (location.x = -delta.x; location.x < endX; location.x += step)
                    for (location.y = -delta.y; location.y < endY; location.y += step)
                        callBack(*frameCopy, location, data.start, data.end, polyline, delta);
        }
        else
        {
            const LONG endX = getW();
            for (size_t polyline : polylines)
                for (location.x = -delta.x; location.x < endX; location.x += step)
                    for (location.y = (LONG)getH() - delta.y - 1; location.y > 0; location.y -= step)
                        callBack(*frameCopy, location, data.start, data.end, polyline, delta);
        }
    }
    else
    {
        if (delta.y < 0)
        {
            const LONG endY = getH();
            for (size_t polyline : polylines)
                for (location.x = (LONG)getW() - delta.x - 1; location.x > 0; location.x -= step)
                    for (location.y = -delta.y; location.y < endY; location.y += step)
                        callBack(*frameCopy, location, data.start, data.end, polyline, delta);
        }
        else
        {
            for (size_t polyline : polylines)
                for (location.x = (LONG)getW() - delta.x - 1; location.x > 0; location.x -= step)
                    for (location.y = (LONG)getH() - delta.y - 1; location.y > 0; location.y -= step)
                        callBack(*frameCopy, location, data.start, data.end, polyline, delta);
        }
    }
}

void _2DsMax::Frame::endTransform()
{
    delete frameCopy;
    frameCopy = nullptr;
}*/