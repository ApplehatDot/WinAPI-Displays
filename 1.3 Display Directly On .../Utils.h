/* WinAPI - 1.3 Display Directly On ...
 *		Github: https://github.com/ApplehatDot
 * 		Enum and Window Creation functions - header file
 */
 
#ifndef UTILS_H
#define UTILS_H

typedef struct {
    HWND hwnd;
    char selectedMonitor[32];
} MonitorEnumData;

BOOL CALLBACK EnumDisplayMonitorsProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

#endif // UTILS_H
