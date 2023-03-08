#ifndef COMPOSER_REVERB_H
#define COMPOSER_REVERB_H

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../core/func.h"

typedef struct
{
    double *buffer;
    unsigned long size;
    unsigned long index;
    unsigned long max;
} ReverbContext;

double reverb_eval(unsigned long index, double rate, Func **args, __attribute__((unused)) int count, void *_context)
{
    ReverbContext *context = (ReverbContext *)_context;
    double input = func_eval(args[0], index, rate);
    unsigned long size = round(func_eval(args[1], index, rate) * rate);
    double decay = func_eval(args[2], index, rate);
    if (size != context->size)
    {
        if (context->buffer == NULL)
        {
            context->buffer = (double *)calloc(size, sizeof(double));
            context->max = size;
        }
        else if (size > context->max)
        {
            unsigned long max = size * 2;
            context->buffer = (double *)realloc(context->buffer, max * sizeof(double));
            memset(context->buffer + context->max, 0, (max - context->max) * sizeof(double));
            // TODO: shift the contents of the buffer
            context->max = max;
        }
        context->size = size;
    }
    double output = context->buffer[context->index];
    context->buffer[context->index] = context->buffer[context->index] * decay + input;
    context->index = (context->index + 1) % context->size;
    return output;
}

void reverb_free(void *_context)
{
    ReverbContext *context = (ReverbContext *)_context;
    if (context->buffer != NULL)
    {
        free(context->buffer);
    }
}

Func *reverb(Func *input, Func *interval, Func *decay)
{
    return func_create(NULL, reverb_eval, reverb_free, sizeof(ReverbContext), NULL, 3, input, interval, decay);
}

#endif // COMPOSER_REVERB_H
