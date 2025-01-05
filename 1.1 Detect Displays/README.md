_Tested and Compiled with MinGW 6.2.0 on Windows 11 23H2:_
```
gcc -o DetectDisplays.exe Detect.c
```

# 1.1 Detecting Connected Displays
_Using non-unicode definitions of functions - for eazier understanding and learning_<br><br>

**Brief Explanation:** the result is a list of available (connected) monitors.

[DISPLAY_DEVICE](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-display_devicea) structure lets us receive information 
for example about _what displays are connected, what are their names and see their status_. First you begin with declarating such structure in 
your code, and after that you must declare its size in bytes, since its function `EnumDisplayDevices` will need that

```C
#include <windows.h>
#include <stdio.h>

/*     Here are defined message tables,
  no need to do exactly as in the source code. */

int deviceIndex = 0;
int monitorCount = 0;

DISPLAY_DEVICE dd;

int main() {

    dd.cb = sizeof(dd);     // cb - Count (of) Bytes

    // rest of the code later.

}
```

In order to make a loop that will receive information about displays, and then show in the terminal, we need to make a `while()` loop that 
uses [`EnumDisplayDevices`](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-enumdisplaydevicesa) function and while it's 
succeeding and is in index (Therefore the use of `int deviceIndex`), and the Display device is connected currently (has `DISPLAY_DEVICE_ATTACHED_TO_DESKTOP` 
flag), will show it's name and add to `monitorCount` - which will reveal in the end how many displays are there, lets recreate it:

```c
/* rest of the code */

int main() {

    dd.cb = sizeof(dd);     // cb - Count (of) Bytes
	
    // a loop thats active as long its deviceIndex is not too high
    while (EnumDisplayDevices(NULL, deviceIndex, &dd, 0)) {

        // Show displays name, if its attached to the PC
        if (dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) {

            //           ⬐ "Display %d: %s\n" 
            printf(C_DISPLAYCOUNT, monitorCount + 1, dd.DeviceName);
            monitorCount++;
        }
        deviceIndex++;  // used Index value for the function
    }
    
    printf(C_AVAIL_DISPL, monitorCount);    // <- "Number of available displays: %d\n"

    // this codes limit is on 2 displays,
    // feel free to play with it and change it
    // - depending how much displays you have

    if (monitorCount >= 2) {  
        printf(C_MONITOR_AVAIL);        // <- "Second display is available"
    } else {
        printf(C_MONITOR_NOT_AVAIL);    // <- "Second display is not available"
    }

    return 0;
}
```

After Compiling and running, you should see _this_ if you have two displays:
```
Display 1: \\.\DISPLAY1
Display 2: \\.\DISPLAY2
Number of available displays: 2
Second display is available
```
