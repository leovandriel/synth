#ifndef CSYNTH_GAUGE_H
#define CSYNTH_GAUGE_H

#include <stdio.h>

#include "../../core/func.h"
#include "../../core/gen.h"
#include "../../event/state_event.h"

/** @see gauge_create */
typedef struct
{
    /** @brief Label for the gauge. */
    const char *label;
    /** @brief Last output value. */
    double last;
    /** @brief Time since last broadcast. */
    double time;
} GaugeContext;

static double gauge_eval(__U size_t count, Gen **args, Eval *eval, void *context_)
{
    GaugeContext *context = (GaugeContext *)context_;
    double input = gen_eval(args[1], eval);
    if (context->time >= 1.0 && input != context->last && eval != NULL)
    {
        state_event_broadcast(eval->wall_time, StateEventKeyTypeLabel, context->label, StateEventValueTypeDouble, &input);
        context->last = input;
        context->time = 0.0;
    }
    double tick = gen_eval(args[0], eval);
    context->time += tick;
    return input;
}

/**
 * @brief Create a function that broadcasts the output value for display.
 *
 * @param label Label for the gauge.
 * @param tick Periods per sample.
 * @param input Input signal.
 * @return Func* Gauge function.
 */
Func *gauge_create(const char *label, Func *tick, Func *input)
{
    GaugeContext initial = {
        .label = label,
        .time = 1.0,
    };
    return func_create(NULL, gauge_eval, NULL, NULL, sizeof(GaugeContext), &initial, FuncFlagNone, tick, input);
}

#endif // CSYNTH_GAUGE_H
