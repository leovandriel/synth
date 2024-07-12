//
// biquad.h - A Biquad filter
//
#ifndef CSYNTH_BIQUAD_H
#define CSYNTH_BIQUAD_H

#include <math.h>

#include "../../core/func.h"
#include "../../core/gen.h"

typedef struct
{
    double cb0, cb1, cb2, ca1, ca2;
    double lx1, lx2, ly1, ly2;
    double time;
} BiquadContext;

static void biquad_scale(BiquadContext *context, double scale)
{
    context->cb0 *= scale;
    context->cb1 *= scale;
    context->cb2 *= scale;
    context->ca1 *= scale;
    context->ca2 *= scale;
}

static double biquad_process(BiquadContext *context, double input, double tick)
{
    double output = context->cb0 * input +
                    context->cb1 * context->lx1 +
                    context->cb2 * context->lx2 -
                    context->ca1 * context->ly1 -
                    context->ca2 * context->ly2;
    context->lx2 = context->lx1;
    context->lx1 = input;
    context->ly2 = context->ly1;
    context->ly1 = output;
    context->time += tick;
    return output;
}

static double lowpass_eval(__U size_t count, Gen **args, Eval *eval, void *context_)
{
    BiquadContext *context = (BiquadContext *)context_;
    double tick = gen_eval(args[0], eval);
    double qfactor = gen_eval(args[1], eval);
    double input = gen_eval(args[2], eval);
    if (context->time >= 1.0)
    {
        context->time -= 1.0;
        double omega = 2 * M_PI * tick;
        double cso = cos(omega);
        double alpha = sin(omega) / (2 * qfactor);
        context->cb1 = 1.0 - cso;
        context->cb0 = 0.5 * context->cb1;
        context->cb2 = context->cb0;
        context->ca1 = -2.0 * cso;
        context->ca2 = 1.0 - alpha;
        double ca0 = 1.0 + alpha;
        biquad_scale(context, 1.0 / ca0);
    }
    return biquad_process(context, input, eval->tick[EvalTickCompute]);
}

Func *lowpass_create(Func *tick, Func *qfactor, Func *input)
{
    BiquadContext initial = {.time = 1.0};
    return func_create(NULL, lowpass_eval, NULL, sizeof(BiquadContext), &initial, FuncFlagNone, FUNCS(tick, qfactor, input));
}

static double highpass_eval(__U size_t count, Gen **args, Eval *eval, void *context_)
{
    BiquadContext *context = (BiquadContext *)context_;
    double tick = gen_eval(args[0], eval);
    double qfactor = gen_eval(args[1], eval);
    double input = gen_eval(args[2], eval);
    if (context->time >= 1.0)
    {
        context->time -= 1.0;
        double omega = 2 * M_PI * tick;
        double cso = cos(omega);
        double alpha = sin(omega) / (2 * qfactor);
        context->cb1 = -1.0 - cso;
        context->cb0 = -0.5 * context->cb1;
        context->cb2 = context->cb0;
        context->ca1 = -2.0 * cso;
        context->ca2 = 1.0 - alpha;
        double ca0 = 1.0 + alpha;
        biquad_scale(context, 1.0 / ca0);
    }
    return biquad_process(context, input, eval->tick[EvalTickCompute]);
}

Func *highpass_create(Func *tick, Func *qfactor, Func *input)
{
    BiquadContext initial = {.time = 1.0};
    return func_create(NULL, highpass_eval, NULL, sizeof(BiquadContext), &initial, FuncFlagNone, FUNCS(tick, qfactor, input));
}

static double bandpass_eval(__U size_t count, Gen **args, Eval *eval, void *context_)
{
    BiquadContext *context = (BiquadContext *)context_;
    double tick = gen_eval(args[0], eval);
    double qfactor = gen_eval(args[1], eval);
    double input = gen_eval(args[2], eval);
    if (context->time >= 1.0)
    {
        context->time -= 1.0;
        double omega = 2 * M_PI * tick;
        double cso = cos(omega);
        double alpha = sin(omega) / (2 * qfactor);
        context->cb0 = alpha;
        context->cb1 = 0;
        context->cb2 = -alpha;
        context->ca1 = -2 * cso;
        context->ca2 = 1 - alpha;
        double ca0 = 1 + alpha;
        biquad_scale(context, 1.0 / ca0);
    }
    return biquad_process(context, input, eval->tick[EvalTickCompute]);
}

Func *bandpass_create(Func *tick, Func *qfactor, Func *input)
{
    BiquadContext initial = {.time = 1.0};
    return func_create(NULL, bandpass_eval, NULL, sizeof(BiquadContext), &initial, FuncFlagNone, FUNCS(tick, qfactor, input));
}

static double notch_eval(__U size_t count, Gen **args, Eval *eval, void *context_)
{
    BiquadContext *context = (BiquadContext *)context_;
    double tick = gen_eval(args[0], eval);
    double qfactor = gen_eval(args[1], eval);
    double input = gen_eval(args[2], eval);
    if (context->time >= 1.0)
    {
        context->time -= 1.0;
        double omega = 2 * M_PI * tick;
        double cso = cos(omega);
        double alpha = sin(omega) / (2 * qfactor);
        context->cb0 = 1;
        context->cb1 = -2 * cso;
        context->cb2 = 1;
        context->ca1 = -2 * cso;
        context->ca2 = 1 - alpha;
        double ca0 = 1 + alpha;
        biquad_scale(context, 1.0 / ca0);
    }
    return biquad_process(context, input, eval->tick[EvalTickCompute]);
}

