#include <parser.h>
#include <attributes.h>

Parser::Parser(Scanner &scan, Generator *gen, Optimizer *opt, std::string asmfile)
    : m_scanner(scan)
{
    m_generator = gen;
    m_optimizer = opt;
    m_asmFile = asmfile;
}

Parser::Parser(Generator *gen, Optimizer *opt)
{
    m_generator = gen;
    m_optimizer = opt;
}

Type Parser::parseType()
{
    Type t;
    switch (m_scanner.token().token())
    {
    case Token::Types::I8:
        t = Type(1);
        break;
    case Token::Types::I16:
        t = Type(2);
        break;
    case Token::Types::I32:
        t = Type(4);
        break;
    case Token::Types::I64:
        t = Type(8);
        break;
    default:
        g_errsys.syntaxError("unknown type");
    }

    while (m_scanner.scan().token() == Token::Types::STAR)
        t.setPtr(t.ptr() + 1);

    return t;
}

ArrayType Parser::parseArrayType()
{
    m_scanner.match(Token::Types::LBRACKET);
    Type t = parseType();
    int amount = m_scanner.token().intValue();
    m_scanner.match(Token::Types::INTLIT);
    m_scanner.match(Token::Types::RBRACKET);
    return ArrayType(t, amount);
}

/// @todo: this function is pretty useless you can just use a m_scanner.match() call
int Parser::parsePrimary(Type t, bool shouldBeReg)
{
    Token &tok = m_scanner.token();

    if (shouldBeReg)
        dbg_assert(tok.token() == Token::Types::REG);

    return tok.intValue();
}

int Parser::addRegister(int reg, Type t)
{
    /// @todo: this function should probably be removed
    return m_regList.addRegister(reg, t);
}

OpQuad *Parser::parseBinOperator()
{
    int tok = m_scanner.token().token();
    m_scanner.scan();

    Type type = parseType();
    OpQuad *quad = new OpQuad(OpQuad::tokToOp(tok), type);

    /// @todo check if the left and right leaf even support the given type, otherwise error
    quad->setArg1(addRegister(parsePrimary(type), type));

    m_scanner.scan();
    quad->setArg2(addRegister(parsePrimary(type), type));

    m_scanner.scan();
    return quad;
}

OpQuad *Parser::parseAssign()
{
    int reg = m_scanner.token().intValue();
    m_scanner.scan();
    m_scanner.match(Token::Types::EQUALSIGN);

    OpQuad *quad = parseOperation();

    if (!quad)
        g_errsys.syntaxError("unexpected operation after register assign");

    quad->setReturn(addRegister(reg, quad->type()));

    return quad;
}

OpQuad *Parser::parseInitialize()
{
    Type t = parseType();

    OpQuad *quad;
    if (m_scanner.token().token() == Token::Types::INTLIT)
        quad = new OpQuad(OpQuad::Types::INTLIT, m_scanner.token().intValue(), t);

    else if (m_scanner.token().token() == Token::Types::GLOB)
    {
        quad = new OpQuad(OpQuad::Types::GLOB, t);
        quad->setIdentifier(m_scanner.token().identifier());
    }
    else
        g_errsys.syntaxError("expected a integer literal or a global variable identifier");

    m_scanner.scan();
    return quad;
}

OpQuad *Parser::parseFunctionCall()
{
    m_scanner.scan();
    Type t = parseType();

    OpQuad *quad = new OpQuad(OpQuad::Types::CALL, t);

    std::string id = m_scanner.match(Token::Types::IDENTIFIER).identifier();
    quad->setIdentifier(id);

    m_scanner.match(Token::Types::LPAREN);

    std::vector<int> arguments;
    /// @todo: if there is no rparen it will deadlock so fix that (maybe check for newline or something)
    /// @todo: Check if the arguments are correct with the function called
    while (m_scanner.token().token() != Token::Types::RPAREN)
    {
        arguments.push_back(m_scanner.token().intValue());
        m_scanner.scan();
    }
    m_scanner.scan();

    quad->setExtraArgs(arguments);
    return quad;
}

OpQuad *Parser::parseLoad()
{
    m_scanner.scan();
    Type t = parseType();

    OpQuad *quad;
    if (m_scanner.token().token() == Token::Types::REG)
    {
        int r = m_scanner.token().intValue();
        r = addRegister(r, t);
        quad = new OpQuad(OpQuad::Types::LOAD, r, t);
    }
    else if (m_scanner.token().token() == Token::Types::GLOB)
    {
        std::string id = m_scanner.token().identifier();
        quad = new OpQuad(OpQuad::Types::LOAD, t);
        quad->setIdentifier(id);
    }
    else
        g_errsys.syntaxError("Expected either a register or a global variable identifier as an argument to load");

    m_scanner.scan();

    return quad;
}

