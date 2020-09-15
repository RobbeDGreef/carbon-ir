#include <arch/i386/optimizer.h>
#include <debug.h>

bool OptimizerX86::graphCheck(RegisterGraph &graph)
{
    /// @todo: refactor
    bool flag = false;

    for (int i = 0; i < (int)graph.statements().regList().size(); i++)
    {
        Register r = graph.statements().regList()[i];
        if (r.spilled())
        {
            OpList tmp = graph.statements();
            int tosearch = i;
            int line = 0;
            for (OpQuad *quad : tmp)
            {
                if (quad->operation() != OpQuad::Types::INTLIT && quad->operation() != OpQuad::Types::SPILLSTORE && quad->operation() != OpQuad::Types::SPILLLOAD)
                {
                    if (quad->arg1() == tosearch)
                    {
                        flag = true;
                        int last = graph.statements().regList().addRegister(graph.statements().regList().lastReg() + 1, quad->type());
                        graph.statements().insert(line, new OpQuad(OpQuad::Types::SPILLLOAD, tosearch, -1, last, quad->type()));
                        graph.statements().regList()[last].setLastOcc(line + 1);
                        quad->setArg1(last);
                        line++;
                    }

                    if (quad->arg2() == tosearch)
                    {
                        flag = true;
                        int last = graph.statements().regList().addRegister(graph.statements().regList().lastReg() + 1, quad->type());
                        graph.statements().insert(line, new OpQuad(OpQuad::Types::SPILLLOAD, tosearch, -1, last, quad->type()));
                        graph.statements().regList()[last].setLastOcc(line + 1);
                        quad->setArg2(last);
                        line++;
                    }

                    if (quad->ret() == tosearch && tmp[line + 1]->operation() != OpQuad::Types::SPILLSTORE)
                    {
                        flag = true;
                        int last = graph.statements().regList().addRegister(graph.statements().regList().lastReg() + 1, quad->type());
                        graph.statements().insert(line + 1, new OpQuad(OpQuad::Types::SPILLSTORE, last, -1, tosearch, quad->type()));
                        graph.statements().regList()[last].setFirstOcc(line);
                        quad->setReturn(last);
                        line++;
                    }
                }
                line++;
            }
        }
    }
    return flag;
}

void OptimizerX86::prepAssignRegisters(OpList &list)
{
    /// We don't need to do anything here for X86 (yet)
}