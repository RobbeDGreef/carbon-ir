#include <argument.h>
#include <generator.h>
#include <debug.h>

std::string Argument::toString(Generator *gen)
{
#if 0
    if (m_type == Argument::Types::INTLIT)
        return std::to_string(m_intValue);
    
    if (m_type == Argument::Types::REG)
        return gen->registerToString(m_reg);
#endif

    /// @todo: remove argument class
    dbg_assert("deprecated");

    return "";
}