#include <arch/jvm/generator.h>

int GeneratorJVM::assemble(std::string infile, std::string outfile, std::string assembler)
{
    if (assembler == "")
    {
        g_errsys.fatal("Krakatau assembler not supplied, cannot assemble");
        return 1;
    }

    return system((assembler + " " + infile).c_str());
}

int GeneratorJVM::link(std::string infile, std::string outfile, std::string linker)
{
    return 0;
}

GeneratorJVM::GeneratorJVM(std::string outfile, std::string binname) : Generator(outfile)
{
    m_className = binname.substr(0, binname.find('.'));
}