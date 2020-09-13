#pragma once

#include <core.h>
#include <debug.h>

static const char *TokenTypes[] = 
{
    "NULL", "reg", "intlit", "identifier",
    "add", "sub", "mul", "div", "mod",
    "call", "alloca", "store", "load",
    "jmp", "jmpcond", "cmp",
    "return",

    "eq", "neq", "le", "l", "ge", "g",
    "function", "attribute", 
    "comma", "left parenthesis '('", "right parenthesis ')'", "equalsign", "newline",
    "left brace '{'", "right brace '}'", "star '*'", "colon ':'",
    "left bracket '['", "right bracket ']'", 
    "i8", "i16", "i32", "i64",
    "f32", "f64",
    "glob"
};

class Token
{
public:
    enum Types
    {
        REG = 1, INTLIT, IDENTIFIER,

        ADD, SUB, MUL, DIV, MOD,
        CALL, ALLOCA, STORE, LOAD,
        JMP, JMPCOND, CMP,
        RETURN,

        EQ, NEQ, LE, L, GE, G,
        FUNCTION, 

        ATTRIBUTE, 
        COMMA, LPAREN, RPAREN, EQUALSIGN, NEWLINE,
        LBRACE, RBRACE, STAR, COLON,
        LBRACKET, RBRACKET,

        I8, I16, I32, I64,
        F32, F64,

        GLOB,

        TOKAMOUNT
    };

private:
    int m_token = 0;
    int m_intValue = 0;

    std::string m_identifier;

public:

#ifdef MODE_DEBUG
    void print() { dbg_printf("Scanner token %s (%i) val %i identifier '%s'\n", getName().c_str(), m_token, m_intValue, m_identifier.c_str()); }
#endif

    int token() { return m_token; }
    int intValue() { return m_intValue; }
    std::string identifier() { return m_identifier; }

    void set(int tok, int val);
    void set(int tok, std::string id);

    void setToken(int tok);
    void setIntValue(int val);
    void setIdentifier(std::string id);

    std::string getName();
};