//usr/bin/gcc "$0" -o bin/slope -Wall -Wextra -O3 -lm -lportaudio && ./bin/slope "$@"; exit $?
#include "../../../src/func/all.h"
#include "../../../src/io/player.h"

int main()
{
    // square wave filtered with decreasing limit on slope
    func tone = mul_(.3, square(A4));
    func sloped = slope(expo(linear_env_(10, 10, 0)), tone);
    return play(sloped);
}
