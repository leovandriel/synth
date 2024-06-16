//
// track.h - Track and save key presses
//
#ifndef CSYNTH_TRACK_H
#define CSYNTH_TRACK_H

#include "../../core/func.h"
#include "../../core/gen.h"
#include "../../event/keyboard_event.h"
#include "../../mem/key_list.h"
#include "../../util/config.h"

typedef struct
{
    KeyList list;
    const char *filename;
    void *handle;
} TrackContext;

static double track_eval(__attribute__((unused)) int count, Gen **args, __attribute__((unused)) double delta, __attribute__((unused)) void *context_)
{
    // TODO(leo): consider converting to non-func
    return gen_eval(args[0]);
}

static void track_listen(EventType type, void *event_, void *context_)
{
    TrackContext *context = (TrackContext *)context_;
    if (type == EventTypeKeyboard)
    {
        KeyboardEvent *event = (KeyboardEvent *)event_;
        TimedKeyboardEvent timed_event = {
            .key = event->key,
            .time = event->time,
        };
        csError error = key_list_add(&context->list, timed_event);
        error_catch(error);
    }
}

static int track_init(__attribute__((unused)) int count, __attribute__((unused)) Gen **args, __attribute__((unused)) double delta, void *context_)
{
    TrackContext *context = (TrackContext *)context_;
    void *handle = event_add_listener(track_listen, context);
    if (handle == NULL)
    {
        return error_catch_message(csErrorInit, "Unable to add track listener");
    }
    context->handle = handle;
    context->list = NULL;
    return 0;
}

static void track_free(__attribute__((unused)) int count, void *context_)
{
    TrackContext *context = (TrackContext *)context_;
    csError error = event_remove_listener(context->handle);
    error_catch(error);
    error = key_list_write_filename(&context->list, context->filename);
    error_catch(error);
    key_list_clear(&context->list);
}

Func *track(Func *func, const char *filename)
{
    TrackContext initial = (TrackContext){
        .filename = filename,
    };
    return func_create(track_init, track_eval, track_free, sizeof(TrackContext), &initial, FUNC_FLAG_DEFAULT, 1, func);
}

Func *track_(Func *func)
{
    return track(func, CONFIG_DEFAULT_REC_FILENAME);
}

#endif // CSYNTH_TRACK_H
