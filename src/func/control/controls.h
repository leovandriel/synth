//
// controls.h - Misc keyboard and MIDI controls
//
#ifndef CSYNTH_CONTROLS_H
#define CSYNTH_CONTROLS_H

#include <math.h>

#include "../../core/func.h"
#include "../../util/error.h"
#include "../filter/filters.h"
#include "../gen/gens.h"
#include "../op/ops.h"
#include "../time/times.h"
#include "./actuate.h"
#include "./blend.h"
#include "./key.h"
#include "./knob.h"
#include "./mute.h"
#include "./pad.h"
#include "./pause.h"
#include "./replay.h"
#include "./selector.h"
#include "./stepper.h"
#include "./track.h"
#include "./trigger.h"

#define MIDI_NOTE_COUNT 0x80

#define DEFAULT_REC_FILENAME "output/default.rec"

Func *trigger(int key, Func *input) { return trigger_create(key, input); }

Func *mute(int key, Func *input) { return mute_create(key, 0, input); }
Func *unmute(int key, Func *input) { return mute_create(key, 1, input); }

Func *pause_play(int key, Func *input) { return pause_create(key, 0, 0, input); }
Func *play_pause(int key, Func *input) { return pause_create(key, 0, 1, input); }
Func *pause_reset(int key, Func *input) { return pause_create(key, 1, 0, input); }
Func *reset_pause(int key, Func *input) { return pause_create(key, 1, 1, input); }

#define selector(key, ...) (selector_create(key, FUNCS(__VA_ARGS__)))

Func *stepper(int key, double value, double delta) { return stepper_create(key, value, delta, -FLT_MAX, FLT_MAX, 0); }
Func *stepper_rel(int key, double value, double perc) { return stepper_create(key, value, perc, -FLT_MAX, FLT_MAX, 1); }

Func *track(const char *filename, Func *input) { return track_create(filename, input); }
Func *track_(Func *input) { return track(DEFAULT_REC_FILENAME, input); }
Func *replay(const char *filename, Func *input) { return replay_create(filename, tempo_ticker_(1), input); }
Func *replay_(Func *input) { return replay(DEFAULT_REC_FILENAME, input); }

Func *actuate(int key) { return actuate_create(key); }

Func *knob_smooth(int channel, int control, Func *derivative) { return slope(derivative, knob_create(channel, control)); }
Func *knob(int channel, int control, Func *min, Func *max) { return linear_op(const_(1), min, max, knob_smooth(channel, control, const_(1))); }
Func *knob_(int channel, int control, double min, double max) { return knob(channel, control, const_(min), const_(max)); }
Func *knob_ex(int channel, int control, Func *min, Func *max) { return exponent_op(const_(1), min, max, knob_smooth(channel, control, const_(1))); }
Func *knob_ex_(int channel, int control, double min, double max) { return knob_ex(channel, control, const_(min), const_(max)); }

Func *key(int channel, int pitch, Func *input) { return key_create(channel, pitch, input); }
Func *pad(int channel, int pad, Func *input) { return pad_create(channel, pad, input); }
Func *blend(int channel, Func *factor, Func *input) { return blend_create(channel, factor, input); }
Func *blend_(int channel, double factor, Func *input) { return blend(channel, const_(factor), input); }

typedef Func *(*midi_control_func)(int channel, int key, Func *input);

Func *midi_keyboard(int channel, midi_control_func control, Func *input)
{
    Func **array = (Func **)malloc_(MIDI_NOTE_COUNT * sizeof(Func *));
    if (array == NULL)
    {
        return error_null(csErrorMemoryAlloc);
    }
    for (int i = 0; i < MIDI_NOTE_COUNT; i++)
    {
        Func *pitched = pitch_(exp2(i / 12.0), input);
        array[i] = control(channel, i, pitched);
    }
    Func *output = add_create(MIDI_NOTE_COUNT, array);
    free_(array);
    return output;
}

#endif // CSYNTH_CONTROLS_H