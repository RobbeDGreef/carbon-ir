#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <list>

#define __CLR_END "\033[0m"
#define __CLR_YELLOW "\033[33;1m"
#define __CLR_RED "\033[31;1m"
#define __CLR_BOLDWHITE "\033[37;1m"
#define BOLD(x) __CLR_BOLDWHITE x __CLR_END

#include <errsys.h>
extern ErrorSystem g_errsys;