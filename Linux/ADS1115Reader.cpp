/**
 * (c)2017 CDP Technologies AS
 */

#include "ADS1115Reader.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>

#include <OSAPI/Network/CDPNetwork.h>
#include <OSAPI/Process/OSAPIProcess.h>

#include <I2CException.h>

namespace I2CIO {

class ADS1115Reader::Private
{
public:
  void Configure(const std::string& adapter)
  {
    uint8_t asdAddress = 0x48;
    if(adapter.empty())
      throw I2CException("No adapter set");

    if ((m_fd = open(adapter.c_str(), O_RDWR)) < 0)
      throw I2CException("Error: Couldn't open device " + adapter);
    if (ioctl(m_fd, I2C_SLAVE, asdAddress) < 0)
      throw I2CException("Error: Couldn't find device on address " + std::to_string(asdAddress));
  }

  short Read(int channel, int gain, int dataRate)
  {
    WriteConfig(dataRate, gain, channel);
    WaitForConversion();
    SelectConversionRegister();
    return ReadConversionRegister();
  }

  void WriteConfig(int dataRate, int gain, int channel)
  {
    uint8_t config[3];
    config[0] = 0x01; // Select config register
    config[1] = 0x81 | (channel << 4) | (gain << 1);
    config[2] = 0x03 | (dataRate << 5);
    if (write(m_fd, config, 3) != 3)
      throw I2CException("Selecting channel, gain and sample rate failed");
  }

  void WaitForConversion()
  {
    uint8_t readBuf[2] {};
    while (true)
    {
      if (read(m_fd, readBuf, 2) != 2)
        throw I2CException("Reading config register failed\n");
      if (readBuf[0] & 0x80)
        break;
      OSAPISleep(0, 500);
    }
  }

  void SelectConversionRegister()
  {
    uint8_t reg[] = {0x00};
    if (write(m_fd, reg, 1) != 1)
      throw I2CException("Selecting conversion register failed");
  }

  short ReadConversionRegister()
  {
    short buffer = 0;
    if (read(m_fd, &buffer, sizeof buffer) != 2)
      throw I2CException("Reading conversion register failed");
    return ntoh(buffer);
  }

private:
  int m_fd{0};
};

ADS1115Reader::ADS1115Reader()
  : d(new Private())
{
}

ADS1115Reader::~ADS1115Reader() = default;

void ADS1115Reader::Configure(const std::string& adapter)
{
  d->Configure(adapter);
}

short ADS1115Reader::Read(int channel, int gain, int dataRate)
{
  return d->Read(channel, gain, dataRate);
}


}
