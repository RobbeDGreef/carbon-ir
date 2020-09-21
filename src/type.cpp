#include <type.h>
#include <api/carbon.h>

Type::Type(struct ctype t)
{
    m_byteSize = t.byte_size;
    m_ptr = t.ptr;
    m_isFloat = t.is_float;
}
