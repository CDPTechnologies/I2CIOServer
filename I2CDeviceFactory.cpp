/**
 * (c)2016 CDP Technologies AS
 */

#include "I2CDeviceFactory.h"

#include "LSM9DS1AccelerometerGyroscope.h"
#include "LSM9DS1Magnetometer.h"

using namespace std;

I2CDevice* I2CDeviceFactory::CreateDevice(const string& model)
{
  if (model == "I2CDevice")
    return new I2CDevice();

  else if (model == "LSM9DS1AccelerometerGyroscope")
    return new LSM9DS1AccelerometerGyroscope();

  else if (model == "LSM9DS1Magnetometer")
    return new LSM9DS1Magnetometer();

  return nullptr;
}