Func *notch_create(Func *tick, Func *qfactor, Func *input)
{
    BiquadContext initial = {.time = 1.0};
    return func_create(NULL, notch_eval, NULL, sizeof(BiquadContext), &initial, FuncFlagNone, FUNCS(tick, qfactor, input));
}

static double peak_eval(__U size_t count, Gen **args, Eval *eval, void *context_)
{
    BiquadContext *context = (BiquadContext *)context_;
    double tick = gen_eval(args[0], eval);
    double qfactor = gen_eval(args[1], eval);
    double gain = gen_eval(args[2], eval);
    double input = gen_eval(args[3], eval);
    if (context->time >= 1.0)
    {
        context->time -= 1.0;
        double omega = 2 * M_PI * tick;
        double cso = cos(omega);
        double alpha = sin(omega) / (2 * qfactor);
        double amg = alpha * gain;
        double adg = alpha / gain;
        context->cb0 = 1 + amg;
        context->cb1 = -2 * cso;
        context->cb2 = 1 - amg;
        context->ca1 = -2 * cso;
        context->ca2 = 1 - adg;
        double ca0 = 1 + adg;
        biquad_scale(context, 1.0 / ca0);
    }
    return biquad_process(context, input, eval->tick[EvalTickCompute]);
}

Func *peak_create(Func *tick, Func *qfactor, Func *gain, Func *input)
{
    BiquadContext initial = {.time = 1.0};
    return func_create(NULL, peak_eval, NULL, sizeof(BiquadContext), &initial, FuncFlagNone, FUNCS(tick, qfactor, gain, input));
}

static double allpass_eval(__U size_t count, Gen **args, Eval *eval, void *context_)
{
    BiquadContext *context = (BiquadContext *)context_;
    double tick = gen_eval(args[0], eval);
    double qfactor = gen_eval(args[1], eval);
    double input = gen_eval(args[2], eval);
    if (context->time >= 1.0)
    {
        context->time -= 1.0;
        double omega = 2 * M_PI * tick;
        double cso = cos(omega);
        double alpha = sin(omega) / (2 * qfactor);
        context->cb0 = 1 - alpha;
        context->cb1 = -2 * cso;
        context->cb2 = 1 + alpha;
        context->ca1 = -2 * cso;
        context->ca2 = context->cb0;
        double ca0 = context->cb2;
        biquad_scale(context, 1.0 / ca0);
    }
    return biquad_process(context, input, eval->tick[EvalTickCompute]);
}

Func *allpass_create(Func *tick, Func *qfactor, Func *input)
{
    BiquadContext initial = {.time = 1.0};
    return func_create(NULL, allpass_eval, NULL, sizeof(BiquadContext), &initial, FuncFlagNone, FUNCS(tick, qfactor, input));
}

static double lowshelf_eval(__U size_t count, Gen **args, Eval *eval, void *context_)
{
    BiquadContext *context = (BiquadContext *)context_;
    double tick = gen_eval(args[0], eval);
    double gain = gen_eval(args[1], eval);
    double input = gen_eval(args[2], eval);
    if (context->time >= 1.0)
    {
        context->time -= 1.0;
        double omega = 2 * M_PI * tick;
        double cso = cos(omega);
        double gp1 = gain + 1;
        double gm1 = gain - 1;
        double bsno = sqrt(gain * 2) * sin(omega);
        context->cb0 = gain * (gp1 - gm1 * cso + bsno);
        context->cb1 = 2 * gain * (gm1 - gp1 * cso);
        context->cb2 = gain * (gp1 - gm1 * cso - bsno);
        context->ca1 = -2 * (gm1 + gp1 * cso);
        context->ca2 = gp1 + gm1 * cso - bsno;
        double ca0 = gp1 + gm1 * cso + bsno;
        biquad_scale(context, 1.0 / ca0);
    }
    return biquad_process(context, input, eval->tick[EvalTickCompute]);
}

Func *lowshelf_create(Func *tick, Func *gain, Func *input)
{
    BiquadContext initial = {.time = 1.0};
    return func_create(NULL, lowshelf_eval, NULL, sizeof(BiquadContext), &initial, FuncFlagNone, FUNCS(tick, gain, input));
}

static double highshelf_eval(__U size_t count, Gen **args, Eval *eval, void *context_)
{
    BiquadContext *context = (BiquadContext *)context_;
    double tick = gen_eval(args[0], eval);
    double gain = gen_eval(args[1], eval);
    double input = gen_eval(args[2], eval);
    if (context->time >= 1.0)
    {
        context->time -= 1.0;
        double omega = 2 * M_PI * tick;
        double cso = cos(omega);
        double gp1 = gain + 1;
        double gm1 = gain - 1;
        double bsno = sqrt(gain * 2) * sin(omega);
        context->cb0 = gain * (gp1 + gm1 * cso + bsno);
        context->cb1 = -2 * gain * (gm1 + gp1 * cso);
        context->cb2 = gain * (gp1 + gm1 * cso - bsno);
        context->ca1 = 2 * (gm1 - gp1 * cso);
        context->ca2 = gp1 - gm1 * cso - bsno;
        double ca0 = gp1 - gm1 * cso + bsno;
        biquad_scale(context, 1.0 / ca0);
    }
    return biquad_process(context, input, eval->tick[EvalTickCompute]);
}

Func *highshelf_create(Func *tick, Func *gain, Func *input)
{
    BiquadContext initial = {.time = 1.0};
    return func_create(NULL, highshelf_eval, NULL, sizeof(BiquadContext), &initial, FuncFlagNone, FUNCS(tick, gain, input));
}

#endif // CSYNTH_BIQUAD_H
