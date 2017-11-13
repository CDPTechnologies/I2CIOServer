/**
 * (c)2017 CDP Technologies AS
 */

#include "ADS1115Reader.h"

#include <CDPDefines.h>

namespace I2CIO {

class ADS1115Reader::Private {};

ADS1115Reader::ADS1115Reader()
{
}

ADS1115Reader::~ADS1115Reader() = default;

void ADS1115Reader::Configure(const std::string&)
{
  CDPMessage("Warning: ADS1115 IOServer is not supported on Windows\n");
}

short ADS1115Reader::Read(int, int, int)
{
  return 0;
}

}
