//usr/bin/gcc "$0" -o bin/wav -Wall -Wextra -O3 -lm -lportaudio && ./bin/wav "$@"; exit $?
#include "../../../src/func/all.h"
#include "../../../src/io/player.h"

func meow(func factor)
{
    return wav_filename("examples/demo/meow.wav", 0, factor);
}

int main()
{
    // play meow keyboard
    func f = keyboard_(trigger, meow, 1);
    printf("keyboard keys: %s\n", keyboard_keys);
    return play(mul_(f, .5));
}