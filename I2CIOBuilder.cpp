/**
 * (c)2017 CDP Technologies AS
 */

#include "I2CIOBuilder.h"
#include "I2CIOServer.h"
#include "ADS1115IOServer.h"

using namespace std;

I2CIOBuilder::I2CIOBuilder(const char* libName, const char* timeStamp)
  : CDPBuilder(libName, timeStamp)
{
}

CDPComponent* I2CIOBuilder::CreateNewComponent(const string& type)
{
  if (type == "I2CIOServer")
    return new I2CIOServer;
  if (type == "I2CIO.ADS1115IOServer")
    return new I2CIO::ADS1115IOServer;

  return nullptr;
}

CDPBaseObject* I2CIOBuilder::CreateNewCDPOperator(const string&, const string&, const CDPPropertyBase*)
{
  return nullptr;
}
