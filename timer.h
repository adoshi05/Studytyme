#ifndef TIMER_H
#define TIMER_H

#include <time.h>

typedef enum { MODE_IDLE, MODE_STUDY, MODE_BREAK, MODE_AUTO } Mode;

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
//void run_automatic_mode(TimerState *state); //automatic hour study, 20 minute break
void run_automatic_mode(TimerState *state, int study_minutes, int break_minutes); //Set times as desired by user for study and break
int save_timer_state(const TimerState *state, const char *filename); //file saving
int load_timer_state(TimerState *state, const char *filename); //retrieve from file save


#endif