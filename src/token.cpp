#include <token.h>

void Token::set(int tok, int val)
{
    m_token = tok;
    m_intValue = val;
}

void Token::set(int tok, std::string id)
{
    m_token = tok;
    m_identifier = id;
}

void Token::setToken(int tok)
{
    m_token = tok;
}
void Token::setIntValue(int val)
{
    m_intValue = val;
}

void Token::setIdentifier(std::string id)
{
    m_identifier = id;
}

std::string Token::getName()
{
    if (m_token < Token::Types::TOKAMOUNT && m_token > 0)
        return std::string(TokenTypes[m_token]);
    
    return "NULL";
}