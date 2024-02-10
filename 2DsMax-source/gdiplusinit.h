#pragma once
#include <gdiplus.h>
class GdiPlusInit
{
public:
    GdiPlusInit()
    {
        Gdiplus::GdiplusStartupInput startupInput;
        if (Gdiplus::GdiplusStartup(&m_token, &startupInput, NULL) != S_OK)
            throw "GDI+ INIT ERROR";
    }
    ~GdiPlusInit()
    {
        if (m_token)
            Gdiplus::GdiplusShutdown(m_token);
    }
    GdiPlusInit(const GdiPlusInit&) = delete;
    GdiPlusInit& operator=(const GdiPlusInit&) = delete;
private:
    ULONG_PTR m_token = 0;
};