/**
 * (c)2016 CDP Technologies AS
 */

#ifndef I2CHELPERS_H
#define I2CHELPERS_H

#include <CDPDefines.h>

#include <string>

template <typename ... T>
static void MessageLine(const std::string& format, const T& ... args)
{
  CDPMessage((format + '\n').c_str(), args ...);
}

#endif
