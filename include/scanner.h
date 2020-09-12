#pragma once

#include <core.h>
#include <token.h>

class Scanner
{
private:
    int m_putback = 0;
    FILE *m_infile = nullptr;

    Token m_token;
    Token m_prevToken;

    std::string m_lastIdentifier;

private:
    int next();
    int skip();
    void skipLine();
    int scanInt(int c);
    int scanReg();
    std::string scanIdentifier(int c);
    int identifyKeyword(std::string id);
    void putback(int c);

public:
    Token &scan();
    Token &match(int tok);
    Token &match(int tok1, int tok2);
    Token &scanUntil(int tok);
    Scanner(std::string openfile);

public:
    Token &token() { return m_token; }
    Token &prevToken() { return m_prevToken; }
    std::string lastIdentifier() { return m_lastIdentifier; }
};