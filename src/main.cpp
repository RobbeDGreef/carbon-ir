#include <core.h>
#include <debug.h>
#include <config.h>

#include <scanner.h>
#include <arch/i386/generator.h>
#include <arch/i386/optimizer.h>
#include <parser.h>

#include <getopt.h>

std::string genTemp()
{
    srand(time(NULL));

    std::string s = "/tmp/";
    for (int i = 0; i < FILE_TEMP_LIMIT; i++)
        s += 'a' + rand() % 26;
    
    return s;
}

int main(int argc, char **argv)
{
    int opt;
    int opt_index = 0;

    int f_onlyCompile = 0;
    int f_onlyAssemble = 0;
    std::string outfile = "a.out";
    std::string asmFile = genTemp() + ".S";
    std::string linkFile = genTemp() + ".o";

    std::string assembler = "nasm";
    std::string linker = "gcc";
    std::string linkflags = "-m32";


    static struct option options[] =
    {
        /// Flags
        {"Compile", no_argument, &f_onlyCompile, 'S'},
        {"NoLink", no_argument, &f_onlyAssemble, 'c'},

        /// Arguments
        {"output", required_argument, 0, 'o'},
        {"assembler", optional_argument, 0, 'a'},
        {"linker", optional_argument, 0, 'l'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "o:a:l:cS", options, &opt_index)) != -1)
    {
        switch (opt)
        {
        case 0:
            /// Set a flag (handled by getopt_long)
            break;

        case 'o':
            outfile = std::string(optarg);
            break;
        
        case 'S':
            f_onlyCompile = true;
            break;
        
        case 'c':
            f_onlyAssemble = true;
            break;

        default:
            g_errsys.fatal("usage: glu -o <OUTFILE> <INFILES>");
        }
    }

    if (optind >= argc)
    {
        g_errsys.fatal("Error expected input files");
    }

    if (f_onlyCompile)
        asmFile = outfile;
    
    if (f_onlyAssemble)
        linkFile = outfile;

    dbg_print("Asmfile: " << asmFile);
    dbg_print("Linkfile: " << linkFile);

    for (; optind < argc; optind++)
    {
        Scanner scanner = Scanner(std::string(argv[optind]));
        GeneratorX86 generator = GeneratorX86(asmFile);
        OptimizerX86 optimizer = OptimizerX86(generator);

        Parser parser = Parser(scanner, generator, optimizer);
        parser.parse();
    }

    if (f_onlyCompile)
        return 0;
    
    int s = system((assembler + " -F dwarf -g -felf -o " + linkFile + " " + asmFile).c_str());

    if (s)
        g_errsys.fatal("failed to assemble binary");

    if (f_onlyAssemble)
        return 0;

    s = system((linker + " -o " + outfile + " " + linkFile + " " + linkflags).c_str());

    if (s)
        g_errsys.fatal("failed to link binary");

    return 0;
}