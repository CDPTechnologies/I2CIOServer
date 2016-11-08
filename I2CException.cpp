/**
 * (c)2016 CDP Technologies AS
 */

#include "I2CException.h"

using namespace std;

I2CException::I2CException(const string &what)
    : m_what(what)
{
}

const char *I2CException::what() const noexcept
{
    return m_what.c_str();
}
