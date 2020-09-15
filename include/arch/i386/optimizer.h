#pragma once

#include <optimizer/optimizer.h>
#include <optimizer/reggraph.h>

class OptimizerX86 : public Optimizer
{
protected:
    bool graphCheck(RegisterGraph &graph);
    void prepAssignRegisters(OpList &list);
};