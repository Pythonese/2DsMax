#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <gdiplus.h>
#include "gdiplusinit.h"
#include "_2DsMax.h"
#include "structures.h"
#include "RTSCOM.h"
#include "combaseapi.h"
#include "touch.h"
#include "method_forward_declarations.h"
#include "menu_creater.h"
#pragma comment (lib, "Gdiplus.lib")

GdiPlusInit gdiPlusInit;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
Level1::MenuCreater menu;
Level1::KeyInfo key;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"2DsMax",    // Window text
        WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        0, 0, 800, 800,

        NULL,       // Parent window    
        menu.get(),       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
    menu.hwnd = hwnd;
    menu.hInstance = hInstance;
    //menu.animation->colorText = CreateWindow(L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL,
    //    10, 10, 200, 20, hwnd, NULL, hInstance, 0);
    //menu.animation->widthText = CreateWindow(L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL,
    //    10, 30, 200, 20, hwnd, NULL, hInstance, 0);
    //SetWindowTextA(menu.animation->colorText, "0000ffff");
    //SetWindowTextA(menu.animation->widthText, "9");

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //POINTER_PEN_INFO
    if (!RegisterTouchWindow(hwnd, 0))
    {
        MessageBox(hwnd, L"Cannot register application window for multitouch input", L"Error", MB_OK);
        return FALSE;
    }
    switch (uMsg)
    {
    case WM_COMMAND:
    {
        return menu.command(wParam);
    }
    case WM_SYSKEYDOWN:
        menu.animation->params.key.set(Level1::KeyInfo::ALT, true);
        return 0;
    case WM_SYSKEYUP:
        menu.animation->params.key.set(Level1::KeyInfo::ALT, false);
        return 0;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            menu.command(Level1::MenuCreater::PREVIOUS_FRAME);
            return 0;
        case VK_RIGHT:
            menu.command(Level1::MenuCreater::NEXT_FRAME);
            return 0;
        case VK_CONTROL:
            menu.animation->params.key.set(Level1::KeyInfo::CTRL, true);
            return 0;
        case VK_SHIFT:
            menu.animation->params.key.set(Level1::KeyInfo::SHIFT, true);
            return 0;
        case 0x4D: //M
            if (menu.animation->params.key.get(Level1::KeyInfo::CTRL))
                menu.command(Level1::MenuCreater::MID);
            return 0;
        case 0x45:
            if (menu.animation->params.key.get(Level1::KeyInfo::CTRL))
                menu.command(Level1::MenuCreater::ADD_FRAME);
            return 0;
        case 0x4F:
            if (menu.animation->params.key.get(Level1::KeyInfo::CTRL))
                menu.command(Level1::MenuCreater::OPEN);
            return 0;
        case 0x53:
            if (menu.animation->params.key.get(Level1::KeyInfo::CTRL))
                menu.command(Level1::MenuCreater::SAVE_ANIMATION_AS);
            return 0;
        case VK_DELETE:
            menu.command(Level1::MenuCreater::DEL);
            return 0;
        default:
            return 0;
        }
        return 0;
    case WM_KEYUP:
        switch (wParam)
        {
        case VK_CONTROL:
            menu.animation->params.key.set(Level1::KeyInfo::CTRL, false);
            return 0;
        case VK_SHIFT:
            menu.animation->params.key.set(Level1::KeyInfo::SHIFT, false);
            return 0;
        default:
            break;
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        wchar_t wc = L'9';
        int aa = (int)L'0';
        aa = 0;
        // All painting occurs here, between BeginPaint and EndPaint.
        //widthText = CreateWindowEx(0, L"Static", L"Лейбл\nLabel", WS_CHILD | WS_VISIBLE, 10, 10, 60, 60, hwnd, 0, 0, 0);
        menu.animation->paint(hdc);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        menu.animation->mouseMove(hwnd);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        menu.animation->mouseDown(Level1::LEFT, hwnd);
        return 0;
    }
    case WM_LBUTTONUP:
    {
        menu.animation->mouseUp(hwnd);
        return 0;
    }
    case WM_RBUTTONDOWN:
    {
        menu.animation->mouseDown(Level1::RIGHT, hwnd);
        return 0;
    }
    case WM_RBUTTONUP:
    {
        menu.animation->mouseUp(hwnd);
        return 0;
    }
    case WM_MBUTTONDOWN:
    {
        menu.animation->mouseDown(Level1::MIDDLE, hwnd);
        return 0;
    }
    case WM_MBUTTONUP:
    {
        menu.animation->mouseUp(hwnd);
        return 0;
    }

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}