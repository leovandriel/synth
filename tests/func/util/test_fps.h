#include "../../../src/func/gen/const.h"
#include "../../../src/func/util/fps.h"
#include "../../util/test.h"

static int handled = 0;

void handle(StateEvent *event, __U void *context)
{
    assert_long_equal(event->key_type, StateEventKeyTypeLabel);
    assert_long_equal(event->value_type, StateEventValueTypeDouble);
    assert_double_equal(*(double *)event->value, 1.0);
    handled++;
}

void test_fps()
{
    StateEventContext context = {
        .handler = NULL,
        .handle_event = handle,
    };
    state_event_add(&context);
    Func *input = fps_create("label", const_create(0));
    Gen *gen = gen_create(input);
    EvalContext eval = eval_create(0.1);
    gen_eval(gen, &eval);
    gen_eval(gen, &eval);
    assert_long_equal(handled, 1);
    gen_free(gen);
    func_free();
    event_clear();
}
