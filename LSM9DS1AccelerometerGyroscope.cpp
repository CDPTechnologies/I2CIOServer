/**
 * (c)2016 CDP Technologies AS
 */

#include "LSM9DS1AccelerometerGyroscope.h"

#include "I2CAdapter.h"

#include <IO/ServerIO/ICDPChannel.h>

using namespace std;

static uint8_t CTRL_REG1_G = 0x10;

static double AngularRateFullScaleDps = 245;
static double AccelerationFullScaleG = 2;
static double AngularRateResolution = AngularRateFullScaleDps / 32768;
static double AccelerationResolution = AccelerationFullScaleG / 32768;

LSM9DS1AccelerometerGyroscope::LSM9DS1AccelerometerGyroscope()
  : m_angularRateX(0)
  , m_angularRateY(0)
  , m_angularRateZ(0)
  , m_accelerationX(0)
  , m_accelerationY(0)
  , m_accelerationZ(0)
{
}

void LSM9DS1AccelerometerGyroscope::Configure(XMLElementEx* element, CDPComponent* owner, ServerIO::ChannelManager* channelManager)
{
  I2CDevice::Configure(element, owner, channelManager);
  // TODO error handling
  channelManager->HasChannel("AngularRateX")->SetDataPointer(&m_angularRateX);
  channelManager->HasChannel("AngularRateY")->SetDataPointer(&m_angularRateY);
  channelManager->HasChannel("AngularRateZ")->SetDataPointer(&m_angularRateZ);
  channelManager->HasChannel("AccelerationX")->SetDataPointer(&m_accelerationX);
  channelManager->HasChannel("AccelerationY")->SetDataPointer(&m_accelerationY);
  channelManager->HasChannel("AccelerationZ")->SetDataPointer(&m_accelerationZ);
}

void LSM9DS1AccelerometerGyroscope::Initialize(I2CAdapter& adapter)
{
  adapter.SetAddress(GetAddress());
  adapter.Write(CTRL_REG1_G, { 0x60 });
}

void LSM9DS1AccelerometerGyroscope::Process(I2CAdapter& adapter)
{
  adapter.SetAddress(GetAddress());
  ReadAngularRate(adapter);
  ReadAcceleration(adapter);
}

void LSM9DS1AccelerometerGyroscope::ReadAngularRate(I2CAdapter& adapter)
{
  auto buffer = adapter.Read(0x18, 6);
  m_angularRateX = CalcAngularResolution(buffer.at(1), buffer.at(0));
  m_angularRateY = CalcAngularResolution(buffer.at(3), buffer.at(2));
  m_angularRateZ = CalcAngularResolution(buffer.at(5), buffer.at(4));
}

void LSM9DS1AccelerometerGyroscope::ReadAcceleration(I2CAdapter& adapter)
{
  auto buffer = adapter.Read(0x28, 6);
  m_accelerationX = CalcAcceleration(buffer.at(1), buffer.at(0));
  m_accelerationY = CalcAcceleration(buffer.at(3), buffer.at(2));
  m_accelerationZ = CalcAcceleration(buffer.at(5), buffer.at(4));
}

double LSM9DS1AccelerometerGyroscope::CalcAngularResolution(uint8_t high, uint8_t low) const
{
  return AngularRateResolution * Combine(high, low);
}

double LSM9DS1AccelerometerGyroscope::CalcAcceleration(uint8_t high, uint8_t low) const
{
  return AccelerationResolution * (int16_t) Combine(high, low);
}

int16_t LSM9DS1AccelerometerGyroscope::Combine(int16_t high, int16_t low) const
{
  return high << 8 | low;
}
