#include <arch/jvm/generator.h>

std::string typeToJVM(Type t)
{
    return "I";

    switch (t.byteSize())
    {
    case 1: return "B";
    case 2: return "I";
    }
}
void GeneratorJVM::genIntlitLoad(Type t, int val, Register ret)
{
    write("\tbipush " + std::to_string(val));
    write("\tistore_" + std::to_string(ret.virt()));
}
void GeneratorJVM::genGlobLoad(Type t, std::string glob, Register ret) {}
void GeneratorJVM::genMul(Type t, Register r1, Register r2, Register ret)
{
    write("\tiload_" + std::to_string(r1.virt()));
    write("\tiload_" + std::to_string(r2.virt()));
    write("\timul");
    write("\tistore_" + std::to_string(ret.virt()));
}

void GeneratorJVM::genAdd(Type t, Register r1, Register r2, Register ret) {}
void GeneratorJVM::genSub(Type t, Register r1, Register r2, Register ret) {}
void GeneratorJVM::genDiv(Type t, Register r1, Register r2, Register ret) {}
void GeneratorJVM::genMod(Type t, Register r1, Register r2, Register ret) {}
void GeneratorJVM::genReg(Type t, Register r, Register ret) {}

void GeneratorJVM::genAlloca(Type t, Register r, Register ret) {}
void GeneratorJVM::genStore(Type t, Register r1, Register r2) {}
void GeneratorJVM::genLoad(Type t, Register r, Register ret) {}
void GeneratorJVM::genLoad(Type t, std::string glob, Register ret) {}
void GeneratorJVM::genSpillLoad(Type t, Register r, Register ret) {}
void GeneratorJVM::genSpillStore(Type t, Register r, Register ret) {}
void GeneratorJVM::genFunctionCall(Type t, std::string function, Register ret, std::vector<Register> args) {}
void GeneratorJVM::genReturn(Type t, Register ret)
{
    write("\tiload_" + std::to_string(ret.virt()));
    write("\tireturn");
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
    write("\t.limit stack 10");
    write("\t.limit locals 10");
}
void GeneratorJVM::genGlobalVariable(std::string name, ArrayType t, std::vector<LARGEINT> init) {}

void GeneratorJVM::genSetupFile(std::string filename)
{
    write(".class public carbon");
    write(".super java/lang/Object");
    
    write(".method public static main : ([Ljava/lang/String;)V");
    write(".limit locals 1");
    write(".limit stack 2");
    write("\tinvokestatic Method carbon _main ()I");
    write("\tinvokestatic Method java/lang/System exit (I)V");
    write("\treturn");
    write(".end method");
}

void GeneratorJVM::genExternSymbol(std::string sym) {}
