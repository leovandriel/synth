//
// const.h - A constant value function.
//
// `const_(value)` takes one value, which it will return during sampling. Primary
// purpose is wrapping doubles in a Func, so it can be used as argument for
// other functions.
//
// For convenience, the macro `_(value)` is provided, which is equal to
// `const_(value)`.
// - ZERO - A constant function, which always returns 0
// - ONE - A constant function, which always returns 1
//
#ifndef CSYNTH_CONST_H
#define CSYNTH_CONST_H

#include "../../core/func.h"
#include "../../core/gen.h"

typedef struct
{
    double value;
} ConstContext;

static double const_eval(__attribute__((unused)) int count, __attribute__((unused)) Gen **args, __attribute__((unused)) double delta, void *context_)
{
    ConstContext *context = (ConstContext *)context_;
    return context->value;
}

Func *const_(double value)
{
    ConstContext initial = (ConstContext){
        .value = value,
    };
    return func_create(NULL, const_eval, NULL, sizeof(ConstContext), &initial, FUNC_FLAG_DEFAULT, 0);
}

Func *_(double value) { return const_(value); }

#endif // CSYNTH_CONST_H
