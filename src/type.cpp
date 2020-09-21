#include <type.h>
#include <api/carbon.h>

Type::Type(struct ctype t)
{
    m_byteSize = t.byteSize;
    m_ptr = t.ptr;
    m_isFloat = t.isFloat;
}
