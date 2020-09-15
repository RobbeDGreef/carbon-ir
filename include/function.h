#pragma once

#include <type.h>

class Function
{
private:
    std::string m_name;
    Type m_retType;
    std::vector<Type> m_args;

public:
    Function(std::string name, Type t);
    std::string name() { return m_name; }
    Type retType() { return m_retType; }
    std::vector<Type> &args() { return m_args; }
};