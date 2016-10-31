/**
 * (c)2016 CDP Technologies AS
 */

#ifndef I2CDEVICEFACTORY_H
#define I2CDEVICEFACTORY_H

#include "I2CDevice.h"

class I2CDeviceFactory
{
public:
  static I2CDevice* CreateDevice(const std::string& model);

private:
  I2CDeviceFactory() = delete;
};

#endif
