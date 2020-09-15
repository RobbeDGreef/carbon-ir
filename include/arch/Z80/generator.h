#pragma once

#include <generator.h>

enum
{
    BC,
    DE,
    HL,

    REGAMOUNT24
};

enum
{
    A,
    B,
    C,
    D,
    E,
    H,
    L,

    REGAMOUNT8
};

class GeneratorZ80 : public Generator
{
private:
    std::string m_registers24[REGAMOUNT24] = {"bc", "de", "hl"};
    std::string m_registers8[REGAMOUNT8] = {"a", "b", "c", "d", "e", "h", "l"};

protected:
    void genIntlitLoad(Type t, int val, Register ret) {}
    void genGlobLoad(Type t, std::string glob, Register ret) {}
    void genMul(Type t, Register r1, Register r2, Register ret) {}
    void genAdd(Type t, Register r1, Register r2, Register ret) {}
    void genSub(Type t, Register r1, Register r2, Register ret) {}
    void genDiv(Type t, Register r1, Register r2, Register ret) {}
    void genMod(Type t, Register r1, Register r2, Register ret) {}
    void genReg(Type t, Register r, Register ret) {}

    void genAlloca(Type t, Register r, Register ret) {}
    void genStore(Type t, Register r1, Register r2) {}
    void genLoad(Type t, Register r, Register ret) {}
    void genLoad(Type t, std::string glob, Register ret) {}
    void genSpillLoad(Type t, Register r, Register ret) {}
    void genSpillStore(Type t, Register r, Register ret) {}
    void genFunctionCall(Type t, std::string function, Register ret, std::vector<Register> args) {}
    void genReturn(Type t, Register ret) {}
    void genJmpCond(Type t, int op, Register r1, Register r2, std::string label) {}
    void genLabel(std::string label) {}
    void genJmp(std::string label) {}
    void genCmp(Type t, int op, Register r1, Register r2, Register ret) {}

public:
    void genFunction(Type t, Function f) {}
    void genGlobalVariable(std::string name, ArrayType t, std::vector<LARGEINT> init) {}
    void genSetupFile(std::string file) {}
    void genExternSymbol(std::string sym) {}

    int registerAmount() { return REGAMOUNT24; }
    bool shouldAllocateRegisters() { return true; }

    int assemble(std::string infile, std::string outfile, std::string assembler) { return 0; }
    int link(std::string infile, std::string outfile, std::string linker) { return 0; }

public:
    GeneratorZ80(std::string outfile) : Generator(outfile) {}
};