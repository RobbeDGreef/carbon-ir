#include <arch/aarch64/generator.h>
#include <debug.h>

int GeneratorAARCH64::assemble(std::string infile, std::string outfile, std::string assembler)
{
    return 0;
}

int GeneratorAARCH64::link(std::string infile, std::string outfile, std::string linker)
{
    return 0;
}
std::string GeneratorAARCH64::registerToString(Register r)
{
    if (r.spilled())
    {
    }
    else if (r.hintReg() != -1)
    {
        /// @todo: different bytetypes etc
        std::string pre = "x";
        dbg_assert(r.hintReg() < 16 && r.hintReg() > 8);
        return pre + std::to_string(r.hintReg() + 9);
    }

    dbg_assert(0);
    return "";
}

void GeneratorAARCH64::writeInst(std::string inst, std::string op)
{
    write("\t" + inst + "\t" + op);
}

void GeneratorAARCH64::writeInst(std::string inst, std::string ret, std::string r1, std::string r2)
{
    write("\t" + inst + "\t" + ret + ", " + r1 + ", " + r2);
}

void GeneratorAARCH64::writeInst(std::string inst, std::string ret, std::string val)
{
    write("\t" + inst + "\t" + ret + ", " + val);
}

void GeneratorAARCH64::writeInst(std::string inst, int val)
{
    write("\t" + inst + "\t" + std::to_string(val));
}

void GeneratorAARCH64::writeInst(std::string inst)
{
    write("\t" + inst);
}

void GeneratorAARCH64::writeMov(std::string r1, std::string r2)
{
    if (r1 != r2)
        writeInst("mov", r1, r2);
}