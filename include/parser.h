#pragma once

#include <core.h>
#include <scanner.h>
#include <generator.h>
#include <optimizer/optimizer.h>
#include <type.h>
#include <oplist.h>
#include <function.h>

class Parser
{
private:
    Scanner m_scanner;
    Generator *m_generator;
    Optimizer *m_optimizer;

    RegisterList m_regList;
    std::vector<Function> m_functions;

private:
    int parsePrimary(Type t, bool shouldBeReg = false);
    OpQuad *parseBinOperator();
    OpQuad *parseOperation();
    OpQuad *parseInitialize();
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
    ArrayType parseArrayType();
    std::vector<LARGEINT> parseArrayInit(int amount);

    int addRegister(int r, Type t);

public:
    Parser(Scanner &scan, Generator *gen, Optimizer *opt);
    void parse();
};