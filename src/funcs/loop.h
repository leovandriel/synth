//
// loop.h - A time loop on the sampling of a function
//
// `loop(input, duration)` loops the input function every duration (a function).
//
#ifndef CSYNTH_LOOP_H
#define CSYNTH_LOOP_H

#include <assert.h>
#include <math.h>

#include "../core/func.h"
#include "./const.h"
#include "./step.h"

typedef struct
{
    double time;
} LoopContext;

double loop_eval(Gen **args, __attribute__((unused)) int count, double delta, void *_context)
{
    LoopContext *context = (LoopContext *)_context;
    double duration = gen_eval(args[1]);
    if (context->time >= duration)
    {
        context->time -= duration;
        gen_reset(args[0]);
    }
    double output = gen_eval(args[0]);
    context->time += delta;
    return output;
}

Func *loop(Func *input, Func *duration)
{
    return func_create(NULL, loop_eval, NULL, sizeof(LoopContext), NULL, 2, input, duration);
}

#define loop_(_input, _duration) (loop(_input, const_(_duration)))

void test_loop()
{
    func t = loop(step(const_(0.3)), const_(0.5));
    Gen *g = gen_create(t, 0.1);
    double epsilon = 1e-9;
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 1.000000) < epsilon);
    assert(fabs(gen_eval(g) - 1.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 1.000000) < epsilon);
    assert(fabs(gen_eval(g) - 1.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
}

#endif // CSYNTH_LOOP_H
