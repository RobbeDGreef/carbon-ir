#include <generator.h>
#include <debug.h>

void Generator::generate(OpQuad *op)
{
    dbg_print("operation: " << op->operation());
    switch (op->operation())
    {
    case OpQuad::Types::INTLIT:
        genIntlitLoad(op->type(), op->arg1(), findReg(op->ret()));
        break;

    case OpQuad::Types::MUL:
        genMul(op->type(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;
    case OpQuad::Types::ADD:
        genAdd(op->type(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;
    case OpQuad::Types::SUB:
        genSub(op->type(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;
    case OpQuad::Types::DIV:
        genDiv(op->type(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;
    case OpQuad::Types::MOD:
        genMod(op->type(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;

    case OpQuad::Types::REG:
        genReg(op->type(), findReg(op->arg1()), findReg(op->ret()));
        break;

    case OpQuad::Types::CALL:
        genFunctionCall(op->type(), op->identifier(), findReg(op->ret()));
        break;

    case OpQuad::Types::SPILLLOAD:
        genSpillLoad(op->type(), findReg(op->arg1()), findReg(op->ret()));
        break;

    case OpQuad::Types::SPILLSTORE:
        genSpillStore(op->type(), findReg(op->arg1()), findReg(op->ret()));
        break;
    
    case OpQuad::Types::ALLOCA:
        genAlloca(op->type(), findReg(op->arg1()), findReg(op->ret()));
        break;
    
    case OpQuad::Types::STORE:
        genStore(op->type(), findReg(op->arg1()), findReg(op->arg2()));
        break;
    
    case OpQuad::Types::LOAD:
        dbg_print("arg1: " << op->arg1() << " ret: " << op->ret());
        genLoad(op->type(), findReg(op->arg1()), findReg(op->ret()));
        break;
    
    case OpQuad::Types::JMPCOND:
        genJmpCond(op->type(), op->extra(), findReg(op->arg1()), findReg(op->arg2()), op->identifier());
        break;
    
    case OpQuad::Types::LABEL:
        genLabel(op->identifier());
        break;

    case OpQuad::Types::JMP:
        genJmp(op->identifier());
        break;
    
    case OpQuad::Types::CMP:
        genCmp(op->type(), op->extra(), findReg(op->arg1()), findReg(op->arg2()), findReg(op->ret()));
        break;
        
    case OpQuad::Types::RETURN:
        genReturn(op->type(), findReg(op->arg1()));
        break;

    default:
        dbg_print_r("Unknown " << op->operation());
        break;
    }

}