_Tested on Windows 11 23H2, Compiled using MinGW 6.2.0:_
```
gcc -o Display.exe Display.c Utils.c -mwindows
```
# 1.3 Display Directly On ...
_non-unicode functions/declarations; GUI only example (`-mwindows`)_<br><br>

**Brief Explanation:** from the available displays, that are in ComboBox, you can decide on which display will be a window displayed on<br>

  ## .   .   . `Display.c`
In this file, the main Window is created, in this window there are controls (ComboBoxes, Buttons etc.) that let user decide,
on what display wants a window to be created/shown. Having a GUI window already created, we'll add controls to the window - ComboBoxes and Buttons.
Let's start with it. in the main window's `WndProc` at `WM_CREATE` we'll define a Combobox, aswell a button:
```c
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

            // Searches through the available monitors and adds them to the Combobox control
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
        /* the rest of the function*/

```

Let's add a command to a Button - that will display the window on a specified display. in `WndProc` at `WM_COMMAND` lets make an index 
that will search a specified/chosen display and then pass it to the `MonitorEnumData data`:
```
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
```


### .   .   . `Utils.c/*.h`
This file has Enumerating (searching) functions for displays, aswell has the sent window declaration. **Let's see the functions:**

```c
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
```
The Function shown above uses the same method as in 
**[1.2.B](https://github.com/ApplehatDot/WinAPI-Displays/tree/main/1.2%20Receive%20Window%20Placement#b-implementing-the-functionality)**
uses a Monitor's handle and gets the display's name into MONITORINFOEX struct; The `MonitorEnumData` structure as the name suggests, it holds the handle 
and the name of the display the user chose 


```c
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

    // if got the monitor, 
    if (GetMonitorInfo(hMonitor, (LPMONITORINFO)&mi)) {

        //       the travelling window is created here;
        // if those values are equal to eachother, define the new window
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
```
The comments pretty much explain themselfs, and some things are repeated there. 



after compiling these two files, we'll have a working win32 program, which works like in the Example Video:

https://github.com/user-attachments/assets/6da09546-8c6b-4f5e-9c6b-e3960b4a60f9


