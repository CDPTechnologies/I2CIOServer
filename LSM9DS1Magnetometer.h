/**
 * (c)2016 CDP Technologies AS
 */

#ifndef LSM9DS1MAGNETOMETER_H
#define LSM9DS1MAGNETOMETER_H

#include "I2CDevice.h"

class LSM9DS1Magnetometer : public I2CDevice
{
public:
  LSM9DS1Magnetometer();

  void Configure(XMLElementEx* element, CDPComponent* owner, ServerIO::ChannelManager* channelManager) override;

  void Initialize(I2CAdapter& adapter) override;
  void Process(I2CAdapter& adapter) override;

private:
  LSM9DS1Magnetometer(const LSM9DS1Magnetometer& rhs) = delete;
  const LSM9DS1Magnetometer& operator= (const LSM9DS1Magnetometer& rhs) = delete;

  void ReadMagneticField(I2CAdapter& adapter);
  double CalcMagneticField(uint8_t high, uint8_t low) const;
  int16_t Combine(int16_t high, int16_t low) const;

  double m_magneticFieldX;
  double m_magneticFieldY;
  double m_magneticFieldZ;
};

#endif
