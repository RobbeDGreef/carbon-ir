#pragma once

#include <string>

class ErrorSystem
{
public:
    void fatal(std::string reason) __attribute__((noreturn));
    void syntaxError(std::string reason) __attribute__((noreturn));
};