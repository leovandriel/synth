//
// reverb.h - Reverb function
//
// `reverb(input, interval, decay)` returns a reverberated version of input.
//
#ifndef CSYNTH_REVERB_H
#define CSYNTH_REVERB_H

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../core/func.h"
#include "./const.h"

typedef struct
{
    double *buffer;
    unsigned long size;
    unsigned long index;
    unsigned long max;
} ReverbContext;

double reverb_eval(Gen **args, __attribute__((unused)) int count, double delta, void *_context)
{
    ReverbContext *context = (ReverbContext *)_context;
    double input = gen_eval(args[0]);
    unsigned long size = round(gen_eval(args[1]) / delta);
    double decay = gen_eval(args[2]);
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

#define reverb_(_input, _interval, _decay) (reverb(_input, const_(_interval), const_(_decay)))

void test_reverb()
{
    func t = reverb(const_(1), const_(.2), const_(.5));
    Gen *g = gen_create(t, 0.1);
    double epsilon = 1e-4;
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 1.000000) < epsilon);
    assert(fabs(gen_eval(g) - 1.000000) < epsilon);
    assert(fabs(gen_eval(g) - 1.500000) < epsilon);
    assert(fabs(gen_eval(g) - 1.500000) < epsilon);
    assert(fabs(gen_eval(g) - 1.750000) < epsilon);
    assert(fabs(gen_eval(g) - 1.750000) < epsilon);
    assert(fabs(gen_eval(g) - 1.875000) < epsilon);
    assert(fabs(gen_eval(g) - 1.875000) < epsilon);
    assert(fabs(gen_eval(g) - 1.937500) < epsilon);
    assert(fabs(gen_eval(g) - 1.937500) < epsilon);
}

#endif // CSYNTH_REVERB_H
