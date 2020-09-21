#pragma once

#include <core.h>
#include <config.h>

struct ctype;

class Type
{
private:
    int m_byteSize = -1;
    bool m_isFloat = false;

    int m_ptr = 0;

public:
    int byteSize() { return m_byteSize; }
    int ptr() { return m_ptr; }
    void setPtr(int ptr) { m_ptr = ptr; }
    Type() {}
    Type(struct ctype t);
    Type(int bytesize, bool isFloat = false)
    {
        m_byteSize = bytesize;
        m_isFloat = isFloat;
    }
};

class ArrayType
{
private:
    int m_arrSize = -1;
    Type m_type;

public:
    ArrayType(Type t, int amount)
    {
        m_type = t;
        m_arrSize = amount;
    }
    int arrSize() { return m_arrSize; }
    Type type() { return m_type; }
};
