//
// writer.h - Samples a function and writes the result to a WAV file.
//
#ifndef CSYNTH_WRITER_H
#define CSYNTH_WRITER_H

#include <string.h>
#include <stdio.h>

#include "./sampler.h"
#include "./wav_header.h"

static const int WRITER_BUFFER_SIZE = 4096;

typedef int (*writer_callback)(size_t size, void *buffer, void *context);

int writer_write_callback(int channel_count, Func **roots, double duration, writer_callback callback, void *context)
{
    uint32_t sample_count = duration * SAMPLER_RATE;
    uint32_t data_size = sizeof(sample_t) * channel_count * sample_count;
    WavHeader header = {0};
    memcpy(header.riff_type, "RIFF", 4);
    header.file_size = WAV_HEADER_SIZE + data_size;
    memcpy(header.file_type, "WAVE", 4);
    memcpy(header.format_mark, "fmt ", 4);
    header.format_size = WAV_HEADER_FORMAT_SIZE;
    header.format_type = 1; // PCM
    header.num_channels = channel_count;
    header.sample_rate = SAMPLER_RATE;
    header.byte_rate = sizeof(sample_t) * channel_count * SAMPLER_RATE;
    header.block_align = sizeof(sample_t) * channel_count;
    header.bits_sample = sizeof(sample_t) * 8;
    memcpy(header.data_chunk, "data", 4);
    header.data_size = data_size;
    int result = callback(sizeof(header), &header, context);
    Sampler *sampler = sampler_create(channel_count, roots);
    sample_t buffer[WRITER_BUFFER_SIZE];
    uint32_t buffer_samples = WRITER_BUFFER_SIZE / channel_count;
    while (sample_count && !result)
    {
        unsigned long samples = buffer_samples < sample_count ? buffer_samples : sample_count;
        sampler_sample(sampler, samples, buffer);
        result = callback(sizeof(sample_t) * channel_count * samples, buffer, context);
        if (result)
        {
            return result;
        }
        sample_count -= samples;
    }
    sampler_free(sampler);
    return result;
}

int writer_file_callback(size_t size, void *buffer, void *context)
{
    fwrite(buffer, 1, size, (FILE *)context);
    return 0;
}

int writer_write_channels(int channel_count, Func **roots, double duration, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    int result = writer_write_callback(channel_count, roots, duration, writer_file_callback, file);
    fclose(file);
    return result;
}

int write_file(Func *root, double duration, const char *filename)
{
    return writer_write_channels(1, (Func *[]){root}, duration, filename);
}

int write_file_stereo(Func *left, Func *right, double duration, const char *filename)
{
    return writer_write_channels(2, (Func *[]){left, right}, duration, filename);
}

#endif // CSYNTH_WRITER_H
