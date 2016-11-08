/**
 * (c)2016 CDP Technologies AS
 */

#ifndef I2CHELPERS_H
#define I2CHELPERS_H

#include <CDPDefines.h>

#include <string>

template <typename T>
static const char* c_str(const T& t)
{
  return static_cast<const std::string&>(t).c_str();
}

template <typename ... T>
static void MessageLine(const std::string& format, const T& ... args)
{
  CDPMessage((format + '\n').c_str(), args ...);
}

#endif
