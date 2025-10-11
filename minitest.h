/**
 *   __ __ _ __  _ _ _____ ___  __ _____
 *  |  V  | |  \| | |_   _| __/' _/_   _|
 *  | \_/ | | | ' | | | | | _|`._`. | |
 *  |_| |_|_|_|\__|_| |_| |___|___/ |_|
 *
 *  minitest.h — v1.1.3
 *
 *
 *  This file is placed in the public domain.
 *  See end of file for license details.
 *
 *
 *  Example usage:
 *
 *    #include "minitest.h"
 *
 *    MT_DEFINE_TEST(addition_check)
 *    {
 *        // Non-fatal: test continues even if this fails
 *        MT_CHECK_THAT(1 + 1 == 2);
 *        MT_CHECK_THAT(2 + 2 == 5); // Fails, but test still runs to end
 *        MT_CHECK_THAT(1 + 3 == 4);
 *    }
 *
 *    MT_DEFINE_TEST(addition_assert)
 *    {
 *        // Fatal: test stops immediately on failure
 *        MT_ASSERT_THAT(1 + 1 == 3); // Fails and aborts this test
 *        MT_ASSERT_THAT(2 + 2 == 4); // Never reached
 *    }
 *
 *    int
 *    main(void)
 *    {
 *        MT_INIT();
 *
 *        MT_RUN_TEST(addition_check);
 *        MT_RUN_TEST(addition_assert);
 *
 *        MT_PRINT_SUMMARY();
 *        return MT_EXIT_CODE;
 *    }
 *
 *
 *  Customization:
 *   - Define MT_NO_ANSI_ESCAPE_CODES to disable colored output.
 *   - Define MT_PRINT_OK(...) and/or MT_PRINT_ERR(...) to override where output is written.
 *     (e.g., redirect to a log file or GUI console. Default is stdout/stderr.
 *      Alternative macros must support printf formatting).
 **/

#ifndef MINITEST_H_
#define MINITEST_H_

#if !defined(MT_PRINT_OK) || !defined(MT_PRINT_ERR)
#include <stdio.h>
#endif

#ifndef MT_PRINT_OK
// I would really prefer to have PRINT_OK output to stdout
// by default. However, Github runners tend to buffer stdout
// but not stderr, leading to outputs in the wrong order.
// So I send everything to stderr.
#define MT_PRINT_OK(...) fprintf(stderr, __VA_ARGS__)
#endif

#ifndef MT_PRINT_ERR
#define MT_PRINT_ERR(...) fprintf(stderr, __VA_ARGS__)
#endif

#ifndef MT_NO_ANSI_ESCAPE_CODES
#define MT__INTERNAL__ANSI_CLEAR_  "\033[0m"
#define MT__INTERNAL__ANSI_BOLD_   "\033[1m"
#define MT__INTERNAL__ANSI_RED_    "\033[31m"
#define MT__INTERNAL__ANSI_GREEN_  "\033[32m"
#define MT__INTERNAL__ANSI_BLUE_   "\033[36m"
#else
#define MT__INTERNAL__ANSI_CLEAR_
#define MT__INTERNAL__ANSI_BOLD_
#define MT__INTERNAL__ANSI_RED_
#define MT__INTERNAL__ANSI_GREEN_
#define MT__INTERNAL__ANSI_BLUE_
#endif

static int mt__internal__test_success_;
static int mt__internal__total_test_success_count_;
static int mt__internal__total_test_failure_count_;
static int mt__internal__total_check_success_count_;
static int mt__internal__total_check_failure_count_;
static int mt__internal__total_assert_success_count_;
static int mt__internal__total_assert_failure_count_;

#define MT__INTERNAL__NAMEOF_TEST_FUNC_(name) mt_test_##name

#define MT_INIT()                                      \
    do {                                               \
        mt__internal__test_success_ = 0;               \
        mt__internal__total_test_success_count_ = 0;   \
        mt__internal__total_test_failure_count_ = 0;   \
        mt__internal__total_check_success_count_ = 0;  \
        mt__internal__total_check_failure_count_ = 0;  \
        mt__internal__total_assert_success_count_ = 0; \
        mt__internal__total_assert_failure_count_ = 0; \
    } while (0)

#define MT_DEFINE_TEST(name) \
    static void MT__INTERNAL__NAMEOF_TEST_FUNC_(name)(void)

