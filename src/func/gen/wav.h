#ifndef CSYNTH_WAV_H
#define CSYNTH_WAV_H

#include "../../core/func.h"
#include "../../core/gen.h"
#include "../../io/reader.h"

/** @see wav_create */
typedef struct
{
    /** @brief Sample data loaded in memory. */
    PcmBuffer buffer;
    /** @brief Channel to read samples from. */
    int channel;
    /** @brief Time offset to read next sample at. */
    double time;
} WavContext;

static double wav_eval(__U size_t count, Gen **args, Eval *eval, void *context_)
{
    WavContext *context = (WavContext *)context_;
    sample_t sample = reader_sample(&context->buffer, context->time, context->channel);
    double output = (double)sample / 32767;
    double tick = gen_eval(args[0], eval);
    context->time += tick;
    return output;
}

/**
 * @brief Create a function that reads samples from a WAV file.
 *
 * @param buffer PcmBuffer struct containing the samples.
 * @param channel Channel to read from.
 * @param tick Function that controls the reading speed.
 * @return Func* WAV function.
 */
Func *wav_create(PcmBuffer buffer, int channel, Func *tick)
{
    WavContext initial = {
        .buffer = buffer,
        .channel = channel,
    };
    return func_create(NULL, wav_eval, NULL, NULL, sizeof(WavContext), &initial, FuncFlagNone, tick);
}

/**
 * @brief Create a function that reads samples from a WAV file.
 *
 * @param filename Filename of the WAV file.
 * @param channel Channel to read from.
 * @param input Function that controls the reading speed.
 * @return Func* WAV function
 */
Func *wav_filename(const char *filename, int channel, Func *input)
{
    PcmBuffer buffer = {0};
    if (reader_read_filename(&buffer, filename) != csErrorNone)
    {
        return NULL;
    }
    return wav_create(buffer, channel, input);
}

#endif // CSYNTH_WAV_H
