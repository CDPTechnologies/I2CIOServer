/**
 * (c)2017 CDP Technologies AS
 */

#include "I2CIOServer.h"
#include "I2CIOBuilder.h"

using namespace std;

I2CIOBuilder::I2CIOBuilder(const char* libName, const char* timeStamp)
  : CDPBuilder(libName, timeStamp)
{
}

CDPComponent* I2CIOBuilder::CreateNewComponent(const string& type)
{
  if (type == "I2CIOServer")
    return new I2CIOServer;

  return nullptr;
}

CDPBaseObject* I2CIOBuilder::CreateNewCDPOperator(const string&, const string&, const CDPPropertyBase*)
{
  return nullptr;
}
