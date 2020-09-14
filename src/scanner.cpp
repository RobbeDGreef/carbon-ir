#include <scanner.h>
#include <config.h>

Scanner::Scanner(std::string file)
{
    m_infile = fopen(file.c_str(), "r");

    if (m_infile == nullptr)
        g_errsys.fatal("Unable to open file " BOLD("\"" + file + "\""));

}

int Scanner::next()
{
    if (m_putback)
    {
        int c = m_putback;
        m_putback = 0;
        return c;
    }

    return fgetc(m_infile);
}

void Scanner::putback(int c)
{
    m_putback = c;
}

void Scanner::skipLine()
{
    int c = next();

    while (c != '\n' && c != EOF)
        c = next();
    
    putback(c);
}

int Scanner::skip()
{
    int c = next();
    while (c == ' ' || c == '\t' || c == '\r')
    {
        c = next();
    }

    return c;
}

static int chrpos(char *s, int c)
{
    char *p = strchr(s, c);
    return (p ? p - s : -1);
}

int Scanner::scanInt(int c)
{
    int i = 0;
    int val = 0;

    while ((i = chrpos((char *)"0123456789", c)) >= 0)
    {
        val = val * 10 + i;
        c = next();
    }

    putback(c);
    return val;
}

int Scanner::scanReg()
{
    int c = next();
    if (!isdigit(c))
        g_errsys.syntaxError("expected a integer to identify the register");
    
    return scanInt(c);
}

std::string Scanner::scanIdentifier(int c)
{
    /// @todo: this might not be an optimal solution (the deallocation and reallocation
    ///        of the string)
    std::string id;

    for (int i = 0; i < SCANNER_IDENTIFIER_LIMMIT; i++)
    {
        if (isalpha(c) || isdigit(c) || c == '_' || c == '@')
        {
            id += c;
            c = next();
        }
        else
        {
            putback(c);
            m_lastIdentifier = id;
            return id;
        }
    }

    g_errsys.syntaxError("identifier too long");
}

int Scanner::identifyKeyword(std::string id)
{
    /// Small optimization
    switch(id[0])
    {
    case 'a':
        if (!id.compare("add"))
            return Token::Types::ADD;
        
        if (!id.compare("alloca"))
            return Token::Types::ALLOCA;
        break;
    
    case 'c':
        if (!id.compare("call"))
            return Token::Types::CALL;
        if (!id.compare("cmp"))
            return Token::Types::CMP;
        break;

    case 'd':
        if (!id.compare("div"))
            return Token::Types::DIV;
        break;
    
    case 'e':
        if (!id.compare("eq"))
            return Token::Types::EQ;
        if (!id.compare("extern"))
            return Token::Types::EXTERN;
        break;

    case 'f':
        if (!id.compare("function"))
            return Token::Types::FUNCTION;
        break;
    
    case 'g':
        if (!id.compare("ge"))
            return Token::Types::GE;
        if (!id.compare("g"))
            return Token::Types::G;
        break;

    case 'i':
        if (!id.compare("i8"))
            return Token::Types::I8;
        if (!id.compare("i16"))
            return Token::Types::I16;
        if (!id.compare("i32"))
            return Token::Types::I32;
        if (!id.compare("i64"))
            return Token::Types::I64;
        break;
    
    case 'j':
        if (!id.compare("jmp"))
            return Token::Types::JMP;
        
        if (!id.compare("jmpcond"))
            return Token::Types::JMPCOND;
        break;

    case 'l':
        if (!id.compare("load"))
            return Token::Types::LOAD;
        if (!id.compare("l"))
            return Token::Types::L;
        if (!id.compare("le"))
            return Token::Types::LE;
        break;
    
    case 'r':
        if (!id.compare("return"))
            return Token::Types::RETURN;
        break;

    case 's':
        if (!id.compare("store"))
            return Token::Types::STORE;
        if (!id.compare("sub"))
            return Token::Types::SUB;
        break;

    case 'm':
        if (!id.compare("mul"))
            return Token::Types::MUL;
        else if (!id.compare("mod"))
            return Token::Types::MOD;
        break;
    
    case 'n':
        if (!id.compare("neq"))
            return Token::Types::NEQ;
        break;
    }

    return 0;
}

Token &Scanner::match(int tok)
{
    if (m_token.token() == tok)
        return scan();
    
    dbg_call(m_token.print();)
    g_errsys.syntaxError("unexpected token");
}

Token &Scanner::match(int tok1, int tok2)
{
    if (m_token.token() == tok1 || m_token.token() == tok2)
        return scan();

    dbg_call(m_token.print();)
    g_errsys.syntaxError("unexpected token");
}

Token &Scanner::scanUntil(int tok)
{
    while (m_token.token() != tok)
        scan();

    return m_token;
}

Token &Scanner::scan()
{
    int c = skip();

    switch (c)
    {
    case ',':
        /// @todo: I don't think we ever use comma's anymore so this can probably be removed
        m_token.setToken(Token::Types::COMMA);
        break;

    case '\n':
        m_token.setToken(Token::Types::NEWLINE);
        break;
    
    case '%':
        m_token.set(Token::Types::REG, scanReg());
        break;
    
    case '=':
        m_token.setToken(Token::Types::EQUALSIGN);
        break;
    
    case '(':
        m_token.setToken(Token::Types::LPAREN);
        break;
    
    case ')':
        m_token.setToken(Token::Types::RPAREN);
        break;
    
    case '{':
        m_token.setToken(Token::Types::LBRACE);
        break;
    
    case '}':
        m_token.setToken(Token::Types::RBRACE);
        break;
    
    case '[':
        m_token.setToken(Token::Types::LBRACKET);
        break;
    
    case ']':
        m_token.setToken(Token::Types::RBRACKET);
        break;
    
    case '*':   
        m_token.setToken(Token::Types::STAR);
        break;

    case ':':
        m_token.setToken(Token::Types::COLON);
        break;

    case '#':
        skipLine();
        return scan();
    
    case '@':
        m_token.set(Token::Types::GLOB, scanIdentifier(c));
        break;

    case EOF:
        m_token.setToken(EOF);
        break;

    default:
        if (isdigit(c))
        {
            m_token.set(Token::Types::INTLIT, scanInt(c));
        }
        else if (isalpha(c) || c == '_')
        {
            /// Also stores the string in scanner::m_lastIdentifier
            std::string id = scanIdentifier(c);

            /// @todo: function names should not be scanned as keyword
            int tok = identifyKeyword(id);
            if (tok)
            {
                m_token.setToken(tok);
                break;
            }
            
            m_token.set(Token::Types::IDENTIFIER, id);
            break;
        }
    }

    dbg_call(m_token.print());
    return m_token;
}