#include <oplist.h>
#include <debug.h>

int OpQuad::tokToOp(int tok)
{
    if (tok <= Types::ASSIGN && tok > 0)
        return tok;

    dbg_assert(0);
    return -1;
}

OpQuad::OpQuad(int op, Type t)
{
    m_operation = op;
    m_type = t;
}

OpQuad::OpQuad(int op, int arg1, Type t)
{
    m_operation = op;
    m_arg1 = arg1;
    m_type = t;
}

OpQuad::OpQuad(int op, std::string id)
{
    m_operation = op;
    m_identifier = id;
}


OpQuad::OpQuad(int op, int arg1, int arg2, int ret, Type t)
{
    m_operation = op;
    m_arg1 = arg1;
    m_arg2 = arg2;
    m_return = ret;
    m_type = t;
}

void OpList::destroy()
{
    for (OpQuad *o : *this)
        delete o;
}

void OpList::updateRegisterLifetime(OpQuad *quad, int line)
{
    switch (quad->operation())
    {
    case OpQuad::Types::INTLIT:
        break;
    
    case OpQuad::Types::CALL:
        for (int r : quad->extraArgs())
            m_list[r].setLastOcc(line);
        
        break;

    default:
        if (quad->arg1() != -1)
            m_list[quad->arg1()].setLastOcc(line);
        if (quad->arg2() != -1)
            m_list[quad->arg2()].setLastOcc(line);
    }

    if (quad->ret() != -1)
            m_list[quad->ret()].setLastOcc(line);
}

void OpList::push_back(OpQuad *quad)
{
    dbg_print("pushed: " << quad->operation());
    updateRegisterLifetime(quad, size());
    std::vector<OpQuad *>::push_back(quad);
}

void OpList::insert(int line, OpQuad *quad)
{
    for (Register &r : m_list)
    {
        if (r.firstOcc() >= line)
        {
            r.setFirstOcc(r.firstOcc() + 1);
        }

        if (r.lastOcc() >= line)
        {
            r.setLastOcc(r.lastOcc() + 1);
        }
    }
    updateRegisterLifetime(quad, line);
    std::vector<OpQuad *>::insert(begin() + line, quad);
}

void OpList::push_newregs(RegisterList &list)
{
    m_list.insert(m_list.end(), list.begin() + m_list.size(), list.end());
}