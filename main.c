#include "timer.h"
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int main() {
    TimerState timer = { MODE_IDLE, 0, 0, 0, 0, 0 };

    start_session(&timer, MODE_STUDY);
    #ifdef _WIN32
    Sleep(10000); // milliseconds
    #else
    sleep(10); // seconds
    #endif
    end_session(&timer);

    start_session(&timer, MODE_BREAK);
    #ifdef _WIN32
    Sleep(4000); // milliseconds
    #else
    sleep(4); // seconds
    #endif
    end_session(&timer);

    get_status(&timer);
    return 0;
}