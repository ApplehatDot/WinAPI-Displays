/* WinAPI - 1.4 run program On ...
 *		Github: https://github.com/ApplehatDot
 * 		Enum and OpenDialog functions - C file
 */

#include "utils.h"
#include <commdlg.h>
#include <stdio.h>
#include <string.h>

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
    MonitorEnumData *pData = (MonitorEnumData *)dwData;
    MONITORINFOEX mi;
    mi.cbSize = sizeof(mi);

    if (GetMonitorInfo(hMonitor, (LPMONITORINFO)&mi)) {
        if (strcmp(mi.szDevice, pData->selectedMonitor) == 0) {
            // Uruchomienie wybranego programu na wybranym monitorze
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USEPOSITION;
            si.wShowWindow = SW_SHOWNORMAL;
            si.dwX = mi.rcMonitor.left + 100;
            si.dwY = mi.rcMonitor.top + 100;
            ZeroMemory(&pi, sizeof(pi));

            if (!CreateProcess(NULL, pData->exePath, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
                printf("CreateProcess failed (%d).\n", GetLastError());
                return FALSE; // Stop enum
            }

            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            return FALSE; // Stop enum
        }
    }
    return TRUE; // Continue enum'ing
}

BOOL CALLBACK EnumDisplayMonitorsProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
    MONITORINFOEX mi;
    mi.cbSize = sizeof(mi);

    if (GetMonitorInfo(hMonitor, (LPMONITORINFO)&mi)) {
        SendMessage((HWND)dwData, CB_ADDSTRING, 0, (LPARAM)mi.szDevice);
    }
    return TRUE; // Continue enum'ing
}

void OpenAndRunExeOnSelectedMonitor(HWND hwnd) {
    OPENFILENAME ofn;
    char szFile[260] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Executable Files\0*.EXE\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        HWND hComboBox = GetDlgItem(hwnd, 1);
        int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
        if (index != CB_ERR) {
            char buffer[256];
            SendMessage(hComboBox, CB_GETLBTEXT, (WPARAM)index, (LPARAM)buffer);

            MonitorEnumData data = { hwnd, "", "" };
            strcpy(data.selectedMonitor, buffer);
            strcpy(data.exePath, szFile);
            EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&data);
        }
    }
}
