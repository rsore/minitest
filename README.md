# minitest.h

![CI](https://github.com/rsore/minitest/actions/workflows/selftest.yml/badge.svg)

A minimal single‑header test library for C.
Public domain (Unlicense) — free for any use.

## Features
- Single file, drop‑in.
- Colored output (ANSI) with optional disable flag.
- `CHECK_THAT` (non‑fatal) and `ASSERT_THAT` (fatal) assertions.
- Customizable output via `MT_PRINT_OK` and `MT_PRINT_ERR`.

## Example

```c
#include "minitest.h"

MT_DEFINE_TEST(addition_check)
{
    // Non-fatal: test continues even if this fails
    MT_CHECK_THAT(1 + 1 == 2);
    MT_CHECK_THAT(2 + 2 == 5); // Fails, but test still runs to end
    MT_CHECK_THAT(1 + 3 == 4);
}

MT_DEFINE_TEST(addition_assert)
{
    // Fatal: test stops immediately on failure
    MT_ASSERT_THAT(1 + 1 == 3); // Fails and aborts this test
    MT_ASSERT_THAT(2 + 2 == 4); // Never reached
}

int
main(void)
{
    MT_INIT();

    MT_RUN_TEST(addition_check);
    MT_RUN_TEST(addition_assert);

    MT_PRINT_SUMMARY();
    return MT_EXIT_CODE;
}
```