#include <arch/jvm/generator.h>

std::string typeToJVM(Type t)
{
    switch (t.byteSize())
    {
    case 1: return "C";
    case 2: return "S";
    case 4: return "I";
    case 8: return "L";
    }
}
void GeneratorJVM::genIntlitLoad(Type t, int val, Register ret)
{
    write("\tbipush " + std::to_string(val));
    write("\tistore " + std::to_string(ret.virt()));
}
void GeneratorJVM::genGlobLoad(Type t, std::string glob, Register ret) {}
void GeneratorJVM::genMul(Type t, Register r1, Register r2, Register ret)
{
    writeInst("iload", r1.virt());
    writeInst("iload", r2.virt());
    writeInst("imul");
    writeInst("istore", ret.virt());
}

void GeneratorJVM::genAdd(Type t, Register r1, Register r2, Register ret)
{
    writeInst("iload", r1.virt());
    writeInst("iload", r2.virt());
    writeInst("iadd");
    writeInst("istore", ret.virt());
}

void GeneratorJVM::genSub(Type t, Register r1, Register r2, Register ret)
{
    writeInst("iload", r1.virt());
    writeInst("iload", r2.virt());
    writeInst("isub");
    writeInst("istore", ret.virt());
}
void GeneratorJVM::genDiv(Type t, Register r1, Register r2, Register ret)
{
    writeInst("iload", r1.virt());
    writeInst("iload", r2.virt());
    writeInst("idiv");
    writeInst("istore", ret.virt());
}
void GeneratorJVM::genMod(Type t, Register r1, Register r2, Register ret)
{
    writeInst("iload", r1.virt());
    writeInst("iload", r2.virt());
    writeInst("irem");
    writeInst("istore", ret.virt());
}

void GeneratorJVM::genReg(Type t, Register r, Register ret) {}

void GeneratorJVM::genAlloca(Type t, Register r, Register ret) {}
void GeneratorJVM::genStore(Type t, Register r1, Register r2) {}
void GeneratorJVM::genLoad(Type t, Register r, Register ret) {}
void GeneratorJVM::genLoad(Type t, std::string glob, Register ret) {}
void GeneratorJVM::genSpillLoad(Type t, Register r, Register ret) {}
void GeneratorJVM::genSpillStore(Type t, Register r, Register ret) {}
void GeneratorJVM::genFunctionCall(Type t, std::string function, Register ret, std::vector<Register> args)
{
    writeInst("invokestatic", "Method " + m_className + " _" + function + " ()I");
    writeInst("istore", ret.virt());
}
void GeneratorJVM::genReturn(Type t, Register ret)
{
    writeInst("iload", ret.virt());
    writeInst("ireturn");
    write(".end method");
}
void GeneratorJVM::genJmpCond(Type t, int op, Register r1, Register r2, std::string label) {}
void GeneratorJVM::genLabel(std::string label) {}
void GeneratorJVM::genJmp(std::string label) {}
void GeneratorJVM::genCmp(Type t, int op, Register r1, Register r2, Register ret) {}
void GeneratorJVM::genFunction(Type t, Function f) 
{
    std::string method = ".method public static _" + f.name() + " : (";
    for (Type t : f.args())
        method += typeToJVM(t);
    
    method += + ")" + typeToJVM(f.retType());

    write(method);
    write("\t.limit stack 25");
    write("\t.limit locals 25");
}
void GeneratorJVM::genGlobalVariable(std::string name, ArrayType t, std::vector<LARGEINT> init) {}

void GeneratorJVM::genSetupFile(std::string filename)
{
    write(".class public " + m_className);
    write(".super java/lang/Object");
    
    write(".method public static main : ([Ljava/lang/String;)V");
    write(".limit locals 1");
    write(".limit stack 2");
    write("\tinvokestatic Method " + m_className + " _main ()I");
    write("\tinvokestatic Method java/lang/System exit (I)V");
    write("\treturn");
    write(".end method");
}

void GeneratorJVM::genExternSymbol(std::string sym) {}

void GeneratorJVM::writeInst(std::string inst, std::string op)
{
    write("\t" + inst + "\t" + op);
}

void GeneratorJVM::writeInst(std::string inst, int val)
{
    write("\t" + inst + "\t" + std::to_string(val));
}

void GeneratorJVM::writeInst(std::string inst)
{
    write("\t" + inst);
}