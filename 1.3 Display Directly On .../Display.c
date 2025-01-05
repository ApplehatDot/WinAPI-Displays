/* WinAPI - 1.3 Display Directly On ...
 *		Github: https://github.com/ApplehatDot
 */

#include <windows.h>
#include <commctrl.h>
#include <string.h>
#include "utils.h"

/* From Utils.h */
// MonitorEnumData struct
BOOL CALLBACK EnumDisplayMonitorsProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
/* end */

HFONT hFont;
HWND hComboBox;
HWND hButton;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {
        case WM_CREATE: {
            // Getting system Font
            hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

            // ComboBox Creation
            hComboBox = CreateWindowA(
                WC_COMBOBOXA, "",
                CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                50, 50, 200, 150,
                hwnd, (HMENU)1, NULL, NULL
            );
            SendMessage(hComboBox, WM_SETFONT, (WPARAM)hFont, TRUE);

            // Enumerating screens and adding them to Combobox as options
            EnumDisplayMonitors(NULL, NULL, EnumDisplayMonitorsProc, (LPARAM)hComboBox);

            // adding a button
            hButton = CreateWindowA(
                WC_BUTTONA, "Display",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                50, 120, 100, 30,
                hwnd, (HMENU)2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
            );
            SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == 2) {
                int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
                if (index != CB_ERR) {
                    char buffer[256];
                    SendMessage(hComboBox, CB_GETLBTEXT, (WPARAM)index, (LPARAM)buffer);

                    // Searching the monitors, to find the chosen one
                    MonitorEnumData data = { hwnd, {0} };
                    strcpy(data.selectedMonitor, buffer);
                    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&data);
                }
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

    // registering windows class
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MainWindow";
    RegisterClassA(&wc);

    // creating the main window
    HWND hwnd = CreateWindowA(
        wc.lpszClassName, "Display On ...",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Msg loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
