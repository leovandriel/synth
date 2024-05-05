//
// timer.h - The sample time
//
// `timer()` returns accumulated time deltas
//
#ifndef CSYNTH_TIMER_H
#define CSYNTH_TIMER_H

#include "../../core/func.h"

typedef struct
{
    double time;
} TimerContext;

static double timer_eval(__attribute__((unused)) int count, __attribute__((unused)) Gen **args, double delta, void *context_)
{
    TimerContext *context = (TimerContext *)context_;
    double output = context->time;
    context->time += delta;
    return output;
}

Func *timer()
{
    return func_create(NULL, timer_eval, NULL, sizeof(TimerContext), NULL, 0);
}

#endif // CSYNTH_TIMER_H
