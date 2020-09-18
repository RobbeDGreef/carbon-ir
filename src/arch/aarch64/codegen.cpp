#include <arch/aarch64/generator.h>

void GeneratorAARCH64::genIntlitLoad(Type t, int val, Register ret)
{
    writeInst("mov", registerToString(ret), "#" + std::to_string(val));
}
void GeneratorAARCH64::genGlobLoad(Type t, std::string glob, Register ret) {}
void GeneratorAARCH64::genMul(Type t, Register r1, Register r2, Register ret)
{
    writeInst("mul", registerToString(ret), registerToString(r1), registerToString(r2));
}

void GeneratorAARCH64::genAdd(Type t, Register r1, Register r2, Register ret)
{
    writeInst("add", registerToString(ret), registerToString(r1), registerToString(r2));
}

void GeneratorAARCH64::genSub(Type t, Register r1, Register r2, Register ret)
{
    writeInst("sub", registerToString(ret), registerToString(r1), registerToString(r2));
}
void GeneratorAARCH64::genDiv(Type t, Register r1, Register r2, Register ret)
{
}
void GeneratorAARCH64::genMod(Type t, Register r1, Register r2, Register ret)
{
}

void GeneratorAARCH64::genReg(Type t, Register r, Register ret) {}

void GeneratorAARCH64::genAlloca(Type t, Register r, Register ret) {}
void GeneratorAARCH64::genStore(Type t, Register r1, Register r2) {}
void GeneratorAARCH64::genLoad(Type t, Register r, Register ret) {}
void GeneratorAARCH64::genLoad(Type t, std::string glob, Register ret) {}
void GeneratorAARCH64::genSpillLoad(Type t, Register r, Register ret) {}
void GeneratorAARCH64::genSpillStore(Type t, Register r, Register ret) {}
void GeneratorAARCH64::genFunctionCall(Type t, std::string function, Register ret, std::vector<Register> args)
{
}
void GeneratorAARCH64::genReturn(Type t, Register ret)
{
    writeMov("r0", registerToString(ret));
}
void GeneratorAARCH64::genJmpCond(Type t, int op, Register r1, Register r2, std::string label) {}
void GeneratorAARCH64::genLabel(std::string label) {}
void GeneratorAARCH64::genJmp(std::string label) {}
void GeneratorAARCH64::genCmp(Type t, int op, Register r1, Register r2, Register ret) {}
void GeneratorAARCH64::genFunction(Type t, Function f)
{
    if (f.attributes().getBoolValueOf("global") == true)
        write(".globl\t " + f.name());
    write(".type\t" + f.name() + ", %function");
    write(f.name() + ":");

    /// Now the function preamble
    writeInst("push", "{fp, lr}");
    writeInst("add", "fp, sp, #4");
    writeInst("sub", "sp, sp, #8");
}
void GeneratorAARCH64::genGlobalVariable(std::string name, ArrayType t, std::vector<LARGEINT> init) {}

void GeneratorAARCH64::genSetupFile(std::string filename)
{
    write(".text");
}

void GeneratorAARCH64::genExternSymbol(std::string sym) {}