OpQuad *Parser::parseStore()
{
    m_scanner.scan();
    Type t = parseType();

    OpQuad *quad = new OpQuad(OpQuad::Types::STORE, t);
    if (m_scanner.token().token() == Token::Types::REG)
    {
        int r1 = m_scanner.token().intValue();
        r1 = addRegister(r1, t);
        quad->setArg1(r1);
    }
    else if (m_scanner.token().token() == Token::Types::GLOB)
    {
        quad->setIdentifier(m_scanner.token().identifier());
    }
    else
        g_errsys.syntaxError("expected either a register or a global variable identifier as the first argument to store");

    int r2 = m_scanner.scan().intValue();
    m_scanner.match(Token::Types::REG);
    r2 = addRegister(r2, t);
    quad->setArg2(r2);

    return quad;
}

OpQuad *Parser::parseAlloca()
{
    m_scanner.scan();
    Type t = parseType();
    int r = m_scanner.match(Token::Types::REG).intValue();
    return new OpQuad(OpQuad::Types::ALLOCA, addRegister(r, t), t);
}

OpQuad *Parser::parseJmp()
{
    std::string id = m_scanner.scan().identifier();
    m_scanner.match(Token::Types::IDENTIFIER);
    return new OpQuad(OpQuad::Types::JMP, id);
}

OpQuad *Parser::parseCmp()
{
    int op = m_scanner.scan().token();
    m_scanner.scan();
    Type t = parseType();
    int r1 = addRegister(m_scanner.token().intValue(), t);
    m_scanner.match(Token::Types::REG);
    int r2 = addRegister(m_scanner.token().intValue(), t);
    m_scanner.match(Token::Types::REG);

    OpQuad *quad = new OpQuad(OpQuad::Types::CMP, r1, r2, -1, t);
    quad->setExtra(op - Token::Types::EQ);
    return quad;
}

OpQuad *Parser::parseOperation()
{
    int tok = m_scanner.token().token();
    switch (tok)
    {
    case Token::Types::ADD:
    case Token::Types::SUB:
    case Token::Types::MUL:
    case Token::Types::DIV:
    case Token::Types::MOD:
        return parseBinOperator();

    case Token::Types::I8:
    case Token::Types::I16:
    case Token::Types::I32:
    case Token::Types::I64:
        return parseInitialize();

    case Token::Types::CALL:
        return parseFunctionCall();

    case Token::Types::ALLOCA:
        return parseAlloca();

    case Token::Types::LOAD:
        return parseLoad();

    case Token::Types::CMP:
        return parseCmp();
    }

    return nullptr;
}

OpQuad *Parser::parseJmpCond()
{
    int op = m_scanner.scan().token();
    m_scanner.scan();
    Type t = parseType();
    int r1 = addRegister(m_scanner.token().intValue(), t);
    m_scanner.match(Token::Types::REG);
    int r2 = addRegister(m_scanner.token().intValue(), t);
    m_scanner.match(Token::Types::REG);

    std::string id = m_scanner.token().identifier();
    m_scanner.match(Token::Types::IDENTIFIER);
    /// @todo: check if this is a valid label
    OpQuad *quad = new OpQuad(OpQuad::Types::JMPCOND, r1, r2, -1, t);
    quad->setExtra(op - Token::Types::EQ);
    quad->setIdentifier(id);

    return quad;
}

OpQuad *Parser::parseLabel()
{
    std::string id = m_scanner.token().identifier();
    m_scanner.scan();
    m_scanner.match(Token::Types::COLON);
    return new OpQuad(OpQuad::LABEL, id);
}

OpQuad *Parser::parseReturn()
{
    m_scanner.scan();
    Type type = parseType();
    OpQuad *quad = new OpQuad(OpQuad::Types::RETURN, type);
    quad->setArg1(addRegister(parsePrimary(type, true), type));
    m_scanner.scan();
    return quad;
}

OpQuad *Parser::parseStatement()
{
    int tok = m_scanner.token().token();

    switch (tok)
    {
    case Token::Types::REG:
        return parseAssign();

    case Token::Types::RETURN:
        return parseReturn();

    case Token::Types::STORE:
        return parseStore();

    case Token::Types::JMPCOND:
        return parseJmpCond();

    case Token::Types::JMP:
        return parseJmp();

    case Token::Types::IDENTIFIER:
        return parseLabel();
    }

    return parseOperation();
}

