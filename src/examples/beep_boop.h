#include "../core/funcs.h"

func beep_boop()
{
    func high = sine(A4);
    func low = mul(saw(A2), _(.5));
    func beep = mul(high, block_(0, .1));
    func boop1 = mul(low, block_(.4, .5));
    func boop2 = mul(low, block_(1, 1.1));
    func melody = add(beep, boop1, boop2);
    func looped = loop(melody, _(1.6));
    func echoed = reverb(looped, _(.2), _(.5));
    func scaled = mul(echoed, _(.1));
    return scaled;
}