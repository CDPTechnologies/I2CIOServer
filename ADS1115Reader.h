/**
 * (c)2017 CDP Technologies AS
 */

#ifndef I2CIO_ADS1115READER_H
#define I2CIO_ADS1115READER_H

#include <string>
#include <memory>

namespace I2CIO {

class ADS1115Reader
{
public:
  ADS1115Reader();
  ~ADS1115Reader();

  void Configure(const std::string& adapter);
  short Read(int channel, int gain, int dataRate);

private:
  class Private;
  std::unique_ptr<Private> d;
};

}

#endif
