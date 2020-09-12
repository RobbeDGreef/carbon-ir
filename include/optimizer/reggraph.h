#pragma once

#include <core.h>
#include <register.h>
#include <oplist.h>

class RegisterNode
{
private:
    Register *m_reg;
    std::list<RegisterNode *> m_adj;

public:
    RegisterNode(Register *reg) { m_reg = reg; }
    Register *reg() { return m_reg; }
    std::list<RegisterNode *> &adj() { return m_adj; }

    int calcLife() { return m_reg->lifetime(); }
    int virt() { return m_reg->virt(); }
};

class RegisterGraph
{
private:
    std::vector<RegisterNode *> m_list;
    int m_physRegAmount;
    bool *m_usedRegs;
    OpList &m_statements;

private:
    int firstNonSpill(std::list<RegisterNode *> adj);
    void spillLongestLiving(RegisterNode *node);

public:
    RegisterGraph(int physAmount, OpList &tree);
    ~RegisterGraph();
    std::vector<RegisterNode *> &list() { return m_list; }
    OpList &statements() { return m_statements; }

public:
    void color();
    void connect(int i, int j);
    void refresh();

#ifdef MODE_DEBUG
    void show();
#endif
};
