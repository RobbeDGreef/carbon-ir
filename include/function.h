#pragma once

#include <type.h>
#include <attributes.h>

class Function
{
private:
    std::string m_name;
    Type m_retType;
    std::vector<Type> m_args;
    Attributes m_attributes;

public:
    Function(std::string name, Type t);
    std::string name() { return m_name; }
    Type retType() { return m_retType; }
    std::vector<Type> &args() { return m_args; }
    Attributes &attributes() { return m_attributes; }
    void setAttributes(Attributes attrs) { m_attributes = attrs; }
};