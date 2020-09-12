#pragma once

#include <core.h>


#ifdef MODE_DEBUG
#define __DW(x) do { x; } while (0)
#define __STRINGIFY(x) #x
#define __TOSTR(x) __STRINGIFY(x)

#define __DBGSTR __CLR_YELLOW "[ DEBUG ] " __CLR_END
#define __DBGSTRR __CLR_RED "[ DEBUG ] " __CLR_END
#define __ASSRT_FAIL(x) __CLR_RED "[ ASSERTION FAILED ] " __CLR_BOLDWHITE "\"" #x "\"" __CLR_END " at line " __TOSTR(__LINE__) " in file " __FILE__ "\n"

/// Only these are allowed to be used in other source files

#define dbg_print(x) __DW(std::cout << __DBGSTR << x << "\n")
#define dbg_print_r(x) __DW(std::cout << __DBGSTRR << x << "\n")
#define dbg_printf(...) __DW(printf(__VA_ARGS__))
#define dbg_assert(x) __DW(if (!(x)) dbg_printf(__ASSRT_FAIL(x)))
#define dbg_call(x) x

#else

#define dbg_print(x)
#define dbg_print_r(x)
#define dbg_printf(...)
#define dbg_assert(x)
#define dbg_call(x)

#endif