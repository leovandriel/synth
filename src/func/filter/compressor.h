//
// compressor.h - Compression effect
//
// `compressor(input, threshold, ratio, attack, release)`
// - `input` - Input signal
// - `threshold` - Level at which the compressor starts to reduce gain [0..1]
// - `ratio` - Compression ratio [0..1]
// - `attack` - Attack in seconds
// - `release` - Release in seconds
//
#ifndef CSYNTH_COMPRESSOR_H
#define CSYNTH_COMPRESSOR_H

#include <math.h>

#include "../../core/func.h"
#include "../../core/gen.h"
#include "../gen/const.h"

typedef struct
{
    double gain;
} CompressorContext;

static double compressor_eval(__attribute__((unused)) int count, Gen **args, double delta, void *context_)
{
    CompressorContext *context = (CompressorContext *)context_;
    double input = gen_eval(args[0]);
    double threshold = gen_eval(args[1]);
    double ratio = gen_eval(args[2]);
    double attack = gen_eval(args[3]);
    double release = gen_eval(args[4]);
    double level = fabs(input);
    double target = level > threshold ? pow(threshold / level, ratio) : 1.0;
    double coeff = -expm1(-delta / (target < context->gain ? attack : release));
    context->gain += (target - context->gain) * coeff;
    return input * context->gain;
}

Func *compressor(Func *input, Func *threshold, Func *ratio, Func *attack, Func *release)
{
    CompressorContext initial = (CompressorContext){
        .gain = 1.0,
    };
    return func_create(NULL, compressor_eval, NULL, sizeof(CompressorContext), &initial, FUNC_FLAG_DEFAULT, 5, input, threshold, ratio, attack, release);
}

Func *compressor_(Func *input, double threshold, double ratio, double attack, double release)
{
    return compressor(input, const_(threshold), const_(ratio), const_(attack), const_(release));
}

#endif // CSYNTH_COMPRESSOR_H
