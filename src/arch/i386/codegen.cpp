#include <arch/i386/generator.h>
#include <debug.h>

void GeneratorX86::genIntlitLoad(Type t, int val, Register ret)
{
    writeMov(registerToString(ret), std::to_string(val));
}

void GeneratorX86::genMul(Type t, Register r1, Register r2, Register ret)
{
    writeMov(registerToString(ret), registerToString(r1));
    writeInst("imul", registerToString(ret), registerToString(r2));
}

void GeneratorX86::genAdd(Type t, Register r1, Register r2, Register ret)
{
    writeMov(registerToString(ret), registerToString(r1));
    writeInst("add", registerToString(ret), registerToString(r2));
}

void GeneratorX86::genSub(Type t, Register r1, Register r2, Register ret)
{
    writeMov(registerToString(ret), registerToString(r1));
    writeInst("sub", registerToString(ret), registerToString(r2));
}

void GeneratorX86::genIDiv(Type t, Register r1, Register r2, Register ret, std::string out)
{
    if (physInUse(EDX) && ret.hintReg() != EDX)
        writeInst("push", "edx");

    if (physInUse(EAX) && ret.hintReg() != EAX)
        writeInst("push", "eax");

    std::string divisor = registerToString(r2);
    int tmp = (r1.hintReg() == ECX) ? EBX : ECX;
    if (r2.hintReg() == EAX || r2.hintReg() == EDX)
    {
        /// load it to tmp
        if (physInUse(tmp) && ret.hintReg() != tmp)
            writeInst("push", m_registers[tmp]);

        writeMov(m_registers[tmp], registerToString(r2));
        divisor = m_registers[tmp];
    }

    writeMov("eax", registerToString(r1));
    writeInst("xor", "edx", "edx");
    writeInst("idiv", divisor);
    writeMov(registerToString(ret), out);

    if (physInUse(tmp) && ret.hintReg() != tmp)
        writeInst("pop", m_registers[tmp]);

    if (physInUse(EAX) && ret.hintReg() != EAX)
        writeInst("pop", "eax");

    if (physInUse(EDX) && ret.hintReg() != EDX)
        writeInst("pop", "edx");
}

void GeneratorX86::genDiv(Type t, Register r1, Register r2, Register ret)
{
    genIDiv(t, r1, r2, ret, "eax");
}

void GeneratorX86::genMod(Type t, Register r1, Register r2, Register ret)
{
    genIDiv(t, r1, r2, ret, "edx");
}

void GeneratorX86::genFunction(Type t, std::string name)
{
    write("[global " + name + "]");
    write(name + ":");
    writeInst("push", "ebp");
    writeInst("mov", "ebp", "esp");
}

void GeneratorX86::genReturn(Type t, Register ret)
{
    writeMov("eax", registerToString(ret));
    writeInst("leave");
    writeInst("ret");
}

void GeneratorX86::genReg(Type t, Register r, Register ret)
{
    writeMov(registerToString(ret), registerToString(r));
}

void GeneratorX86::genSpillLoad(Type t, Register r, Register ret)
{
    dbg_print("gened spilload: " << r.hintSpill() << ret.hintReg());
    writeMov(registerToString(ret), registerToString(r));
}

void GeneratorX86::genSpillStore(Type t, Register r, Register ret)
{
    dbg_print("gened spilstore: " << r.hintReg() << ret.hintSpill());
    writeMov(registerToString(ret), registerToString(r));
}

static bool isArg(std::vector<Register> args, int i)
{
    for (Register r : args)
        if (r.hintReg() == i)
            return true;
    
    return false;
}

void GeneratorX86::genFunctionCall(Type t, std::string id, Register ret, std::vector<Register> args)
{
    /// @todo: spill all live registers

    // Save all live registers
    bool regInUse[REGAMOUNT];
    for (int i = 0; i < REGAMOUNT; i++)
    {
        if (physInUse(i) && i != ret.hintReg() && !isArg(args, i))
        {
            regInUse[i] = true;
            writeInst("push", m_registers[i]);
        }
        else
            regInUse[i] = false;
    }

    for (Register r : args)
    {
        writeInst("push", registerToString(r));
    }

    writeInst("call", id);
    writeInst("add", "esp", std::to_string(4 * args.size()));
    writeMov(registerToString(ret), "eax");

    // Pop all previously saved registers
    for (int i = 0; i < REGAMOUNT; i++)
        if (regInUse[i])
            writeInst("pop", m_registers[i]);
}

void GeneratorX86::genAlloca(Type t, Register r, Register ret)
{
    writeInst("sub", "esp", registerToString(r));
    writeInst("mov", registerToString(ret), "esp");
}

void GeneratorX86::genStore(Type t, Register r1, Register r2)
{
    writeInst("mov", "[" + registerToString(r1) + "]", registerToString(r2));
}

void GeneratorX86::genLoad(Type t, Register r, Register ret)
{
    writeInst("mov", registerToString(ret), "[" + registerToString(r) + "]");
}

void GeneratorX86::genLoad(Type t, std::string glob, Register ret)
{
    writeInst("mov", registerToString(ret), "[" + glob + "]");
}

void GeneratorX86::genJmpCond(Type t, int op, Register r1, Register r2, std::string label)
{
    writeInst("cmp", registerToString(r1), registerToString(r2));
    dbg_assert(op >= 0 && op <= CMPOPAMOUNT);
    writeInst("j" + m_cmpOps[op], label);
}

void GeneratorX86::genLabel(std::string label)
{
    write(label + ":");
}

void GeneratorX86::genJmp(std::string label)
{
    writeInst("jmp", label);
}

void GeneratorX86::genCmp(Type t, int op, Register r1, Register r2, Register ret)
{
    writeInst("xor", registerToString(ret), registerToString(ret));
    writeInst("cmp", registerToString(r1), registerToString(r2));
    dbg_assert(op >= 0 && op <= CMPOPAMOUNT);
    writeInst("set" + m_cmpOps[op], registerToString(ret, m_loByteRegs));
}

static std::string byteSizeToNasmVar(int byteSize)
{
    switch (byteSize)
    {
    case 1: return "db";
    case 2: return "dw";
    case 4: return "dd";
    case 8: return "dq";
    default:
        dbg_assert(0);
        return "db";
    }
}

void GeneratorX86::genGlobalVariable(std::string name, ArrayType t, std::vector<LARGEINT> init)
{
    std::string text = name + ":\n\t";
    text += byteSizeToNasmVar(t.type().byteSize());

    for (LARGEINT i : init)
    {
        text += " " + std::to_string(i);
    }
    
    text += '\n';

    insert(text, DATASECTION);
}

void GeneratorX86::genSetupFile()
{
    write("section .data");
    write("section .text");
    write("[extern printf]");
}

void GeneratorX86::genGlobLoad(Type t, std::string glob, Register ret)
{
    writeMov(registerToString(ret), glob);
}