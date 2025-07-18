#include "timer.h"
#include <stdio.h>
#include <time.h>

void print_time(int total_seconds) {
    int minutes = total_seconds / 60;
    int seconds = total_seconds % 60;
    printf("%d min %02d sec", minutes, seconds);
}

void start_session(TimerState *state, Mode mode) {
    state->current_mode = mode;
    state->start_time = time(NULL);
    printf("Started %s session.\n", mode == MODE_STUDY ? "study" : "break");
}

void end_session(TimerState *state) {
    state->end_time = time(NULL);
    int elapsed = (int)(state->end_time - state->start_time);

    if (state->current_mode == MODE_STUDY) {
        state->total_study_seconds += elapsed;
        state->earned_break_seconds += elapsed / 5; // Current study to break time conversion
        printf("Study session ended. +");
        print_time(state->earned_break_seconds);
        printf(" earned break.\n");
    } else if (state->current_mode == MODE_BREAK) {
        state->total_break_seconds += elapsed;
        state->earned_break_seconds -= elapsed;
        if (state->earned_break_seconds < 0) state->earned_break_seconds = 0;
        printf("Break session ended. Used ");
        print_time(elapsed);
        printf(" break.\n");
    }

    state->current_mode = MODE_IDLE;
}

int get_elapsed_time(const TimerState *state) {
    if (state->current_mode == MODE_IDLE) return 0;
    return (int)(time(NULL) - state->start_time);
}

void get_status(const TimerState *state) {
    printf("Status:\n");
    printf("  Mode: %s\n", 
           state->current_mode == MODE_IDLE ? "Idle" :
           state->current_mode == MODE_STUDY ? "Studying" : "On Break");
    printf("  Total Studied: ");
    print_time(state->total_study_seconds);
    printf("\n");

    printf("  Total Break: ");
    print_time(state->total_break_seconds);
    printf("\n");

    printf("  Break Time Remaining: ");
    print_time(state->earned_break_seconds);
    printf("\n");
}
