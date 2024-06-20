//
// pad.h - Trigger based on pad
//
#ifndef CSYNTH_PAD_H
#define CSYNTH_PAD_H

#include <math.h>

#include "../../core/func.h"
#include "../../core/gen.h"
#include "../../event/midi_event.h"
#include "../../event/state_event.h"

typedef struct
{
    MidiEventContext parent;
    MidiKey key;
    double value;
    int reset;
} PadContext;

static double pad_eval(__attribute__((unused)) int count, __attribute__((unused)) Gen **args, __attribute__((unused)) double delta, void *context_)
{
    PadContext *context = (PadContext *)context_;
    if (context->reset != 0)
    {
        gen_reset(args[0]);
        context->reset = 0;
    }
    return gen_eval(args[0]) * context->value;
}

static void pad_listener(__attribute__((unused)) double time, MidiType type, uint8_t channel, uint8_t data1, uint8_t data2, void *context_)
{
    PadContext *context = (PadContext *)context_;
    if (type == MidiTypeNoteOn && channel == context->key.channel && data1 == context->key.control)
    {
        context->value = (double)data2 / 127.0;
        context->reset = 1;
    }
}

static int pad_init(__attribute__((unused)) int count, __attribute__((unused)) Gen **args, __attribute__((unused)) double delta, void *context_)
{
    PadContext *context = (PadContext *)context_;
    csError error = midi_event_add(&context->parent);
    return error_catch(error);
}

Func *pad(int channel, int pad, Func *func)
{
    PadContext initial = (PadContext){
        .parent = {.midi_listener = pad_listener},
        .key = (MidiKey){
            .channel = channel - 1,
            .control = pad,
        },
    };
    return func_create(pad_init, pad_eval, midi_event_free, sizeof(PadContext), &initial, FUNC_FLAG_DEFAULT, 1, func);
}

#endif // CSYNTH_PAD_H
