#include "../../../src/func/filter/resonant.h"
#include "../../../src/func/gen/square.h"
#include "../../util/test.h"

void test_resonant()
{
    Func *t = resonant(square_(1), const_(2), const_(1));
    Gen *g = gen_create(t, 0.1);
    assert_gen_equal(g, 1.0000000000000000);
    assert_gen_equal(g, -0.3554467673117652);
    assert_gen_equal(g, 0.9140520505530968);
    assert_gen_equal(g, 0.2068404533398542);
    assert_gen_equal(g, 0.4179689975257589);
    assert_gen_equal(g, -1.5283959545087757);
    assert_gen_equal(g, 1.0572185352198431);
    assert_gen_equal(g, -1.3835083183028383);
    assert_gen_equal(g, -0.0165945916944109);
    assert_gen_equal(g, -0.4292847868595760);
}