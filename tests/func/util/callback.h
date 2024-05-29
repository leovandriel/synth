#include "../../../src/func/util/callback.h"
#include "../../../src/func/gen/const.h"
#include "../../util/test.h"

static double test_callback_add(int count, Gen **args, double delta, void *context)
{
    assert_long_equal(count, 2);
    assert_double_equal(delta, 0.1);
    assert_long_equal(*(int *)context, 2);
    return gen_eval(args[0]) + gen_eval(args[1]);
}

void test_callback()
{
    int context = 2;
    Func *t = callback_func(test_callback_add, &context, const_(3), const_(4));
    Gen *g = gen_create(t, 0.1);
    assert_gen_equal(g, 7);
}