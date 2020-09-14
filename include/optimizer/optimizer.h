#pragma once

#include <oplist.h>
#include <generator.h>
#include <optimizer/reggraph.h>

class Optimizer
{
protected:
    virtual bool graphCheck(RegisterGraph &graph) { return false; }
    virtual void prepAssignRegisters(OpList &list) {}

public:
    void assignRegisters(OpList &list, Generator *gen);
    OpList optimize(OpList list);
};