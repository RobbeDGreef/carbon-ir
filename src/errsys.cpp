#include <errsys.h>
#include <core.h>

ErrorSystem g_errsys;

void ErrorSystem::fatal(std::string reason)
{
    std::cout << __CLR_RED "Fatal error" __CLR_END ": " << reason << "\n";
    exit(1);
}

void ErrorSystem::syntaxError(std::string reason)
{
    std::cout << __CLR_RED "Syntax error" __CLR_END ": " << reason << "\n";
    exit(1);
}