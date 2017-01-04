/**
 * (c)2017 CDP Technologies AS
 */

#ifndef I2CEXCEPTION_H
#define I2CEXCEPTION_H

#include <exception>
#include <string>

class I2CException : public std::exception
{
public:
  I2CException(const std::string& what);

  const char* what() const noexcept;

private:
  const std::string m_what;
};

#endif
