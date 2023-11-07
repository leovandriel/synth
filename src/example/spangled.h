#include "../core/funcs.h"

double speed = 0.4;

func A(func frequency, int duration)
{
    double d = speed * duration;
    func f = saw(frequency);
    f = chorus_(f, sine_(.2), 0.02, 0.003);
    f = low_pass(f, A2);
    f = mul(f, adsr_(d / 4, d / 8, 0.8, d / 8, d));
    return f;
}

func B(func frequency, int duration)
{
    double d = speed * duration;
    func f = add(
        mul(sine(mul_(frequency, 2)), _(0.2)),
        mul(sine(div_(frequency, 2)), _(0.2)));
    f = mul(f, block_(0, d));
    return add(f, A(frequency, duration));
}

func C(func frequency, int duration)
{
    double d = speed * duration;
    func f = karplus_strong(frequency, _(0.5));
    f = low_pass(f, frequency);
    f = mul(f, block_(0, d));
    return add(f, B(frequency, duration));
}

func spangled()
{
    func f1 = comp_seq(
        A(G4, 1),
        A(E4, 1),
        A(C4, 2),
        A(E4, 2),
        A(G4, 2),
        A(C5, 4),
        A(E5, 1),
        A(D5, 1),
        A(C5, 2),
        A(E4, 2),
        A(Fs4, 2),
        A(G4, 4),
        A(G4, 1),
        A(G4, 1),
        A(E5, 3),
        A(D5, 1),
        A(C5, 2),
        A(B4, 4),
        A(A4, 1),
        A(B4, 1),
        A(C5, 2),
        A(C5, 2),
        A(G4, 2),
        A(E4, 2),
        A(C4, 2));

    func f2 = comp_seq(
        B(G4, 1),
        B(E4, 1),
        B(C4, 2),
        B(E4, 2),
        B(G4, 2),
        B(C5, 4),
        B(E5, 1),
        B(D5, 1),
        B(C5, 2),
        B(E4, 2),
        B(Fs4, 2),
        B(G4, 4),
        B(G4, 1),
        B(G4, 1),
        B(E5, 3),
        B(D5, 1),
        B(C5, 2),
        B(B4, 4),
        B(A4, 1),
        B(B4, 1),
        B(C5, 2),
        B(C5, 2),
        B(G4, 2),
        B(E4, 2),
        B(C4, 2));

    func f3 = comp_seq(
        C(E5, 1),
        C(E5, 1),
        C(E5, 2),
        C(F5, 2),
        C(G5, 2),
        C(G5, 4),
        C(F5, 1),
        C(E5, 1),
        C(D5, 2),
        C(E5, 2),
        C(F5, 2),
        C(F5, 4),
        C(F5, 2),
        C(E5, 3),
        C(D5, 1),
        C(C5, 2),
        C(B4, 4),
        C(A4, 1),
        C(B4, 1),
        C(C5, 2),
        C(E4, 2),
        C(Fs4, 2),
        C(G4, 4),
        C(G4, 2),
        C(C5, 2),
        C(C5, 2),
        C(C5, 1),
        C(B4, 1),
        C(A4, 2),
        C(A4, 2),
        C(A4, 2),
        C(D5, 2),
        C(F5, 1),
        C(E5, 1),
        C(D5, 1),
        C(C5, 1),
        C(C5, 2),
        C(B4, 2),
        C(G4, 1),
        C(G4, 1),
        C(C5, 3),
        C(D5, 1),
        C(E5, 1),
        C(F5, 1),
        C(G5, 4),
        C(C5, 1),
        C(D5, 1),
        C(E5, 3),
        C(F5, 1),
        C(D5, 2),
        C(C5, 4));
    func f = comp_rel(f1, _(48.5 * speed), f2, _(48.5 * speed), f3, _(98 * speed));
    f = reverb_(f, 0.1, 0.5);
    return f;
}