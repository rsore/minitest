# minitest.h

![CI](https://github.com/rsore/minitest/actions/workflows/selftest.yml/badge.svg)

`minitest.h` is a single-header library: copy it into your project and start writing tests.
No build system or extra dependencies required.
It’s lightweight, portable across major platforms, and public domain.

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

## Tested platforms and compilers

### Linux
- **Compilers:** GCC 14, Clang 20
- **C standards:** C99, C11, C17, C23
- **C++ standards:** C++11, C++14, C++17, C++20, C++23, C++26
- **Flags:** `-Wall -Wextra -Werror -pedantic-errors`

### Windows
- **Compiler:** MSVC 2022
- **C standards:** C11, C17
- **C++ standards:** C++14, C++17, C++20, C++latest
- **Flags:** `/W4 /WX`

### macOS
- **Compiler:** Apple Clang (latest)
- **C standards:** C99, C11, C17, C2x
- **C++ standards:** C++11, C++14, C++17, C++20, C++2b
- **Flags:** `-Wall -Wextra -Werror -pedantic-errors`
