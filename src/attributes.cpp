#include <attributes.h>

std::string Attributes::getValueOf(std::string s)
{
    for (std::string attr : *this)
    {
        if (attr.find(s) == 0)
            return attr.substr(s.size() + 1);
    }

    return "";
}

int Attributes::getBoolValueOf(std::string s)
{
    for (std::string attr : *this)
    {
        /// @todo: this might not be the cleanest implementation (will just return false if it is not "true")
        if (attr.find(s) == 0)
            return (attr.substr(s.size() + 1) == "true") ? true : false;
    }

    return -1;
}