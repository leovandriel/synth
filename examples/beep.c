//usr/bin/gcc "$0" -o bin/beep -Wall -Wextra -O3 -lm -lportaudio && ./bin/beep "$@"; exit $?
#include "../src/func/all.h"
#include "../src/util/player.h"

int main()
{
    func tone = sine(A4);
    func note = mul(tone, block_(0, .3), _(.5));
    func looped = loop_(note, 1.5);
    func revved = reverb_(looped, .4, .2);
    return play(revved);
}