#include "minitest.h"

// A fully passing test
MT_DEFINE_TEST(always_pass)
{
    MT_CHECK_THAT(10 > 1);
    MT_CHECK_THAT(5 == 5);
}

// A fully failing test
MT_DEFINE_TEST(always_fail)
{
    MT_CHECK_THAT(0); // Always fails
}

// A test with checks: failures do not abort the test
MT_DEFINE_TEST(addition_check)
{
    MT_CHECK_THAT(1 + 1 == 2);   // Pass
    MT_CHECK_THAT(2 + 2 == 5);   // Fail (continues running)
    MT_CHECK_THAT(3 + 3 == 6);   // Pass
}

// A test with asserts: failures abort the test immediately
MT_DEFINE_TEST(addition_assert)
{
    MT_ASSERT_THAT(1 + 1 == 3);  // Fail (aborts here)
    MT_ASSERT_THAT(2 + 2 == 4);  // Never reached
}

int
main(void)
{
    MT_INIT();

    MT_RUN_TEST(always_pass);
    MT_RUN_TEST(always_fail);
    MT_RUN_TEST(addition_check);
    MT_RUN_TEST(addition_assert);

    MT_PRINT_SUMMARY();
    return MT_EXIT_CODE;
}
