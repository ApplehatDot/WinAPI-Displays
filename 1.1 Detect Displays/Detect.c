/* WinAPI - 1.1 Detecting Connected Displays
 *		Github: https://github.com/ApplehatDot
 */

#include <windows.h>
#include <stdio.h>

// messages table
const char *C_DISPLAYCOUNT = "Display %d: %s\n";
const char *C_AVAIL_DISPL = "Number of available displays: %d\n";
const char *C_MONITOR_AVAIL = "Second display is available";
const char *C_MONITOR_NOT_AVAIL = "Second display is not available";

int deviceIndex = 0;
int monitorCount = 0;

DISPLAY_DEVICE dd;

int main() {

    dd.cb = sizeof(dd);     // cb - Count (of) Bytes
	
    // [WINAPI] receiving and displaying information about the displays
    while (EnumDisplayDevices(NULL, deviceIndex, &dd, 0)) {
		
        // or in newer versions - DISPLAY_DEVICE_ACTIVE ↓
        if (dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) {    
            printf(C_DISPLAYCOUNT, monitorCount + 1, dd.DeviceName);
            monitorCount++;
        }
        deviceIndex++;
    }

    printf(C_AVAIL_DISPL, monitorCount);

    if (monitorCount >= 2) {
        printf(C_MONITOR_AVAIL);
    } else {
        printf(C_MONITOR_NOT_AVAIL);
    }

    return 0;
}