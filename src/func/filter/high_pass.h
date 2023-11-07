//
// high_pass.h - A first-order high-pass filter
//
// `high_pass(input, frequency)` with input and frequency functions.
//
#ifndef CSYNTH_HIGH_PASS_H
#define CSYNTH_HIGH_PASS_H

#include <assert.h>
#include <math.h>

#include "../../core/func.h"
#include "../gen/const.h"

typedef struct
{
    double input;
    double output;
} HighPassContext;

static double high_pass_eval(Gen **args, __attribute__((unused)) int count, double delta, void *_context)
{
    HighPassContext *context = (HighPassContext *)_context;
    double input = gen_eval(args[0]);
    double frequency = gen_eval(args[1]);
    double factor = (2.0 * M_PI * frequency * delta) + 1.0;
    double output = context->output;
    context->output = (context->output + input - context->input) / factor;
    context->input = input;
    return output;
}

Func *high_pass(Func *input, Func *frequency)
{
    return func_create(NULL, high_pass_eval, NULL, sizeof(HighPassContext), NULL, 2, input, frequency);
}

Func *high_pass_(Func *input, double frequency) { return high_pass(input, const_(frequency)); }

void test_high_pass()
{
    Func *t = high_pass(const_(1), const_(10));
    Gen *g = gen_create(t, 0.1);
    double epsilon = 1e-4;
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.137303) < epsilon);
    assert(fabs(gen_eval(g) - 0.018852) < epsilon);
    assert(fabs(gen_eval(g) - 0.002588) < epsilon);
    assert(fabs(gen_eval(g) - 0.000355) < epsilon);
    assert(fabs(gen_eval(g) - 0.000049) < epsilon);
    assert(fabs(gen_eval(g) - 0.000007) < epsilon);
    assert(fabs(gen_eval(g) - 0.000001) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
    assert(fabs(gen_eval(g) - 0.000000) < epsilon);
}

#endif // CSYNTH_HIGH_PASS_H
