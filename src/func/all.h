//
// all.h - A single include file for all the functions.
//
#ifndef CSYNTH_FUNC_ALL_H
#define CSYNTH_FUNC_ALL_H

#include "./bank/drums.h"
#include "./comp/comps.h"
#include "./comp/seq.h"
#include "./effect/continuous.h"
#include "./effect/delay.h"
#include "./effect/effects.h"
#include "./effect/loop.h"
#include "./effect/pitch.h"
#include "./effect/reverb.h"
#include "./effect/tempo.h"
#include "./filter/chorus.h"
#include "./filter/compressor.h"
#include "./filter/distortion.h"
#include "./filter/envelopes.h"
#include "./filter/filters.h"
#include "./filter/hpf.h"
#include "./filter/limit.h"
#include "./filter/lpf.h"
#include "./filter/resonant.h"
#include "./filter/slope.h"
#include "./filter/truncate.h"
#include "./gen/const.h"
#include "./gen/crack.h"
#include "./gen/gauss.h"
#include "./gen/gens.h"
#include "./gen/karplus_strong.h"
#include "./gen/notes.h"
#include "./gen/saw.h"
#include "./gen/sine.h"
#include "./gen/square.h"
#include "./gen/timer.h"
#include "./gen/triangle.h"
#include "./gen/uniform.h"
#include "./gen/unison.h"
#include "./gen/wav.h"
#include "./keyboard/actuate.h"
#include "./keyboard/keyboards.h"
#include "./keyboard/mute.h"
#include "./keyboard/pause.h"
#include "./keyboard/replay.h"
#include "./keyboard/selector.h"
#include "./keyboard/stepper.h"
#include "./keyboard/track.h"
#include "./keyboard/trigger.h"
#include "./midi/key.h"
#include "./midi/knob.h"
#include "./midi/midis.h"
#include "./midi/pad.h"
#include "./op/add.h"
#include "./op/adsr.h"
#include "./op/inv.h"
#include "./op/max.h"
#include "./op/min.h"
#include "./op/mul.h"
#include "./op/neg.h"
#include "./op/ops.h"
#include "./op/pow.h"
#include "./op/sin.h"
#include "./op/smooth.h"
#include "./op/step.h"
#include "./op/tanh.h"
#include "./util/gauge.h"
#include "./util/logger.h"
#include "./util/print.h"
#include "./util/record.h"
#include "./util/wrap.h"

#endif // CSYNTH_FUNC_ALL_H
