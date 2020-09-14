#include <optimizer/optimizer.h>
#include <optimizer/reggraph.h>
#include <debug.h>

/// @todo: WHYYYY do I use graph coloring instead of a linear scan??????

RegisterGraph::RegisterGraph(int physAmount, OpList &list) : m_statements(list)
{
    m_physRegAmount = physAmount;
    m_usedRegs = new bool[physAmount]();

    refresh();
}

RegisterGraph::~RegisterGraph()
{
    delete[] m_usedRegs;

    for (RegisterNode *n : m_list)
        delete n;
}

static bool checkLifetimeOverlap(Register &r1, Register &r2)
{
    if (r1.firstOcc() <= r2.firstOcc() && r1.lastOcc() >= r2.firstOcc())
        return true;

    if (r2.firstOcc() <= r1.firstOcc() && r2.lastOcc() >= r1.firstOcc())
        return true;

    return false;
}

void RegisterGraph::refresh()
{
    /// @todo: this is not an optimal solution
    // First remove the previous elements
    for (RegisterNode *n : m_list)
        delete n;

    m_list.clear();

    // Repopulate the list with registernodes
    RegisterList &list = m_statements.regList();

    for (Register &r : list)
        m_list.push_back(new RegisterNode(&r));

    // Check for lifetime overlaps and connect the nodes in the graph
    for (auto i = list.begin(); i != std::prev(list.end()); ++i)
        for (auto j = std::next(i); j != list.end(); ++j)
            if (checkLifetimeOverlap(*i, *j))
                connect(std::distance(list.begin(), i), std::distance(list.begin(), j));
}

int RegisterGraph::firstNonSpill(std::list<RegisterNode *> adj)
{
    std::vector<bool> tot;
    tot.resize(adj.size(), false);

    for (RegisterNode *n : adj)
        if (n->reg()->spilled())
            tot[n->reg()->hintSpill()] = true;

    int i = 0;
    for (bool n : tot)
    {
        if (!n)
            return i;
        i++;
    }
    return tot.size();
}

void RegisterGraph::spillLongestLiving(RegisterNode *node)
{
    // Populate a list with the wanted nodes
    /// @todo: This is ugly but for some weird reason there was no other way to do it
    std::list<RegisterNode *> sorted;
    for (RegisterNode *n : node->adj())
        if (!n->reg()->spilled())
            sorted.push_back(n);

    sorted.push_back(node);

    sorted.sort([](const RegisterNode *a, const RegisterNode *b) { return ((RegisterNode *)a)->calcLife() < ((RegisterNode *)b)->calcLife(); });

    RegisterNode *longest = sorted.back();
    if (longest != node)
        node->reg()->setHintReg(longest->reg()->hintReg());

    longest->reg()->setHintReg(-1);
    longest->reg()->setSpilled(true);
    longest->reg()->setHintSpill(firstNonSpill(longest->adj()));
}

void RegisterGraph::color()
{
    for (auto i = m_list.begin(); i != m_list.end(); ++i)
    {
        RegisterNode *node = *i;

        if (node->reg()->spilled())
            continue;

        // Mark the used colors
        for (auto j = node->adj().begin(); j != node->adj().end(); ++j)
            if ((*j)->reg()->hintReg() != -1)
                m_usedRegs[(*j)->reg()->hintReg()] = true;

        // Find a unused color or spill
        int c = -1;
        for (int j = 0; j < m_physRegAmount; ++j)
        {
            if (!m_usedRegs[j])
            {
                c = j;
                break;
            }
        }

        if (c == -1)
        {
            dbg_print_r("AAAAAAAAAAAAa NO LEFT");
            //for(;;);
            // Spill oldest in use virtreg
            spillLongestLiving(node);
        }
        else
            node->reg()->setHintReg(c);

        // Unmark adjacent registers
        auto j = node->adj().begin();
        for (; j != node->adj().end(); ++j)
            if ((*j)->reg()->hintReg() != -1)
                m_usedRegs[(*j)->reg()->hintReg()] = false;
    }

#ifdef MODE_DEBUG
    for (RegisterNode *r : m_list)
    {
        std::cout << "Register: " << r->virt() << " colored with: " << r->reg()->hintReg() << "\n";
        if (r->reg()->spilled())
            std::cout << "  but spill: " << r->reg()->hintSpill() << "\n";
    }
#endif
}

void RegisterGraph::connect(int a, int b)
{
    m_list[a]->adj().push_back(m_list[b]);
    m_list[b]->adj().push_back(m_list[a]);
}

void Optimizer::assignRegisters(OpList &list, Generator *gen)
{
    // Call the preperation function
    prepAssignRegisters(list);

    // Build the graph and color it
    RegisterGraph graph(gen->registerAmount(), list);

    dbg_call(graph.show());
    graph.color();
    while (graphCheck(graph))
    {
        graph.refresh();
        dbg_call(graph.show());
        graph.color();
    }
}

#ifdef MODE_DEBUG
void RegisterGraph::show()
{
    for (RegisterNode *n : m_list)
    {
        dbg_printf("virt %i overlaps with %i registers\n", n->virt(), (int)n->adj().size());
        for (RegisterNode *adj : n->adj())
            dbg_printf("\toverlaps with %i\n", adj->virt());
    }
}
#endif