OpList Parser::parseFunction()
{
    OpList statements;

    m_scanner.scanUntil(Token::Types::FUNCTION);
    m_scanner.scan();
    Type t = parseType();
    if (t.byteSize() == -1)
        g_errsys.syntaxError("expected a type specifier");

    std::string fname = m_scanner.token().identifier();
    m_scanner.match(Token::Types::IDENTIFIER);
    m_scanner.match(Token::Types::LPAREN);

    m_functions.push_back(Function(fname, t));

    /// @todo: if there is no rparen it will deadlock so fix that (maybe check for newline or something)
    int spill = -1;
    while (m_scanner.token().token() != Token::Types::RPAREN)
    {
        Type t = parseType();
        m_functions.back().args().push_back(t);
        int reg = statements.regList().addRegister(m_scanner.token().intValue(), t);
        m_scanner.match(OpQuad::Types::REG);
        Register &r = statements.regList()[reg];
        r.setFirstOcc(0);
        r.setSpilled(true);
        r.setHintSpill(spill--);
    }

    /// @todo: this might deadlock when no LBRACE is ever found
    while (m_scanner.scan().token() != Token::Types::LBRACE)
        if (m_scanner.token().token() == Token::Types::ATTRIBUTES)
            m_functions.back().setAttributes(Attributes(m_scanner.token().args()));

    m_scanner.scan();

    while (1)
    {
        OpQuad *tree = parseStatement();
        if (tree != nullptr)
        {
            m_scanner.match(Token::Types::NEWLINE);
            statements.push_newregs(m_regList);
            statements.push_back(tree);
            continue;
        }

        if (m_scanner.token().token() == Token::Types::RBRACE)
            break;

        if (m_scanner.token().token() == EOF)
            g_errsys.fatal("unexpected end of file before }");

        m_scanner.scan();
    }
    m_scanner.scan();

    m_regList.clear();
    m_generator->genFunction(m_functions.back());
    return statements;
}

std::vector<LARGEINT> Parser::parseArrayInit(int amount)
{
    std::vector<LARGEINT> init;

    /// @todo @fixme: this might deadlock when this is the end of the file i guess
    while (m_scanner.token().token() != Token::Types::NEWLINE)
    {
        switch (m_scanner.token().token())
        {
        case Token::Types::INTLIT:
            init.push_back(m_scanner.token().intValue());
            break;
        case Token::Types::STRINGLIT:
            for (char c : m_scanner.token().identifier())
                init.push_back(c);
            break;
        default:
            g_errsys.syntaxError("unexpected token used as array initializer");
        }
        m_scanner.scan();
    }

    return init;
}

void Parser::parseGlobal()
{
    std::string id = m_scanner.token().identifier();
    m_scanner.scan();
    m_scanner.match(Token::Types::EQUALSIGN);
    ArrayType t = parseArrayType();
    std::vector<LARGEINT> init = parseArrayInit(t.arrSize());
    m_generator->genGlobalVariable(id, t, init);
}

    int tok;
    while ((tok = m_scanner.scan().token()) != EOF)
    {
        if (tok == Token::Types::FUNCTION)
        {
            OpList statements = parseFunction();
            statements = m_optimizer->optimize(statements);
            if (m_generator->shouldAllocateRegisters())
                m_optimizer->assignRegisters(statements, m_generator);
            m_generator->setRegList(statements.regList());
            dbg_call(statements.print();)
                m_generator->feedGenerate(statements);
            statements.destroy();
        }
        else if (tok == Token::Types::GLOB)
        {
            std::string id = m_scanner.token().identifier();
            m_scanner.scan();
            m_scanner.match(Token::Types::EQUALSIGN);
            ArrayType t = parseArrayType();
            std::vector<LARGEINT> init = parseArrayInit(t.arrSize());
            m_generator->genGlobalVariable(id, t, init);
        }
        else if (tok == Token::Types::EXTERN)
        {
            /// @todo: actually parse extern functions and store them
            m_scanner.scan();
            m_scanner.match(Token::Types::FUNCTION);
            dbg_print("no");
            parseType();
            dbg_print("yes");
            m_generator->genExternSymbol(m_scanner.token().identifier());
            m_scanner.scanUntil(Token::Types::RPAREN);
            m_scanner.scan();
        }
    }

    m_generator->writeOutfile(m_asmFile);
    dbg_print("end");
}

int Parser::addFunction(Function f)
{
    m_functions.push_back(f);
    return m_functions.size() - 1;
}