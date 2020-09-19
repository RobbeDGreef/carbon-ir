#pragma once

#include <core.h>
#include <generator.h>
#include <optimizer.h>

std::tuple<Generator *, Optimizer *> getMachine(std::string machine);