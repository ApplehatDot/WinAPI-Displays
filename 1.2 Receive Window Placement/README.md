_Tested on Windows 11 23H2, Compiled using MinGW 6.2.0:_
```
gcc -o WindowPlacement.exe WindowPlacement.c
```

# 1.2 Receive Window Placement
_non-unicode functions/declarations used._<br><br>

**Brief Explanation**: after 2 seconds, on the terminal you can see on what display is Win32 window on.<br>

This example uses [Win32 GUI window](https://learn.microsoft.com/en-us/windows/win32/learnwin32/your-first-windows-program)
, and by its position the terminal will show on which available display its shown on.<br> 
Let's start defining the GUI window. In the `main()` we need to create our window, in there we'll define its Window style, Title etc.
First we start registering our class:
#### A. Defining GUI Window
```c
#include <windows.h>
#include <stdio.h>

int main(){
    const char CLASS_NAME[] = "MovingWindow";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;    // there's the Window Process, we'll define it later
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    /* the rest of source code */
}
```
Then after registering our class, we'll create our window:
```c
int main(){
    /* previous code here*/

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,          // class name
        "WinAPI Window",    // title
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,    // starting window position
        800, 600,        // window size (x, y)
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );

    // if window successfully created, show and update it
    if (hWnd) {
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);

        // Message loop
        MSG msg = {};
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    } else {
        printf("Could not create the window.\n");
    }
}
```
and then we define WindowProc:


```c
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // here we'll create a timer
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
```
#### B. Implementing the functionality
Now after creating a GUI window, let's get to defining functionality. Let's first create a function that will search where's the window,
or exactly **on which screen is it on**. We'll start by defining our Monitor handle (`hMonitor`), from that we'll receive information about 
the monitor (display). 

```c
void CheckMonitorForWindow(HWND hWnd) {
    // our Monitor's handle
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	
    MONITORINFOEX monitorInfo;    // struct, that holds information
    monitorInfo.cbSize = sizeof(MONITORINFOEX);    // byte size

    // if Monitor found, show it's DeviceName
    if (GetMonitorInfo(hMonitor, (LPMONITORINFO)&monitorInfo)) {
        printf("The Window is on Display %s\n", monitorInfo.szDevice);
    } else {
        printf("Could not receive informations about Display\n");
    }
}
```

However if we'd try implementing it, it would show the result one time - and that's it. That's why we need a Timer, which will count to 2 
seconds and will once again call that function. In `WindowProc` at `case WM_CREATE` we'll create a Timer, that will have an ID of 1:
```c

//         You can define the values in the function,
// if adjustments needed, its better to have a value representing it

#define TIMER_ID 1
#define TIMER_INTERVAL 2000 // in miliseconds

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);
        break;
    case WM_TIMER:    // this defines what happens after the interval 
        if (wParam == TIMER_ID) {
            CheckMonitorForWindow(hWnd);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, TIMER_ID);    // Kill timer when window is closed
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
```

After Compiling and running it, you'll see that it detects on what display the window is on, like on the Example Video:

https://github.com/user-attachments/assets/1d1e203a-a15e-42a7-a256-55dbd26bc348



