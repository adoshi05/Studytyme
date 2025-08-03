#include <stdio.h>
#include "timer.h"
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void wait_for_enter(const char *prompt) {
    printf("%s", prompt);
    fflush(stdout);
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

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
    getchar(); // remove newline

    if (choice == 2) {
        // get study and break times
        int study_minutes, break_minutes;
        printf("Enter study duration in minutes: ");
        scanf("%d", &study_minutes);
        printf("Enter break duration in minutes: ");
        scanf("%d", &break_minutes);
        getchar(); // remove newline
        run_automatic_mode(&timer, study_minutes, break_minutes);
    } else {
        // manual mode (stopwatch mode)
        printf("\nRunning in self-paced mode.\n");
        while (1) {
            // studying
            wait_for_enter("\nPress [Enter] to START studying...");
            start_session(&timer, MODE_STUDY);
            wait_for_enter("Press [Enter] to STOP studying...");
            int study_elapsed = end_session_with_return(&timer);
            save_timer_state(&timer, "timer_state.txt");

            // calculate earned break
            int earned_break = study_elapsed / 5;
            printf("\nEarned break time: ");
            print_time(earned_break);
            printf("\n");

            // initiate break
            wait_for_enter("Press [Enter] to START break...");
            start_session(&timer, MODE_BREAK);
            printf("Taking break for earned time...\n");
            #ifdef _WIN32
            Sleep(earned_break * 1000);
            #else
            sleep(earned_break);
            #endif

            // more break prompt
            char more;
            printf("Break time over. Do you want more break time? (y/n): ");
            scanf(" %c", &more);
            wait_for_enter("");

            int break_elapsed = earned_break;

            if (more == 'y' || more == 'Y') {
                wait_for_enter("Press [Enter] to END extra break...");
                int extra_start = (int)time(NULL);

                //wait_for_enter("");
                int extra_end = (int)time(NULL);
                int extra = extra_end - extra_start;

                break_elapsed += extra;
                printf("Extra break duration: ");
                print_time(extra);
                printf("\n");
            }

            // end and save session
            timer.end_time = timer.start_time + break_elapsed;
            end_session_with_return(&timer);
            save_timer_state(&timer, "timer_state.txt");

            get_status(&timer);

            char again;
            printf("\nRepeat another cycle? (y/n): ");
            scanf(" %c", &again);
            getchar(); // remove newline
            if (again != 'y' && again != 'Y') break;
        }
    }

    return 0;
}