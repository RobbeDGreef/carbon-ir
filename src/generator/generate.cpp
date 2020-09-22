#include <generator.h>
#include <debug.h>

void Generator::generate(OpQuad *op)
{
    switch (op->operation())
    {
    case OpTypes::INTLIT:
        genIntlitLoad(op->type(), op->arg1(), findReg(op->ret()));
        break;

    case OpTypes::GLOB:
        genGlobLoad(op->type(), op->identifier(), findReg(op->ret()));
        break;

    case OpTypes::MUL:
        genMul(op->type(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;
    case OpTypes::ADD:
        genAdd(op->type(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;
    case OpTypes::SUB:
        genSub(op->type(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;
    case OpTypes::DIV:
        genDiv(op->type(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;
    case OpTypes::MOD:
        genMod(op->type(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;

    case OpTypes::REG:
        genReg(op->type(), findReg(op->arg1()), findReg(op->ret()));
        break;

    case OpTypes::CALL:
    {
        std::vector<Register> args;
        for (int arg : op->extraArgs())
            args.push_back(findReg(arg));
        genFunctionCall(op->type(), op->identifier(), findReg(op->ret()), args);
    }
    break;

    case OpTypes::SPILLLOAD:
        genSpillLoad(op->type(), findReg(op->arg1()), findReg(op->ret()));
        break;

    case OpTypes::SPILLSTORE:
        genSpillStore(op->type(), findReg(op->arg1()), findReg(op->ret()));
        break;

    case OpTypes::ALLOCA:
        genAlloca(op->type(), findReg(op->arg1()), findReg(op->ret()));
        break;

    case OpTypes::STORE:
        if (op->arg1() == -1)
            genStore(op->type(), op->identifier(), findReg(op->arg2()));
        else
            genStore(op->type(), findReg(op->arg1()), findReg(op->arg2()));
        break;

    case OpTypes::LOAD:
        if (op->arg1() == -1)
            genLoad(op->type(), op->identifier(), findReg(op->ret()));
        else
            genLoad(op->type(), findReg(op->arg1()), findReg(op->ret()));
        break;

    case OpTypes::JMPCOND:
        genJmpCond(op->type(), op->extra(), findReg(op->arg1()), findReg(op->arg2()), op->identifier());
        break;

    case OpTypes::LABEL:
        genLabel(op->identifier());
        break;

    case OpTypes::JMP:
        genJmp(op->identifier());
        break;

    case OpTypes::CMP:
        genCmp(op->type(), op->extra(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;

    case OpTypes::RETURN:
        genReturn(op->type(), findReg(op->arg1()));
        break;

    default:
        dbg_print_r("Unknown " << op->operation());
        break;
    }
}