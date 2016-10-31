/**
 * (c)2016 CDP Technologies AS
 */

#ifndef LSM9DS1ACCELEROMETERGYROSCOPE_H
#define LSM9DS1ACCELEROMETERGYROSCOPE_H

#include "I2CDevice.h"

class LSM9DS1AccelerometerGyroscope : public I2CDevice
{
public:
  LSM9DS1AccelerometerGyroscope();

  void Configure(XMLElementEx* element, CDPComponent* owner, ServerIO::ChannelManager* channelManager) override;

  void Initialize(I2CAdapter& adapter) override;
  void Process(I2CAdapter& adapter) override;

private:
  LSM9DS1AccelerometerGyroscope(const LSM9DS1AccelerometerGyroscope& rhs) = delete;
  const LSM9DS1AccelerometerGyroscope& operator= (const LSM9DS1AccelerometerGyroscope& rhs) = delete;

  void ReadAngularRate(I2CAdapter& adapter);
  void ReadAcceleration(I2CAdapter& adapter);
  double CalcAngularResolution(uint8_t high, uint8_t low) const;
  double CalcAcceleration(uint8_t high, uint8_t low) const;
  int16_t Combine(int16_t high, int16_t low) const;

  double m_angularRateX;
  double m_angularRateY;
  double m_angularRateZ;
  double m_accelerationX;
  double m_accelerationY;
  double m_accelerationZ;
};

#endif
