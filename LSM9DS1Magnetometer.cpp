/**
 * (c)2016 CDP Technologies AS
 */

#include "LSM9DS1Magnetometer.h"

#include "I2CAdapter.h"
#include "I2CException.h"
#include "I2CHelpers.h"

#include <IO/ServerIO/ICDPChannel.h>

using namespace std;

static uint8_t CTRL_REG3_M = 0x22;

static double MagneticFieldFullScaleGauss = 4;
static double MagneticFieldResolution = MagneticFieldFullScaleGauss / 32768;

LSM9DS1Magnetometer::LSM9DS1Magnetometer()
  : m_magneticFieldX(0)
  , m_magneticFieldY(0)
  , m_magneticFieldZ(0)
{
}

void LSM9DS1Magnetometer::Configure(XMLElementEx* element, CDPComponent* owner, ServerIO::ChannelManager* channelManager)
{
  I2CDevice::Configure(element, owner, channelManager);
  // TODO error handling
  channelManager->HasChannel("MagneticFieldX")->SetDataPointer(&m_magneticFieldX);
  channelManager->HasChannel("MagneticFieldY")->SetDataPointer(&m_magneticFieldY);
  channelManager->HasChannel("MagneticFieldZ")->SetDataPointer(&m_magneticFieldZ);
}

void LSM9DS1Magnetometer::Initialize(I2CAdapter& adapter)
{
  if (!TrySetAdapterAddress(adapter))
  {
    MessageLine("LSM9DS1Magnetometer: Initialization failed");
    return;
  }
  if (!TryWriteAdapter(adapter, CTRL_REG3_M, { 0x00 }))
    MessageLine("LSM9DS1Magnetometer: Initialization failed");
}

void LSM9DS1Magnetometer::Process(I2CAdapter& adapter)
{
  adapter.SetAddress(GetAddress());
  ReadMagneticField(adapter);
}

void LSM9DS1Magnetometer::ReadMagneticField(I2CAdapter& adapter)
{
  vector<uint8_t> buffer(6);
  if (!TryReadAdapter(adapter, 0x28, buffer))
  {
    MessageLine("LSM9DS1Magnetometer: Reading angular rate failed");
    return;
  }
  m_magneticFieldX = CalcMagneticField(buffer.at(1), buffer.at(0));
  m_magneticFieldY = CalcMagneticField(buffer.at(3), buffer.at(2));
  m_magneticFieldZ = CalcMagneticField(buffer.at(5), buffer.at(4));
}

double LSM9DS1Magnetometer::CalcMagneticField(uint8_t high, uint8_t low) const
{
  return MagneticFieldResolution * Combine(high, low);
}

int16_t LSM9DS1Magnetometer::Combine(int16_t high, int16_t low) const
{
  return high << 8 | low;
}