#define MT_RUN_TEST(name)                                                                                                                 \
    do {                                                                                                                                  \
        MT_PRINT_OK(MT__INTERNAL__ANSI_BOLD_ MT__INTERNAL__ANSI_GREEN_ "[ RUN         ]" MT__INTERNAL__ANSI_CLEAR_ " %s\n", #name);       \
        mt__internal__test_success_ = 1;                                                                                                  \
        MT__INTERNAL__NAMEOF_TEST_FUNC_(name)();                                                                                          \
        if (mt__internal__test_success_) {                                                                                                \
            MT_PRINT_OK(MT__INTERNAL__ANSI_BOLD_ MT__INTERNAL__ANSI_GREEN_ "[          OK ]" MT__INTERNAL__ANSI_CLEAR_ " %s\n\n", #name); \
            mt__internal__total_test_success_count_ += 1;                                                                                 \
        } else {                                                                                                                          \
            MT_PRINT_OK(MT__INTERNAL__ANSI_BOLD_ MT__INTERNAL__ANSI_RED_ "[     FAILURE ]" MT__INTERNAL__ANSI_CLEAR_ " %s\n\n", #name);   \
            mt__internal__total_test_failure_count_ += 1;                                                                                 \
        }                                                                                                                                 \
    } while (0)

#define MT_CHECK_THAT(cond)                                                                                                              \
    do {                                                                                                                                 \
        if ((cond)) {                                                                                                                    \
            MT_PRINT_OK(MT__INTERNAL__ANSI_GREEN_ "%s:%d: Check succeeded: %s\n" MT__INTERNAL__ANSI_CLEAR_, __FILE__, __LINE__, #cond);  \
            mt__internal__total_check_success_count_ += 1;                                                                               \
        } else {                                                                                                                         \
            MT_PRINT_ERR(MT__INTERNAL__ANSI_RED_ "%s:%d: Check failed: %s\n" MT__INTERNAL__ANSI_CLEAR_, __FILE__, __LINE__, #cond);      \
            mt__internal__total_check_failure_count_ += 1;                                                                               \
            mt__internal__test_success_ = 0;                                                                                             \
        }                                                                                                                                \
    } while (0)

#define MT_ASSERT_THAT(cond)                                                                                                                 \
    do {                                                                                                                                     \
        if ((cond)) {                                                                                                                        \
            MT_PRINT_OK(MT__INTERNAL__ANSI_GREEN_ "%s:%d: Assertion succeeded: %s\n" MT__INTERNAL__ANSI_CLEAR_, __FILE__, __LINE__, #cond);  \
            mt__internal__total_assert_success_count_ += 1;                                                                                  \
        } else {                                                                                                                             \
            MT_PRINT_ERR(MT__INTERNAL__ANSI_RED_ "%s:%d: Assertion failed: %s\n" MT__INTERNAL__ANSI_CLEAR_, __FILE__, __LINE__, #cond);      \
            mt__internal__total_assert_failure_count_ += 1;                                                                                  \
            mt__internal__test_success_ = 0;                                                                                                 \
            return;                                                                                                                          \
        }                                                                                                                                    \
    } while (0)

#define MT_PRINT_SUMMARY()                                                                                                                            \
    do {                                                                                                                                              \
        MT_PRINT_OK(MT__INTERNAL__ANSI_BLUE_ MT__INTERNAL__ANSI_BOLD_ "===== Summary =====\n\n" MT__INTERNAL__ANSI_CLEAR_);                           \
        const int mt__internal__total_tests_run_ =                                                                                                    \
            mt__internal__total_test_success_count_ + mt__internal__total_test_failure_count_;                                                        \
        MT_PRINT_OK(MT__INTERNAL__ANSI_BLUE_ MT__INTERNAL__ANSI_BOLD_ "Total tests run: .............. %d\n" MT__INTERNAL__ANSI_CLEAR_,               \
                    mt__internal__total_tests_run_);                                                                                                  \
        if (mt__internal__total_test_success_count_ == mt__internal__total_tests_run_) {                                                              \
            MT_PRINT_OK(MT__INTERNAL__ANSI_GREEN_ MT__INTERNAL__ANSI_BOLD_ "Total tests succeeded: ........ %d\n" MT__INTERNAL__ANSI_CLEAR_,          \
                        mt__internal__total_test_success_count_);                                                                                     \
            MT_PRINT_OK(MT__INTERNAL__ANSI_GREEN_ MT__INTERNAL__ANSI_BOLD_ "Total tests failed: ........... %d\n\n" MT__INTERNAL__ANSI_CLEAR_,        \
                        mt__internal__total_test_failure_count_);                                                                                     \
        } else {                                                                                                                                      \
            MT_PRINT_ERR(MT__INTERNAL__ANSI_RED_ MT__INTERNAL__ANSI_BOLD_ "Total tests succeeded: ........ %d\n" MT__INTERNAL__ANSI_CLEAR_,           \
                        mt__internal__total_test_success_count_);                                                                                     \
            MT_PRINT_ERR(MT__INTERNAL__ANSI_RED_ MT__INTERNAL__ANSI_BOLD_ "Total tests failed: ........... %d\n\n" MT__INTERNAL__ANSI_CLEAR_,         \
                        mt__internal__total_test_failure_count_);                                                                                     \
        }                                                                                                                                             \
                                                                                                                                                      \
        const int mt__internal__total_checks_done_ =                                                                                                  \
            mt__internal__total_check_success_count_ + mt__internal__total_check_failure_count_;                                                      \
        MT_PRINT_OK(MT__INTERNAL__ANSI_BLUE_ MT__INTERNAL__ANSI_BOLD_ "Total checks done: ................ %d\n" MT__INTERNAL__ANSI_CLEAR_,           \
                    mt__internal__total_checks_done_);                                                                                                \
        if (mt__internal__total_check_success_count_ == mt__internal__total_checks_done_) {                                                           \
            MT_PRINT_OK(MT__INTERNAL__ANSI_GREEN_ MT__INTERNAL__ANSI_BOLD_ "Total checks succeeded: ........... %d\n" MT__INTERNAL__ANSI_CLEAR_,      \
                        mt__internal__total_check_success_count_);                                                                                    \
            MT_PRINT_OK(MT__INTERNAL__ANSI_GREEN_ MT__INTERNAL__ANSI_BOLD_ "Total checks failed: .............. %d\n\n" MT__INTERNAL__ANSI_CLEAR_,    \
                        mt__internal__total_check_failure_count_);                                                                                    \
        } else {                                                                                                                                      \
            MT_PRINT_ERR(MT__INTERNAL__ANSI_RED_ MT__INTERNAL__ANSI_BOLD_ "Total checks succeeded: ........... %d\n" MT__INTERNAL__ANSI_CLEAR_,       \
                        mt__internal__total_check_success_count_);                                                                                    \
            MT_PRINT_ERR(MT__INTERNAL__ANSI_RED_ MT__INTERNAL__ANSI_BOLD_ "Total checks failed: .............. %d\n\n" MT__INTERNAL__ANSI_CLEAR_,     \
                        mt__internal__total_check_failure_count_);                                                                                    \
        }                                                                                                                                             \
                                                                                                                                                      \
        const int mt__internal__total_asserts_done_ =                                                                                                 \
            mt__internal__total_assert_success_count_ + mt__internal__total_assert_failure_count_;                                                    \
        MT_PRINT_OK(MT__INTERNAL__ANSI_BLUE_ MT__INTERNAL__ANSI_BOLD_ "Total assertions done: ................ %d\n" MT__INTERNAL__ANSI_CLEAR_,       \
                    mt__internal__total_asserts_done_);                                                                                               \
        if (mt__internal__total_assert_success_count_ == mt__internal__total_asserts_done_) {                                                         \
            MT_PRINT_OK(MT__INTERNAL__ANSI_GREEN_ MT__INTERNAL__ANSI_BOLD_ "Total assertions succeeded: ........... %d\n" MT__INTERNAL__ANSI_CLEAR_,  \
                        mt__internal__total_assert_success_count_);                                                                                   \
            MT_PRINT_OK(MT__INTERNAL__ANSI_GREEN_ MT__INTERNAL__ANSI_BOLD_ "Total assertions failed: .............. %d\n" MT__INTERNAL__ANSI_CLEAR_,  \
                        mt__internal__total_assert_failure_count_);                                                                                   \
        } else {                                                                                                                                      \
            MT_PRINT_ERR(MT__INTERNAL__ANSI_RED_ MT__INTERNAL__ANSI_BOLD_ "Total assertions succeeded: ........... %d\n" MT__INTERNAL__ANSI_CLEAR_,   \
                        mt__internal__total_assert_success_count_);                                                                                   \
            MT_PRINT_ERR(MT__INTERNAL__ANSI_RED_ MT__INTERNAL__ANSI_BOLD_ "Total assertions failed: .............. %d\n" MT__INTERNAL__ANSI_CLEAR_,   \
                        mt__internal__total_assert_failure_count_);                                                                                   \
        }                                                                                                                                             \
    } while (0)

#define MT_EXIT_CODE mt__internal__total_test_failure_count_ == 0 ? 0 : 1

#endif // MINITEST_H_



/*
  LICENSE

  This is free and unencumbered software released into the public domain.

  Anyone is free to copy, modify, publish, use, compile, sell, or
  distribute this software, either in source code form or as a compiled
  binary, for any purpose, commercial or non-commercial, and by any
  means.

  In jurisdictions that recognize copyright laws, the author or authors
  of this software dedicate any and all copyright interest in the
  software to the public domain. We make this dedication for the benefit
  of the public at large and to the detriment of our heirs and
  successors. We intend this dedication to be an overt act of
  relinquishment in perpetuity of all present and future rights to this
  software under copyright law.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.
*/
