#include "timer.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

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

//Automatic hour of studying followed by 20 minutes of break time with autoswitching between modes
void run_automatic_mode(TimerState *state, int study_minutes, int break_minutes) {
    state->current_mode = MODE_AUTO;

    printf("Entering automatic mode: %d min study / %d min break cycles.\n", study_minutes, break_minutes);

    int study_secs = study_minutes * 60;
    int break_secs = break_minutes * 60;

    while (1) {
        // Study phase
        printf("\n--- STUDYING ---\n");
        start_session(state, MODE_STUDY);
        #ifdef _WIN32
        Sleep(study_secs * 1000);
        #else
        sleep(study_secs);
        #endif
        end_session(state);
        save_timer_state(state, "timer_state.txt");

        // Break phase
        printf("\n--- BREAK TIME ---\n");
        start_session(state, MODE_BREAK);
        #ifdef _WIN32
        Sleep(break_secs * 1000);
        #else
        sleep(break_secs);
        #endif
        end_session(state);
        save_timer_state(state, "timer_state.txt");
    }

    /*printf("Entering automatic mode: 1h study / 20m break cycles.\n");

    while (1) {
        // Study phase
        printf("\n--- STUDYING ---\n");
        start_session(state, MODE_STUDY);
        #ifdef _WIN32
        Sleep(3600 * 1000);  //60 minute studying
        #else
        sleep(3600);         //60 minute studying
        #endif
        end_session(state);
        save_timer_state(state, "timer_state.txt");

        // Break phase
        printf("\n--- BREAK TIME ---\n");
        start_session(state, MODE_BREAK);
        #ifdef _WIN32
        Sleep(1200 * 1000);  //20 minute break
        #else
        sleep(1200); //20 minute break
        #endif
        end_session(state);
        save_timer_state(state, "timer_state.txt");
    }*/
}

//File save everything: current mode, start/end time, total time studied/break, and how much break was earned
int save_timer_state(const TimerState *state, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return -1;

    fprintf(file, "%d\n", state->current_mode);
    fprintf(file, "%lld\n", state->start_time);
    fprintf(file, "%lld\n", state->end_time);
    fprintf(file, "%d\n", state->total_study_seconds);
    fprintf(file, "%d\n", state->total_break_seconds);
    fprintf(file, "%d\n", state->earned_break_seconds);

    fclose(file);
    return 0;
}

int load_timer_state(TimerState *state, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    int mode;
    long start, end;
    int study, brk, earned;

    if (fscanf(file, "%d\n%ld\n%ld\n%d\n%d\n%d",
        &mode, &start, &end, &study, &brk, &earned) != 6) {
        fclose(file);
        return -2;
    }

    state->current_mode = (Mode)mode;
    state->start_time = (time_t)start;
    state->end_time = (time_t)end;
    state->total_study_seconds = study;
    state->total_break_seconds = brk;
    state->earned_break_seconds = earned;

    fclose(file);
    return 0;
}