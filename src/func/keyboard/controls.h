//
// controls.h - Misc controls
//
#ifndef CSYNTH_CONTROLS_H
#define CSYNTH_CONTROLS_H

#include <math.h>

#include "../../core/func.h"
#include "../gen/gens.h"
#include "../gen/notes.h"
#include "../op/add.h"
#include "../op/mul.h"

typedef Func *(*control_func)(int key, Func *frequency);

const char *keyboard_keys = "zsxdcvgbhnjm,l.;/";

Func *keyboard(control_func control, gen_func generator, Func *frequency)
{
    int count = (int)strlen(keyboard_keys);
    Func **array = (Func **)calloc_(count, sizeof(Func *));
    for (int i = 0; i < count; i++)
    {
        char key = keyboard_keys[i];
        Func *gen = generator(mul_(frequency, pow(2, i / 12.0)));
        array[i] = control(key, gen);
    }
    Func *func = add_array(count, array);
    free_(array);
    return func;
}

Func *keyboard_(control_func control, gen_func generator, double frequency)
{
    return keyboard(control, generator, const_(frequency));
}

#endif // CSYNTH_CONTROLS_H