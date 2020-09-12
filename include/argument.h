#pragma once

#include <core.h>
#include <register.h>

class Generator;

#define UNDEF_ARG Argument()

class Argument
{
public:
    enum Types
    {
        UNDEF = -1,
        REG,
        INTLIT
    };

private:
    int m_type = -1;

    /// @todo: maybe in a union
    Register m_reg;
    int m_intValue;

public:
    std::string toString(Generator *gen);
    Argument() { m_reg = reg(); m_intValue = -1; }
    Argument(Register &r) { m_reg = r; m_type = Types::REG; }
    Argument(int i) { m_intValue = i; m_type = Types::INTLIT; }

    int type() { return m_type; }
    int intValue() { return m_intValue; }
    Register &reg() { return m_reg; }
};