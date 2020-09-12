#pragma once

#include <core.h>
#include <scanner.h>
#include <generator.h>
#include <optimizer/optimizer.h>
#include <type.h>
#include <oplist.h>

class Parser
{
private:
    Scanner &m_scanner;
    Generator &m_generator;
    Optimizer &m_optimizer;

    RegisterList m_regList;

private:
    int parsePrimary(Type t, bool shouldBeReg = false);
    OpQuad *parseBinOperator();
    OpQuad *parseOperation();
    OpQuad *parseIntlit();
    OpQuad *parseAssign();
    OpQuad *parseStatement();
    OpQuad *parseReturn();
    OpQuad *parseFunctionCall();
    OpQuad *parseStore();
    OpQuad *parseLoad();
    OpQuad *parseAlloca();
    OpQuad *parseJmpCond();
    OpQuad *parseLabel();
    OpQuad *parseJmp();
    OpQuad *parseCmp();
    OpList parseFunction();
    Type parseType();

    int addRegister(int r);

public:
    Parser(Scanner &scan, Generator &gen, Optimizer &opt);
    void parse();
};