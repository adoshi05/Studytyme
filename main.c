#include <stdio.h>
#include "timer.h"
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int main() {
    TimerState timer;

    if (load_timer_state(&timer, "timer_state.txt") == 0) {
        printf("Loaded previous session.\n");
    } else {
        printf("Starting new session.\n");
        timer = (TimerState){ MODE_IDLE, 0, 0, 0, 0, 0 };
    }

    printf("\nChoose mode:\n");
    printf("1. Self-paced mode (manual start / stop)\n");
    printf("2. Automatic mode (1h study / 20m break)\n");
    printf("Enter choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 2) {
        run_automatic_mode(&timer);
    } else {
        printf("Running in self-paced mode.\n");
        start_session(&timer, MODE_STUDY);
        #ifdef _WIN32
        Sleep(10000);  //10 seconds of studying test
        #else
        sleep(10);
        #endif
        end_session(&timer);

        save_timer_state(&timer, "timer_state.txt");
        get_status(&timer);
    }

    return 0;
}