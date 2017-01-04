/**
 * (c)2017 CDP Technologies AS
 */

#ifndef I2CIOBUILDER_H
#define I2CIOBUILDER_H

#include <CDPSystem/Application/CDPBuilder.h>

class I2CIOBuilder : public CDPBuilder
{
public:
  I2CIOBuilder(const char* libName, const char* timeStamp);
  CDPComponent* CreateNewComponent(const std::string& type) override;
  CDPBaseObject* CreateNewCDPOperator(const std::string& modelName, const std::string& type, const CDPPropertyBase* inputProperty) override;
};

#endif
