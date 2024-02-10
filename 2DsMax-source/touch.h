#pragma once
#include <windows.h>
#include <assert.h>
#include "Stroke.h"
CStrokeCollection g_StrkColDrawing;
CStrokeCollection g_StrkColFinished;
COLORREF GetTouchColor(bool bPrimaryContact)
{
    static int g_iCurrColor = 0;    // Rotating secondary color index
    static COLORREF g_arrColor[] =  // Secondary colors array
    {
        RGB(255, 0, 0),             // Red
        RGB(0, 255, 0),             // Green
        RGB(0, 0, 255),             // Blue
        RGB(0, 255, 255),           // Cyan
        RGB(255, 0, 255),           // Magenta
        RGB(255, 255, 0)            // Yellow
    };

    COLORREF color;
    if (bPrimaryContact)
    {
        // The primary contact is drawn in black.
        color = RGB(0, 0, 0);         // Black
    }
    else
    {
        // Take current secondary color.
        color = g_arrColor[g_iCurrColor];

        // Move to the next color in the array.
        g_iCurrColor = (g_iCurrColor + 1) % (sizeof(g_arrColor) / sizeof(g_arrColor[0]));
    }

    return color;
}

// Extracts contact point in client area coordinates (pixels) from a TOUCHINPUT
// structure. TOUCHINPUT structure uses 100th of a pixel units.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)
// returns:
//      POINT with contact coordinates
POINT GetTouchPoint(HWND hWnd, const TOUCHINPUT& ti)
{
    POINT pt;
    pt.x = ti.x / 100;
    pt.y = ti.y / 100;
    ScreenToClient(hWnd, &pt);
    return pt;
}

// Extracts contact ID from a TOUCHINPUT structure.
// in:
//      ti          TOUCHINPUT structure (info about contact)
// returns:
//      ID assigned to the contact
inline int GetTouchContactID(const TOUCHINPUT& ti)
{
    return ti.dwID;
}

///////////////////////////////////////////////////////////////////////////////
// WM_TOUCH message handlers

// Handler for touch-down message.
// Starts a new stroke and assigns a color to it.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)
void OnTouchDownHandler(HWND hWnd, const TOUCHINPUT& ti)
{
    // Extract contact info: point of contact and ID
    POINT pt = GetTouchPoint(hWnd, ti);
    int iCursorId = GetTouchContactID(ti);

    // We have just started a new stroke, which must have an ID value unique
    // among all the strokes currently being drawn. Check if there is a stroke
    // with the same ID in the collection of the strokes in drawing.
    ASSERT(g_StrkColDrawing.FindStrokeById(iCursorId) == -1);

    // Create new stroke, add point and assign a color to it.
    CStroke* pStrkNew = new CStroke;
    pStrkNew->Add(pt);
    pStrkNew->SetColor(GetTouchColor((ti.dwFlags & TOUCHEVENTF_PRIMARY) != 0));
    pStrkNew->SetId(iCursorId);

    // Add new stroke to the collection of strokes in drawing.
    g_StrkColDrawing.Add(pStrkNew);
}

// Handler for touch-move message.
// Adds a point to the stroke in drawing and draws new stroke segment.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)
void OnTouchMoveHandler(HWND hWnd, const TOUCHINPUT& ti)
{
    // Extract contact info: contact ID
    int iCursorId = GetTouchContactID(ti);

    // Find the stroke in the collection of the strokes in drawing.
    int iStrk = g_StrkColDrawing.FindStrokeById(iCursorId);
    ASSERT((iStrk >= 0) && (iStrk < g_StrkColDrawing.Count()));

    // Extract contact info: contact point
    POINT pt;
    pt = GetTouchPoint(hWnd, ti);

    // Add contact point to the stroke
    g_StrkColDrawing[iStrk]->Add(pt);

    // Partial redraw: only the last line segment
    HDC hDC = GetDC(hWnd);
    g_StrkColDrawing[iStrk]->DrawLast(hDC);
    ReleaseDC(hWnd, hDC);
}

// Handler for touch-up message.
// Finishes the stroke and moves it to the collection of finished strokes.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)
void OnTouchUpHandler(HWND hWnd, const TOUCHINPUT& ti)
{
    // Extract contact info: contact ID
    int iCursorId = GetTouchContactID(ti);

    // Find the stroke in the collection of the strokes in drawing.
    int iStrk = g_StrkColDrawing.FindStrokeById(iCursorId);
    ASSERT((iStrk >= 0) && (iStrk < g_StrkColDrawing.Count()));

    // Add this stroke to the collection of finished strokes.
    g_StrkColFinished.Add(g_StrkColDrawing[iStrk]);

    // Remove this stroke from the collection of strokes in drawing.
    g_StrkColDrawing.Remove(iStrk);

    // Request full redraw.
    InvalidateRect(hWnd, NULL, FALSE);
}