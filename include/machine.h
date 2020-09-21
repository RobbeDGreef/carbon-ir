#pragma once

#include <core.h>
#include <generator.h>
#include <optimizer/optimizer.h>

std::tuple<Generator *, Optimizer *> getMachine(std::string machine);