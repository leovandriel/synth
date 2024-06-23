//
// tanh.h - tanh function
//
#ifndef CSYNTH_TANH_H
#define CSYNTH_TANH_H

#include <math.h>

#include "../../core/func.h"
#include "../../core/gen.h"
#include "../gen/const.h"

static double tanh_eval(__U int count, Gen **args, __U double delta, __U void *context)
{
    double input = gen_eval(args[0]);
    return tanh(input);
}

Func *tanh_op(Func *input)
{
    return func_create(NULL, tanh_eval, NULL, 0, NULL, FUNC_FLAG_DEFAULT, 1, input);
}

#endif // CSYNTH_TANH_H
