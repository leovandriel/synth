//
// seq.h - Sequential composition of functions.
//
// seq(...) takes multiple amplitude-duration pairs (both functions) and
// composes amplitudes sequentially, each with specified duration. This can be
// used for playing notes in sequence.
//
// Has two modes:
// - abs: Each duration is absolute, with each larger than the previous.
// - rel: Each duration is relative, specifying the duration of a single
//   function.
//
#ifndef CSYNTH_SEQ_H
#define CSYNTH_SEQ_H

#include "../../core/func.h"
#include "../../core/gen.h"

typedef struct
{
    double time;
    size_t index;
    size_t counter;
} SeqContext;

static double seq_eval_abs(size_t count, Gen **args, Eval *eval, void *context_)
{
    SeqContext *context = (SeqContext *)context_;
    double tick = gen_eval(args[0], eval);
    double output = 0.0;
    // TODO(leo): use context->index and track time per interval (allowing variable durations)
    for (size_t index = count / 2; index > 0; index--)
    {
        double offset = gen_eval(args[index * 2 - 1], eval);
        if (context->time > offset)
        {
            output = gen_eval(args[index * 2], eval);
            break;
        }
    }
    context->time += tick;
    return output;
}

Func *seq_abs_create(size_t count, Func **args)
{
    return func_create_args(NULL, seq_eval_abs, NULL, sizeof(SeqContext), NULL, FuncFlagNone, count, args, "tick, input");
}

static double seq_eval_rel(size_t count, Gen **args, Eval *eval, void *context_)
{
    SeqContext *context = (SeqContext *)context_;
    double tick = gen_eval(args[0], eval);
    double offset = 0.0;
    double output = 0.0;
    // TODO(leo): use context->index and track time per interval (allowing variable durations)
    for (size_t index = 0; index < count / 2; index++)
    {
        offset += gen_eval(args[index * 2 + 2], eval);
        if (context->time < offset)
        {
            output = gen_eval(args[index * 2 + 1], eval);
            break;
        }
    }
    context->time += tick;
    return output;
}

Func *seq_rel_create(size_t count, Func **args)
{
    return func_create_args(NULL, seq_eval_rel, NULL, sizeof(SeqContext), NULL, FuncFlagNone, count, args, "tick, input");
}

static double seq_eval_seq(size_t count, Gen **args, Eval *eval, void *context_)
{
    SeqContext *context = (SeqContext *)context_;
    double output = gen_eval(args[context->index], eval);
    if (output < FUNC_AUDIBLE && output > -FUNC_AUDIBLE)
    {
        context->counter++;
    }
    else
    {
        context->counter = 0;
    }
    if (context->counter > 100 && context->index < count - 1)
    {
        context->index++;
        context->counter = 0;
    }
    return output;
}

Func *seq_seq_create(size_t count, Func **args)
{
    return func_create_args(NULL, seq_eval_seq, NULL, sizeof(SeqContext), NULL, FuncFlagNone, count, args, "input");
}

static double seq_eval_fix(size_t count, Gen **args, Eval *eval, void *context_)
{
    SeqContext *context = (SeqContext *)context_;
    double tick = gen_eval(args[0], eval);
    size_t index = (size_t)context->time + 1;
    context->time += tick;
    return index < count ? gen_eval(args[index], eval) : 0;
}

Func *seq_fix_create(size_t count, Func **args)
{
    return func_create_args(NULL, seq_eval_fix, NULL, sizeof(SeqContext), NULL, FuncFlagNone, count, args, "tick, input");
}

#endif // CSYNTH_SEQ_H
