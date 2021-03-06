#pragma once

#include <core.h>
#include <type.h>
#include <oplist.h>
#include <function.h>

/// The base generator class will need to be inherited by your architecture
/// dependant generator class. You will need to implement some functionality
/// like for example some gen* functions.
class Generator
{
protected:
    std::vector<std::string> m_internalOutBuf;

    RegisterList m_regData;
    int m_opLine;

protected:
    /// @todo: all of these functions should return nothing? and also arguments are useless now ay?
    /// All of these next functions MUST be overriden in the architecture
    /// dependant generator class
    virtual void genIntlitLoad(Type t, int val, Register ret) {}
    virtual void genGlobLoad(Type t, std::string glob, Register ret) {}
    virtual void genMul(Type t, Register r1, Register r2, Register ret) {}
    virtual void genAdd(Type t, Register r1, Register r2, Register ret) {}
    virtual void genSub(Type t, Register r1, Register r2, Register ret) {}
    virtual void genDiv(Type t, Register r1, Register r2, Register ret) {}
    virtual void genMod(Type t, Register r1, Register r2, Register ret) {}
    virtual void genReg(Type t, Register r, Register ret) {}

    virtual void genAlloca(Type t, Register r, Register ret) {}
    virtual void genStore(Type t, Register r1, Register r2) {}
    virtual void genStore(Type t, std::string glob, Register r2) {}
    virtual void genLoad(Type t, Register r, Register ret) {}
    virtual void genLoad(Type t, std::string glob, Register ret) {}
    virtual void genSpillLoad(Type t, Register r, Register ret) {}
    virtual void genSpillStore(Type t, Register r, Register ret) {}
    virtual void genFunctionCall(Type t, std::string function, Register ret, std::vector<Register> args) {}
    virtual void genReturn(Type t, Register ret) {}
    virtual void genJmpCond(Type t, int op, Register r1, Register r2, std::string label) {}
    virtual void genLabel(std::string label) {}
    virtual void genJmp(std::string label) {}
    virtual void genCmp(Type t, int op, Register r1, Register r2, Register ret) {}

public:
    virtual void genFunction(Function f) {}
    virtual void genGlobalVariable(std::string name, ArrayType t, std::vector<LARGEINT> init) {}
    virtual void genSetupFile(std::string file) {}
    virtual void genExternSymbol(std::string sym) {}

    /// These functions are used to control carbon's register allocation system.
    /// If you want carbon to allocate the registers for you, you must return
    /// the amount of physical registers that you have available in the registerAmount()
    /// function. If not, just return false from the shouldAllocateRegisters() function.
    virtual int registerAmount() { return 0; }
    virtual bool shouldAllocateRegisters() { return true; }

    /// These two functions will need to implement the assembling and the linking
    /// of the binary. If the assembler string is empty use the default assembler for
    /// your platform, if it is not that means that an external assembler was specified
    /// by the user. The same goes for the linker.
    virtual int assemble(std::string infile, std::string outfile, std::string assembler) { return 0; }
    virtual int link(std::string infile, std::string outfile, std::string linker) { return 0; }

public:
    /// This is the base generate function, this can be overriden in the architecture
    /// dependant generator class but it is not required. Basically unless you need
    /// too (because the generator does not fullfill the needs of your architecture)
    /// you can just use the one we provide.
    virtual void generate(OpQuad *tree);

    /// This is the base feed function, it does no optimization. Meaning it just
    /// passes every quadtriple operation in the OpList to the generate class.
    /// You can override this class and implement some form of final architecture
    /// optimization.
    virtual void feedGenerate(OpList list);

public:
    Generator() {}

    void setRegList(RegisterList list);
    int writeOutfile(std::string outfile);
    Register &findReg(int r);

public:
    /// Helpers
    void write(std::string);
    void insert(std::string, int line);
};