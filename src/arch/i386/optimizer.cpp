#include <arch/i386/optimizer.h>
#include <debug.h>

bool OptimizerX86::graphCheck(RegisterGraph &graph)
{
    /// @todo: refactor
    bool flag = false;

    for (int i = 0; i < (int) graph.statements().regList().size(); i++)
    {
        Register r = graph.statements().regList()[i];
        if (r.spilled())
        {
            OpList tmp = graph.statements();
            int tosearch = i;
            int line = 0;
            int prevOp = -1;
            for (OpQuad *quad : tmp)
            {
                dbg_print("op: " << quad->operation());
                if (quad->operation() != OpQuad::Types::INTLIT && prevOp != OpQuad::Types::SPILLLOAD && quad->operation() != OpQuad::Types::SPILLSTORE && quad->operation() != OpQuad::Types::SPILLLOAD)
                {
                    if (quad->arg1() == tosearch)
                    {
                        flag = true;
                        int last = graph.statements().regList().addRegister(graph.statements().regList().lastReg() + 1);
                        graph.statements().insert(line, new OpQuad(OpQuad::Types::SPILLLOAD, tosearch, -1, last, quad->type()));
                        graph.statements().regList()[last].setLastOcc(line + 1);
                        quad->setArg1(last);
                        line++;
                    }

                    if (quad->arg2() == tosearch)
                    {
                        flag = true;
                        int last = graph.statements().regList().addRegister(graph.statements().regList().lastReg() + 1);
                        graph.statements().insert(line, new OpQuad(OpQuad::Types::SPILLLOAD, tosearch, -1, last, quad->type()));
                        graph.statements().regList()[last].setLastOcc(line + 1);
                        dbg_print("reg: " << graph.statements().regList()[last].virt() << " first: " << graph.statements().regList()[last].firstOcc() << " last: " << graph.statements().regList()[last].lastOcc());
                        quad->setArg2(last);
                        line++;
                    }

                    if (quad->ret() == tosearch && tmp[line + 1]->operation() != OpQuad::Types::SPILLSTORE)
                    {
                        flag = true;
                        int last = graph.statements().regList().addRegister(graph.statements().regList().lastReg() + 1);
                        graph.statements().insert(line + 1, new OpQuad(OpQuad::Types::SPILLSTORE, last, -1, tosearch, quad->type()));
                        graph.statements().regList()[last].setFirstOcc(line);
                        quad->setReturn(last);
                        line++;
                    }
                }
                line++;
                prevOp = quad->operation();
            }
        }
    }
    return flag;
}

void OptimizerX86::prepAssignRegisters(OpList &list)
{
    /// Ok so I had a small realization in the shower (yes I think abt programming in the shower)
    /// why would I want to do this for x86, i know we need to load in registers
    /// because, well, otherwise we trash the first register. BUT we can just in the
    /// generator load in the first reg to the ret and preform the operation on that
    /// register. Damn sometimes I overthink shit (i mean the code below)
#if 0
    RegisterList &regList = list.regList();
    int lastReg = regList.lastReg() + 1;

    OpList tmp = list;
    int i = 0;
    for (OpQuad *quad : tmp)
    {   
        switch (quad->operation())
        {
        case OpQuad::Types::INTLIT:
            break;
        
        default:
            /// @todo: refactor this because wow that's some amazing code reuse
            if (quad->arg1() != -1)
            {
                int r = regList.addRegister(lastReg);
                lastReg++;
                list.insert(list.begin() + i, new OpQuad(OpQuad::Types::REG, quad->arg1(), -1, r, quad->type()));
                i++;
                quad->setArg1(r);
            }

            if (quad->arg2() != -1)
            {
                int r = regList.addRegister(lastReg);
                lastReg++;
                list.insert(list.begin() + i, new OpQuad(OpQuad::Types::REG, quad->arg2(), -1, r, quad->type()));
                i++;
                quad->setArg2(r);
            }
        }
        i++;
    }
#endif
}