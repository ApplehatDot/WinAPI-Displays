/* WinAPI - 1.2 Receive Window Placement
 *		Github: https://github.com/ApplehatDot
 */

#include <windows.h>
#include <stdio.h>

#define TIMER_ID 1
#define TIMER_INTERVAL 2000 // 2 seconds


void CheckMonitorForWindow(HWND hWnd) {
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	
    MONITORINFOEX monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFOEX);
	
    if (GetMonitorInfo(hMonitor, (LPMONITORINFO)&monitorInfo)) {
        printf("The Window is on Display %s\n", monitorInfo.szDevice);
    } else {
        printf("Could not receive information about Display Device.\n");
    }
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);
        break;
    case WM_TIMER:
        if (wParam == TIMER_ID) {
            CheckMonitorForWindow(hWnd);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, TIMER_ID);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int main() {
    const char CLASS_NAME[] = "MovingWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "WinAPI Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );

    if (hWnd) {
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);

        MSG msg = {};
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    } else {
        printf("Nie udało się utworzyć okna.\n");
    }

    return 0;
}
