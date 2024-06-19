#include "../../../src/func/filter/distortion.h"
#include "../../../src/func/gen/saw.h"
#include "../../util/test.h"

void test_distortion_pos()
{
    Func *time = distortion(saw_(1), const_(1));
    Gen *gen = gen_create(time, 0.1);
    assert_gen_equal(gen, 0.0000000000000000);
    assert_gen_equal(gen, 0.4046096751916897);
    assert_gen_equal(gen, 0.6444049826448045);
    assert_gen_equal(gen, 0.8030496866860282);
    assert_gen_equal(gen, 0.9157761915991025);
    assert_gen_equal(gen, 1.0000000000000000);
    assert_gen_equal(gen, -0.9157761915991025);
    assert_gen_equal(gen, -0.8030496866860282);
    assert_gen_equal(gen, -0.6444049826448046);
    assert_gen_equal(gen, -0.4046096751916898);
    assert_gen_equal(gen, -0.0000000000000002);
    assert_gen_equal(gen, 0.4046096751916896);
    gen_free(gen);
    func_free();
}

void test_distortion_neg()
{
    Func *time = distortion(saw_(1), const_(-1));
    Gen *gen = gen_create(time, 0.1);
    assert_gen_equal(gen, 0.0000000000000000);
    assert_gen_equal(gen, 0.0842238084008974);
    assert_gen_equal(gen, 0.1969503133139719);
    assert_gen_equal(gen, 0.3555950173551956);
    assert_gen_equal(gen, 0.5953903248083104);
    assert_gen_equal(gen, 1.0000000000000000);
    assert_gen_equal(gen, -0.5953903248083104);
    assert_gen_equal(gen, -0.3555950173551956);
    assert_gen_equal(gen, -0.1969503133139720);
    assert_gen_equal(gen, -0.0842238084008974);
    assert_gen_equal(gen, -0.0000000000000000);
    assert_gen_equal(gen, 0.0842238084008974);
    gen_free(gen);
    func_free();
}

void test_distortion()
{
    test_distortion_pos();
    test_distortion_neg();
}