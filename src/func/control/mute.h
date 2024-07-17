//
// mute.h - Mute/unmute based on key press
//
#ifndef CSYNTH_MUTE_H
#define CSYNTH_MUTE_H

#include "../../core/func.h"
#include "../../core/gen.h"
#include "../../event/control_event.h"
#include "../../event/state_event.h"

typedef struct
{
    ControlEventContext parent;
    ControlEventKey key;
    bool muted;
} MuteContext;

static double mute_eval(__U size_t count, __U Gen **args, Eval *eval, void *context_)
{
    MuteContext *context = (MuteContext *)context_;
    double input = gen_eval(args[0], eval);
    return context->muted ? 0 : input;
}

static void mute_handle_event(ControlEvent *event, void *context_)
{
    MuteContext *context = (MuteContext *)context_;
    if (control_event_key_equal(event->key, context->key))
    {
        context->muted = !context->muted;
        state_event_broadcast(event->time, StateEventKeyTypeControl, &context->key, StateEventValueTypeBoolInv, &context->muted);
    }
}

static bool mute_init(__U size_t count, __U Gen **args, void *context_)
{
    MuteContext *context = (MuteContext *)context_;
    state_event_broadcast(0, StateEventKeyTypeControl, &context->key, StateEventValueTypeBoolInv, &context->muted);
    csError error = control_event_add(&context->parent);
    return error_catch(error);
}

Func *mute_create(int key, bool muted, Func *input)
{
    MuteContext initial = {
        .parent = {.handle_event = mute_handle_event},
        .key = {
            .type = ControlEventTypeKeyboard,
            .keyboard = key,
        },
        .muted = muted,
    };
    return func_create(mute_init, mute_eval, control_event_free, sizeof(MuteContext), &initial, FuncFlagNone, input);
}

#endif // CSYNTH_MUTE_H
