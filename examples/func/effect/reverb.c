//usr/bin/gcc "$0" -o bin/reverb -Wall -Wextra -O3 -lm -lportaudio && ./bin/reverb "$@"; exit $?
#include "../../../src/func/all.h"
#include "../../../src/io/player.h"

int main()
{
    // TODO: Implement
    func f = hush(sine(C4));
    return play(mul_(f, .5));
}
