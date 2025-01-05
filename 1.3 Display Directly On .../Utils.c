/* WinAPI - 1.3 Display Directly On ...
 *		Github: https://github.com/ApplehatDot
 * 		Enum and Window Creation functions - C file
 */

#include <windows.h>
#include <commctrl.h>
#include <string.h>

typedef struct {
    HWND hwnd;
    char selectedMonitor[32];
} MonitorEnumData;

// Processing function for each monitor
BOOL CALLBACK EnumDisplayMonitorsProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	
	// creating an MONITORINFOEX instance, that holds the informations
    MONITORINFOEX mi;
    mi.cbSize = sizeof(mi);
    if (GetMonitorInfo(hMonitor, (LPMONITORINFO)&mi)) {
        SendMessage((HWND)dwData, CB_ADDSTRING, 0, (LPARAM)mi.szDevice); 	// add Monitor name to Combobox
    }
    return TRUE;	// continue enum'ing
}

// Window Process for the 'travelling' window
LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
				// You can add something there, if you really want
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

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	
    MonitorEnumData *pData = (MonitorEnumData *)dwData;
    MONITORINFOEX mi;
    mi.cbSize = sizeof(mi);
	
    if (GetMonitorInfo(hMonitor, (LPMONITORINFO)&mi)) {
		
        if (strcmp(mi.szDevice, pData->selectedMonitor) == 0) {
            // registering windows class
            WNDCLASS wc = {0};
            wc.lpfnWndProc = NewWndProc;
            wc.hInstance = (HINSTANCE)GetWindowLongPtr(pData->hwnd, GWLP_HINSTANCE);
            wc.lpszClassName = "NewWindow";
            RegisterClass(&wc);

            // creating a window which will be on a chosen display
            HWND hNewWnd = CreateWindow(
                wc.lpszClassName, "New Window on another display",
                WS_OVERLAPPEDWINDOW,
                mi.rcMonitor.left + 100, mi.rcMonitor.top + 100, 300, 200,
                NULL, NULL, wc.hInstance, NULL
            );

            ShowWindow(hNewWnd, SW_SHOW);
            UpdateWindow(hNewWnd);
            return FALSE; // stop enum
        }
    }
	return TRUE; // Continue enum'ing
}
