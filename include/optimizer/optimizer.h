#pragma once

#include <oplist.h>
#include <generator.h>
#include <optimizer/reggraph.h>

class Optimizer
{
private:
    Generator &m_generator;

protected:
    virtual bool graphCheck(RegisterGraph &graph) { return false; }
    virtual void prepAssignRegisters(OpList &list) {}

public:
    void assignRegisters(OpList &list);
    Optimizer(Generator &gen) : m_generator(gen) {}
    OpList optimize(OpList list);
};