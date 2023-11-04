#ifndef COMPOSER_LOWPASS_H
#define COMPOSER_LOWPASS_H

#include <math.h>

#include "../core/func.h"

typedef struct
{
    double output;
} LowpassContext;

double lowpass_eval(unsigned long index, double rate, Func **args, __attribute__((unused)) int count, void *_context)
{
    LowpassContext *context = (LowpassContext *)_context;
    double input = func_eval(args[0], index, rate);
    double frequency = func_eval(args[1], index, rate);
    double factor = rate / (frequency * 2.0 * M_PI) + 1.0;
    context->output = context->output + (input - context->output) / factor;
    return context->output;
}

Func *lowpass(Func *input, Func *frequency)
{
    return func_create(NULL, lowpass_eval, NULL, sizeof(LowpassContext), NULL, 2, input, frequency);
}

#endif // COMPOSER_LOWPASS_H