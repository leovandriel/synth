//
// sampler.h - Sampling multiple channels, like stereophonic sound.
//
#ifndef CSYNTH_SAMPLER_H
#define CSYNTH_SAMPLER_H

#include "../core/func.h"
#include "../core/gen.h"

static const int SAMPLER_RATE = 44100;
typedef int16_t sample_t;

typedef struct
{
    Gen **channels;
    int count;
} Sampler;

Sampler *sampler_create(int count, Func **roots)
{
    Sampler *sampler = (Sampler *)calloc(1, sizeof(Sampler));
    sampler->channels = (Gen **)calloc(count, sizeof(Gen *));
    for (int index = 0; index < count; index++)
    {
        sampler->channels[index] = gen_create(roots[index], 1.0 / SAMPLER_RATE);
    }
    sampler->count = count;
    return sampler;
}

void sampler_sample(Sampler *sampler, unsigned long count, sample_t *buffer)
{
    for (unsigned long frame = 0; frame < count; frame++)
    {
        for (int index = 0; index < sampler->count; index++)
        {
            double output = gen_eval(sampler->channels[index]);
            double clip = output > 1.0 ? 1.0 : (output < -1.0 ? -1.0 : output);
            *(buffer++) = clip * 32767;
        }
    }
}

void sampler_free(Sampler *sampler)
{
    for (int index = 0; index < sampler->count; index++)
    {
        gen_free(sampler->channels[index]);
    }
    free(sampler->channels);
    free(sampler);
}

#endif // CSYNTH_SAMPLER_H