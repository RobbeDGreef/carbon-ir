#include <arch/i386/generator.h>
#include <debug.h>

void GeneratorX86::feedGenerate(OpList list)
{
    /// This piece of code just subtracts the correct amount from esp so that the
    /// stack frame of the function is correct
    int spilled = list.regList().spillCount();
    dbg_print("spillcount: " << spilled);
    if (spilled)
        writeInst("sub", "esp", std::to_string(spilled * 4));

    m_opLine = 0;
    for (OpQuad *op : list)
    {
        generate(op);
        m_opLine++;
    }
}

bool GeneratorX86::physInUse(int reg)
{
    for (Register &r : m_regData)
        if (r.hintReg() == reg)
        {
            if (r.firstOcc() <= m_opLine && r.lastOcc() >= m_opLine)
                return true;
        }

    return false;
}

std::string GeneratorX86::registerToString(Register r, std::string *list)
{
    if (r.spilled())
    {
        if (r.hintSpill() >= 0)
            return "[ebp-" + std::to_string(4 + 4 * r.hintSpill()) + "]";
        else
            return "[ebp+" + std::to_string(4 + (-r.hintSpill()) * 4) + "]";
    }

    if (r.hintReg() != -1)
    {
        if (!list)
            list = m_registers;

        return list[r.hintReg()];
    }

    return "";
}

void GeneratorX86::writeInst(std::string inst, std::string dest, std::string src)
{
    write("\t" + inst + "\t" + dest + ", " + src);
}

void GeneratorX86::writeInst(std::string inst, std::string arg)
{
    write("\t" + inst + "\t" + arg);
}

void GeneratorX86::writeInst(std::string inst)
{
    write("\t" + inst);
}

void GeneratorX86::writeMov(std::string dest, std::string src)
{
    if (dest != src)
        writeInst("mov", dest, src);
}

int GeneratorX86::assemble(std::string infile, std::string outfile, std::string assembler)
{
    if (assembler != "")
        return system((assembler + " -o " + outfile + " " + infile).c_str());

    return system(("nasm -F dwarf -g -felf -o " + outfile + " " + infile).c_str());
}

int GeneratorX86::link(std::string infile, std::string outfile, std::string linker)
{
    if (linker != "")
        return system((linker + " -o " + outfile + " " + infile).c_str());

    return system(("gcc -o " + outfile + " " + infile + " -m32").c_str());
}