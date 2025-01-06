/* WinAPI - 1.4 run program On ...
 *		Github: https://github.com/ApplehatDot
 */

#include <windows.h>
#include <commctrl.h>
#include <string.h>
#include "utils.h"

HFONT hFont;
HWND hComboBox;
HWND hButton;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

            hComboBox = CreateWindowA(
                WC_COMBOBOXA, "",
                CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                50, 50, 200, 150,
                hwnd, (HMENU)1, NULL, NULL
            );
            SendMessage(hComboBox, WM_SETFONT, (WPARAM)hFont, TRUE);

            EnumDisplayMonitors(NULL, NULL, EnumDisplayMonitorsProc, (LPARAM)hComboBox);

            hButton = CreateWindowA(
                WC_BUTTONA, "Open *.exe to show on a specified display",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                40, 120, 220, 30,
                hwnd, (HMENU)2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
            );
            SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == 2) {
                OpenAndRunExeOnSelectedMonitor(hwnd);
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default: {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MainWindow";
    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA(
        wc.lpszClassName, "Display program On ...",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 270,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
