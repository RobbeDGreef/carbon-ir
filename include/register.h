#pragma once

#include <type.h>
#include <core.h>

class Register
{
private:
    Type m_type;
    int m_virt = -1;

    int m_firstOcc = -1;
    int m_lastOcc = -1;

    bool m_spilled = false;
    int m_hintReg = -1;
    int m_hintSpill = -1;

public:
    Register(int virt, Type t) { m_virt = virt; m_type = t; }
    Register(Type t) { m_type = t;}

    int virt() { return m_virt; }
    int hintReg() { return m_hintReg; }
    bool spilled() { return m_spilled; }
    int hintSpill() { return m_hintSpill; }

    void setVirt(int v) { m_virt = v; }
    void setHintReg(int r) { m_hintReg = r; }
    void setHintSpill(int r) { m_hintSpill = r; }
    void setSpilled(bool s) { m_spilled = s; }
    
    /// This will set the last occurrence of the register to the given line parameter
    /// and also update the firstOcc variable if it was still uninitialized.
    void setLastOcc(int line);
    void setFirstOcc(int line) { m_firstOcc = line; }

    int lifetime() { return m_lastOcc - m_firstOcc; }
    int firstOcc() { return m_firstOcc; }
    int lastOcc() { return m_lastOcc; }
};

class RegisterList : public std::vector<Register>
{
private:
    int m_lastReg;

public:
    int findReg(int r);
    int lastReg();
    int addRegister(int r, Type t);
    int spillCount();
};