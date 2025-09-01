#include "../minitest.h"

#include "foo_test.c"
#include "bar_test.c"

int
main(void)
{
    MT_INIT();

    MT_RUN_TEST(foo_always_pass);
    MT_RUN_TEST(foo_always_fail);
    MT_RUN_TEST(foo_addition_check);
    MT_RUN_TEST(foo_addition_assert);

    MT_RUN_TEST(bar_always_pass);
    MT_RUN_TEST(bar_always_fail);
    MT_RUN_TEST(bar_addition_check);
    MT_RUN_TEST(bar_addition_assert);

    MT_PRINT_SUMMARY();
    return MT_EXIT_CODE;
}
