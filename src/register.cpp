#include <register.h>

int RegisterList::lastReg()
{
    for (Register &r : *this)
    {
        if (r.virt() > m_lastReg)
            m_lastReg = r.virt();
    }

    return m_lastReg;
}


int RegisterList::addRegister(int reg)
{
    int i = 0;
    for (Register &r : *this)
    {
        if (r.virt() == reg)
            return i;
        i++;
    }

    push_back(Register(reg));
    return size() - 1;
}

void Register::setLastOcc(int line)
{
    if (m_firstOcc == -1)
        m_firstOcc = line;
    
    m_lastOcc = line;
}

int RegisterList::findReg(int reg)
{
    int i = 0;
    for (Register &r : *this)
    {
        if (r.virt() == reg)
            return i;

        ++i;
    }

    return -1;
}

int RegisterList::spillCount()
{
    int highest = -1;
    for (Register &r: *this)
        if (r.spilled())
            if (r.hintSpill() > highest)
                highest = r.hintSpill();
    
    return highest + 1;
}