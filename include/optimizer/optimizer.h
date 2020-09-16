#pragma once

#include <oplist.h>
#include <generator.h>
#include <optimizer/reggraph.h>

class Optimizer
{
private:
    OpList twoOpOptimize(OpList);

protected:
    /// This function will run after the register graph was made and colored,
    /// you can insert spills in this function and if the function returns
    /// true the graphcoloring will again be ran.
    virtual bool graphCheck(RegisterGraph &graph) { return false; }

    /// This function will run before the register graph is build.
    virtual void prepAssignRegisters(OpList &list) {}

    /// This flag will optimize the code in a way that will result in less
    /// code spills for architectures with two operant opcodes like x86 etc.
    virtual bool twoRegisterOperants() { return true; }

public:
    void assignRegisters(OpList &list, Generator *gen);
    OpList optimize(OpList list);
};