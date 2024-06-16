//
// trigger.h - Trigger based on key trigger
//
#ifndef CSYNTH_TRIGGER_H
#define CSYNTH_TRIGGER_H

#include "../../core/func.h"
#include "../../core/gen.h"
#include "../../event/keyboard_event.h"
#include "../../event/state_event.h"

typedef struct
{
    KeyboardEventContext parent;
    int key;
    int on;
    int reset;
} TriggerContext;

static double trigger_eval(__attribute__((unused)) int count, __attribute__((unused)) Gen **args, __attribute__((unused)) double delta, void *context_)
{
    TriggerContext *context = (TriggerContext *)context_;
    if (context->reset != 0)
    {
        gen_reset(args[0]);
        context->reset = 0;
    }
    return context->on ? gen_eval(args[0]) : 0;
}

static void trigger_listener(int key, void *context_)
{
    TriggerContext *context = (TriggerContext *)context_;
    if (key == context->key)
    {
        context->on = 1;
        context->reset = 1;
        state_event_broadcast(context->key, StateEventTypeTrigger, &context->on);
    }
}

static int trigger_init(__attribute__((unused)) int count, __attribute__((unused)) Gen **args, __attribute__((unused)) double delta, void *context_)
{
    TriggerContext *context = (TriggerContext *)context_;
    state_event_broadcast(context->key, StateEventTypeTrigger, &context->on);
    csError error = keyboard_event_add(&context->parent);
    return error_catch(error);
}

Func *trigger(int key, Func *func)
{
    TriggerContext initial = (TriggerContext){
        .parent = {.keyboard_listener = trigger_listener},
        .key = key,
    };
    return func_create(trigger_init, trigger_eval, keyboard_event_free, sizeof(TriggerContext), &initial, FUNC_FLAG_DEFAULT, 1, func);
}

#endif // CSYNTH_TRIGGER_H
