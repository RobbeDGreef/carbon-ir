#pragma once

#include <core.h>

class Attributes : public std::vector<std::string>
{
public:
    std::string getValueOf(std::string);
    int getBoolValueOf(std::string);
};