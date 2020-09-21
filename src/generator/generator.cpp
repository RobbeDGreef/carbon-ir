#include <generator.h>
#include <debug.h>

int Generator::writeOutfile(std::string outfile)
{
    FILE *fp = fopen(outfile.c_str(), "w+");
    if (!fp)
    {
        g_errsys.fatal("could not open output file '" + outfile + "'");
        return -1;
    }

    for (std::string s : m_internalOutBuf)
    {
        fprintf(fp, "%s\n", s.c_str());
    }
    fflush(fp);
    return 0;
}

void Generator::write(std::string s)
{
#ifdef MODE_DEBUG
    static int iter = 1;
    std::cout << iter << ": " << s << "\n";
    iter++;
#endif

    m_internalOutBuf.push_back(s);
}

void Generator::insert(std::string s, int line)
{
    m_internalOutBuf.insert(m_internalOutBuf.begin() + line, s);
}

void Generator::feedGenerate(OpList list)
{
    m_opLine = 0;
    for (OpQuad *op : list)
    {
        generate(op);
        m_opLine++;
    }
}

void Generator::setRegList(RegisterList list)
{
    m_regData = list;
}

Register &Generator::findReg(int r)
{
    dbg_assert(r >= 0 && r <= (int)m_regData.size());
    return m_regData[r];
}