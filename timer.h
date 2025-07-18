#ifndef TIMER_H
#define TIMER_H

#include <time.h>

typedef enum { MODE_IDLE, MODE_STUDY, MODE_BREAK } Mode;

typedef struct {
    Mode current_mode;
    time_t start_time;
    time_t end_time;

    int total_study_seconds;
    int total_break_seconds;
    int earned_break_seconds;
} TimerState;

void start_session(TimerState *state, Mode mode);
void end_session(TimerState *state);
void get_status(const TimerState *state);
int get_elapsed_time(const TimerState *state);

#endif