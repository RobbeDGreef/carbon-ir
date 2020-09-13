#pragma once

#include <core.h> 
#include <register.h>
#include <type.h>

static const char *OpNames[] =
{
    "NULL", "reg", "intlit", "identifier",
    "add", "sub", "mul", "div", "mod",
    "call", "alloca", "store", "load", 
    "jmp", "jmpcond", "cmp",
    "return",
    "function",
    "destroy register",
    "spill", "spillload",

    "assign"
};

class OpQuad
{
public:
    enum Types
    {
        REG = 1, INTLIT, IDENTIFIER,
        ADD, SUB, MUL, DIV, MOD,
        CALL, ALLOCA, STORE, LOAD,
        JMP, JMPCOND, CMP,
        RETURN,
        FUNCTION,
        LABEL,
        DESTROYREG,
        SPILL, SPILLLOAD, SPILLSTORE,

        ASSIGN
    };

private:
    int m_operation = -1;
    int m_arg1 = -1;
    int m_arg2 = -1;
    int m_return = -1;

    /// @todo: doesn't this kinda break the name opQUAD
    int m_extra = -1;
    std::vector<int> m_args;
    
    Type m_type;

    std::string m_identifier;

public:
    OpQuad(int op, Type t);
    OpQuad(int op, std::string id);
    OpQuad(int op, int arg1, Type t);
    OpQuad(int op, int arg1, int arg2, int ret, Type t);
    
    int operation() { return m_operation; }
    int arg1() { return m_arg1; }
    int arg2() { return m_arg2; }
    int ret() { return m_return; }
    int extra() { return m_extra; }
    std::vector<int> extraArgs() { return m_args; }
    Type type() { return m_type; }
    std::string identifier() { return m_identifier; }

    void setReturn(int reg) { m_return = reg; }
    void setArg1(int r) { m_arg1 = r; }
    void setArg2(int r) { m_arg2 = r; }
    void setIdentifier(std::string s) { m_identifier = s; }
    void setExtra(int extra) { m_extra = extra; }
    void setExtraArgs(std::vector<int> args) { m_args = args; }

    static int tokToOp(int tok);
};

class OpList : public std::vector<OpQuad *>
{
private:
    RegisterList m_list;

private:
    void updateRegisterLifetime(OpQuad *op, int line);

public:
    void setRegList(RegisterList list) { m_list = list; }
    RegisterList &regList() { return m_list; }

    void push_newregs(RegisterList &list);
    void push_back(OpQuad *quad);
    void insert(int line, OpQuad *quad);

    /// Removes all the OpQuad pointers in the OpList.
    void destroy();

#ifdef MODE_DEBUG
    void print() {}
#endif
};