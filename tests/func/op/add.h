#include "../../../src/func/op/add.h"
#include "../../util/test.h"

void test_add()
{
    Gen *gen1 = gen_create(add(const_(1), const_(1)), .1);
    Gen *gen2 = gen_create(add(const_(1), const_(2), const_(3)), .1);
    Gen *gen3 = gen_create(add(const_(1)), .1);
    assert_gen_equal(gen1, 2.0);
    assert_gen_equal(gen2, 6.0);
    assert_gen_equal(gen3, 1.0);
    gen_free(gen1);
    gen_free(gen2);
    gen_free(gen3);
    func_free();
}
