/* WinAPI - 1.4 run program On ...
 *		Github: https://github.com/ApplehatDot
 * 		Enum and Window Creation functions - header file
 */
 
#ifndef UTILS_H
#define UTILS_H

#include <windows.h>

typedef struct {
    HWND hwnd;
    char selectedMonitor[32];
    char exePath[260];
} MonitorEnumData;

BOOL CALLBACK EnumDisplayMonitorsProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

void OpenAndRunExeOnSelectedMonitor(HWND hwnd);

#endif // UTILS_H
