#pragma once

#include <generator.h>

#define DATASECTION 1

enum
{
    EAX,
    EBX,
    ECX,
    EDX,
    // ESI,
    // EDI,

    REGAMOUNT
};

/// IDk if this is really necessary
enum
{
    E,
    NE,
    LE,
    L,
    GE,
    G,

    CMPOPAMOUNT
};

class GeneratorX86 : public Generator
{
private:
    std::string m_loByteRegs[REGAMOUNT] = {"al", "bl", "cl", "dl"};
    std::string m_registers[REGAMOUNT] = {"eax", "ebx", "ecx", "edx" /*, "esi", "edi" */};
    std::string m_cmpOps[CMPOPAMOUNT] = {"e", "ne", "le", "l", "ge", "g"};
    int m_textSection = DATASECTION + 1;

private:
    void writeInst(std::string inst);
    void writeInst(std::string inst, std::string arg);
    void writeInst(std::string inst, std::string dest, std::string src);
    void writeMov(std::string dest, std::string src);

private:
    bool physInUse(int reg);
    void genIDiv(Type t, Register r1, Register r2, Register ret, std::string out);

protected:
    void genIntlitLoad(Type t, int val, Register ret);
    void genGlobLoad(Type t, std::string glob, Register ret);
    void genMul(Type t, Register r1, Register r2, Register ret);
    void genAdd(Type t, Register r1, Register r2, Register ret);
    void genSub(Type t, Register r1, Register r2, Register ret);
    void genDiv(Type t, Register r1, Register r2, Register ret);
    void genMod(Type t, Register r1, Register r2, Register ret);
    void genReg(Type t, Register r1, Register ret);

    void genAlloca(Type t, Register r, Register ret);
    void genStore(Type t, Register r1, Register r2);
    void genLoad(Type t, Register r, Register ret);
    void genLoad(Type t, std::string glob, Register ret);

    void genFunctionCall(Type t, std::string function, Register ret, std::vector<Register> args);
    void genSpillLoad(Type t, Register r, Register ret);
    void genSpillStore(Type t, Register r, Register ret);
    void genReturn(Type t, Register ret);
    void genJmpCond(Type t, int op, Register r1, Register r2, std::string label);
    void genLabel(std::string label);
    void genJmp(std::string label);
    void genCmp(Type t, int op, Register r1, Register r2, Register ret);
    std::string registerToString(Register r, std::string *list = nullptr);

public:
    void genSetupFile(std::string file);
    void genFunction(Type t, Function func);
    void genGlobalVariable(std::string name, ArrayType t, std::vector<LARGEINT> init);
    void genExternSymbol(std::string sym);

    int registerAmount() { return REGAMOUNT; }

    /// This function is currently only used to subtract from esp in the beginning
    /// of code generation.
    void feedGenerate(OpList list);

public:
    GeneratorX86(std::string outfile) : Generator(outfile) {}
};