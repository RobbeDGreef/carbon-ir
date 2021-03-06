#include <core.h>
#include <debug.h>
#include <config.h>
#include <getopt.h>

#include <scanner.h>
#include <parser.h>

#include <machine.h>

std::string genTemp()
{
    srand(time(NULL));

    std::string s = "/tmp/";
    for (int i = 0; i < FILE_TEMP_LIMIT; i++)
        s += 'a' + rand() % 26;

    return s;
}

const std::string helpString = "Usage: carbon-ir [options] file\n"
                               "Options:\n"
                               "\t--help (-h)\t\t\tDisplay this information\n"
                               "\t--compile (-S)\t\t\tOnly compile to assembly\n"
                               "\t--nolink (-c)\t\t\tOnly assemble the generated assembly code, do not link\n"
                               "\t--output (-o)\t [outputfile]\tSpecify the name of the output file (default is a.out)\n"
                               "\t--assembler (-a) [assembler]\tForce carbon to use a specific assembler\n"
                               "\t--linker (-l)\t [linker]\tForce carbon to use a specific linker\n"
                               "\t--machine (-m)\t [architecture]\tSpecify the type of machine you want to generate code for\n";

void printHelp()
{
    std::cout << helpString << "\n";
    exit(0);
}

#ifdef __EXEC
int main(int argc, char **argv)
{
    int opt;
    int opt_index = 0;

    int f_onlyCompile = 0;
    int f_onlyAssemble = 0;
    std::string outfile = "a.out";
    std::string asmFile = genTemp() + ".s";
    std::string linkFile = genTemp() + ".o";
    std::string machine = DEFAULT_MACHINE;

    std::string assembler = "";
    std::string linker = "";

    Generator *generator = nullptr;
    Optimizer *optimizer = nullptr;

    static struct option options[] =
        {
            /// Flags
            {"help", no_argument, 0, 'h'},
            {"compile", no_argument, &f_onlyCompile, 'S'},
            {"nolink", no_argument, &f_onlyAssemble, 'c'},

            /// Arguments
            {"output", required_argument, 0, 'o'},
            {"assembler", optional_argument, 0, 'a'},
            {"linker", optional_argument, 0, 'l'},
            {"machine", required_argument, 0, 'm'},
            {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "m:o:a:l:cSh", options, &opt_index)) != -1)
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

        case 'm':
            machine = std::string(optarg);
            break;

        case 'a':
            assembler = std::string(optarg);
            break;

        case 'l':
            linker = std::string(optarg);
            break;

        case 'h':
            printHelp();
            break;

        default:
            g_errsys.fatal("basic usage: carbon-ir -o [outfile] [infiles]\ntry carbin-ir -h for help");
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

    std::tie(generator, optimizer) = getMachine(machine);

    if (generator == nullptr || optimizer == nullptr)
        g_errsys.fatal("unknown machine type '" + machine + "'");

    for (; optind < argc; optind++)
    {
        Scanner scanner = Scanner(std::string(argv[optind]));
        Parser parser = Parser(scanner, generator, optimizer, asmFile);
        parser.parse();
    }

    /// @todo: I'm not proud of thess goto's
    if (f_onlyCompile)
        goto end;

    if (generator->assemble(asmFile, linkFile, assembler))
        g_errsys.fatal("failed to assemble binary");

    if (f_onlyAssemble)
        goto end;

    if (generator->link(linkFile, outfile, linker))
        g_errsys.fatal("failed to link binary");

end:
    /// @todo: this might cause undefined behaviour, look into it
    delete generator;
    delete optimizer;

    return 0;
}
#endif