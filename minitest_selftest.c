#define MT_PRINT_OK(...)
#define MT_PRINT_ERR(...)
#include "minitest.h"

#include <stdio.h>
#include <stdlib.h>

MT_DEFINE_TEST(int_add_check_ok)
{
    MT_CHECK_THAT(1 + 0 == 1);
    MT_CHECK_THAT(35 + 34 == 69);
}

MT_DEFINE_TEST(int_add_check_fail)
{
    MT_CHECK_THAT(1 + 1 == 3);
    MT_CHECK_THAT(35 + 34 != 69);
}

MT_DEFINE_TEST(int_add_assert_ok)
{
    MT_ASSERT_THAT(1 + 0 == 1);
    MT_ASSERT_THAT(35 + 34 == 69);
}

MT_DEFINE_TEST(int_add_assert_fail)
{
    MT_ASSERT_THAT(1 + 1 == 3);
    MT_ASSERT_THAT(35 + 34 != 69);
}

static int all_tests_meet_expectations = 1;

#define RUN_TEST_WRAPPER(name, expect_success)                          \
    MT_RUN_TEST(name);                                                  \
    if (mt_test_success_ != expect_success) {                           \
        fprintf(stderr, "Test \"%s\" expected to %s, but expectation was not met.\n", #name, expect_success ? "succeed" : "fail"); \
        all_tests_meet_expectations = 0;                                \
    }

int
main(void)
{
    MT_INIT();

    RUN_TEST_WRAPPER(int_add_check_ok, 1);
    RUN_TEST_WRAPPER(int_add_check_fail, 0);
    RUN_TEST_WRAPPER(int_add_assert_ok, 1);
    RUN_TEST_WRAPPER(int_add_assert_fail, 0);

    return all_tests_meet_expectations ? 0 : 1;
}
