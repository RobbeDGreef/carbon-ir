#pragma once

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
    Type(int bytesize, bool isFloat=false) { m_byteSize = bytesize; m_isFloat = isFloat; }
};