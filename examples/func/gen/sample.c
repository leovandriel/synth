//usr/bin/gcc "$0" -o bin/sample -Wall -Wextra -O3 -lm -lportaudio && ./bin/sample "$@"; exit $?
#include "../../../src/func/all.h"
#include "../../../src/io/player.h"

int main()
{
    // TODO(leo): implement
    func fun = keyboard(unmute, saw(C4));
    printf("keyboard keys: %s\n", keyboard_keys);
    return play(mul_(.1, fun));
}
