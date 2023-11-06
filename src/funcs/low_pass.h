//
// low_pass.h - A first-order low-pass filter
//
// `low_pass(input, frequency)` with input and frequency functions.
//
#ifndef COMPOSER_LOW_PASS_H
#define COMPOSER_LOW_PASS_H

#include <assert.h>
#include <math.h>

#include "../core/func.h"
#include "./cons.h"

typedef struct
{
    double output;
} LowPassContext;

double low_pass_eval(Gen **args, __attribute__((unused)) int count, double delta, void *_context)
{
    LowPassContext *context = (LowPassContext *)_context;
    double input = gen_eval(args[0]);
    double frequency = gen_eval(args[1]);
    double factor = 1 / (delta * frequency * 2.0 * M_PI) + 1.0;
    double output = context->output;
    context->output = context->output + (input - context->output) / factor;
    return output;
}

Func *low_pass(Func *input, Func *frequency)
{
    return func_create(NULL, low_pass_eval, NULL, sizeof(LowPassContext), NULL, 2, input, frequency);
}

void test_low_pass()
{
    func t = low_pass(cons(1), cons(10));
    Gen *g = gen_create(t, 0.1);
    double epsilon = 1e-4;
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.862697) < epsilon);
    assert(fabs(gen_eval(g) - 0.981148) < epsilon);
    assert(fabs(gen_eval(g) - 0.997412) < epsilon);
    assert(fabs(gen_eval(g) - 0.999645) < epsilon);
    assert(fabs(gen_eval(g) - 0.999951) < epsilon);
    assert(fabs(gen_eval(g) - 0.999993) < epsilon);
    assert(fabs(gen_eval(g) - 0.999999) < epsilon);
    assert(fabs(gen_eval(g) - 1.000000) < epsilon);
    assert(fabs(gen_eval(g) - 1.000000) < epsilon);
    assert(fabs(gen_eval(g) - 1.000000) < epsilon);
    assert(fabs(gen_eval(g) - 1.000000) < epsilon);
}

#endif // COMPOSER_LOW_PASS_H