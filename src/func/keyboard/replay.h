//
// replay.h - Replay key presses
//
#ifndef CSYNTH_REPLAY_H
#define CSYNTH_REPLAY_H

#include "../../core/func.h"
#include "../../core/gen.h"
#include "../../event/keyboard_event.h"
#include "../../mem/key_list.h"
#include "../../util/config.h"

typedef struct
{
    KeyList list;
    TimedKeyboardEvent *current;
    const char *filename;
    size_t index;
    double time;
} ReplayContext;

static double replay_eval(__U int count, Gen **args, double delta, void *context_)
{
    ReplayContext *context = (ReplayContext *)context_;
    while (context->current && context->current->time <= context->time)
    {
        keyboard_event_broadcast(context->current->time, context->current->key);
        context->current = context->current->next;
    }
    context->time += delta;
    return gen_eval(args[0]);
}

static int replay_init(__U int count, __U Gen **args, __U double delta, void *context_)
{
    ReplayContext *context = (ReplayContext *)context_;
    csError error = key_list_read_filename(&context->list, context->filename);
    if (error != csErrorNone)
    {
        return error_catch(error);
    }
    context->current = context->list;
    return 0;
}

static void replay_free(__U int count, void *context_)
{
    ReplayContext *context = (ReplayContext *)context_;
    key_list_clear(&context->list);
}

Func *replay(Func *func, const char *filename)
{
    ReplayContext initial = (ReplayContext){
        .filename = filename,
    };
    return func_create(replay_init, replay_eval, replay_free, sizeof(ReplayContext), &initial, FUNC_FLAG_DEFAULT, 1, func);
}

Func *replay_(Func *func)
{
    return replay(func, CONFIG_DEFAULT_REC_FILENAME);
}

#endif // CSYNTH_REPLAY_H
