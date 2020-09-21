#include <core.h>

#include <generator.h>
#include <optimizer/optimizer.h>

#include <arch/i386/generator.h>
#include <arch/i386/optimizer.h>
#include <arch/Z80/generator.h>
#include <arch/Z80/optimizer.h>
#include <arch/aarch64/generator.h>
#include <arch/aarch64/optimizer.h>

std::tuple<Generator *, Optimizer *> getMachine(std::string machine)
{
    Generator *generator = nullptr;
    Optimizer *optimizer = nullptr;
    switch (machine[0])
    {
    case 'a':
        if (!machine.compare("aarch64") || !machine.compare("armv8"))
        {
            generator = new GeneratorAARCH64();
            optimizer = new OptimizerAARCH64();
        }
    case 'x':
        if (!machine.compare("x86"))
        {
            generator = new GeneratorX86();
            optimizer = new OptimizerX86();
        }
        break;

    case 'e':
        if (!machine.compare("Z80"))
        {
            generator = new GeneratorZ80();
            optimizer = new OptimizerZ80();
        }
    }

    return {generator, optimizer};
}