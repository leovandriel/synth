//
// looper.h - Loop samples
//
#ifndef CSYNTH_LOOPER_H
#define CSYNTH_LOOPER_H

#include <stdlib.h>
#include <string.h>

#include "../../core/func.h"
#include "../../core/gen.h"
#include "../../event/keyboard_event.h"
#include "../../event/state_event.h"
#include "../../mem/buffer.h"

typedef struct
{
    KeyboardEventContext parent;
    int key;
    Buffer buffer;
    size_t index;
    int recording;
    int reset;
} LooperContext;

static double looper_eval(__U int count, Gen **args, Eval eval, void *context_)
{
    LooperContext *context = (LooperContext *)context_;
    double tick = gen_eval(args[0], eval);
    size_t size = (size_t)(1.0 / tick);
    context->index = buffer_resize(&context->buffer, size, context->index, NULL);
    double *buffer = context->buffer.samples;
    double output = 0;
    if (buffer != NULL)
    {
        if (context->recording)
        {
            if (context->reset != 0)
            {
                gen_reset(args[1]);
                buffer_fill(&context->buffer, NULL);
                context->reset = 0;
            }
            output = gen_eval(args[1], eval);
            buffer[context->index] = output;
        }
        else
        {
            output = buffer[context->index];
        }
        context->index = (context->index + 1) % size;
    }
    return output;
}

static void looper_handle_event(int key, void *context_)
{
    LooperContext *context = (LooperContext *)context_;
    if (key == context->key)
    {
        context->recording = !context->recording;
        if (context->recording)
        {
            context->reset = 1;
        }
        state_event_broadcast(StateEventKeyTypeKeyboard, &context->key, StateEventValueTypeBoolInv, &context->recording);
    }
}

static int looper_init(__U int count, __U Gen **args, void *context_)
{
    LooperContext *context = (LooperContext *)context_;
    state_event_broadcast(StateEventKeyTypeKeyboard, &context->key, StateEventValueTypeBoolInv, &context->recording);
    csError error = keyboard_event_add(&context->parent);
    return error_catch(error);
}

static void looper_free(__U int count, void *context_)
{
    LooperContext *context = (LooperContext *)context_;
    buffer_free(&context->buffer);
}

Func *looper_create(int key, Func *tick, Func *input)
{
    LooperContext initial = {
        .parent = {.handle_event = looper_handle_event},
        .key = key,
    };
    return func_create(looper_init, looper_eval, looper_free, sizeof(LooperContext), &initial, FuncFlagNone, FUNCS(tick, input));
}

#endif // CSYNTH_LOOPER_H
