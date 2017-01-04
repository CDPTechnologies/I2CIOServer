/**
 * (c)2017 CDP Technologies AS
 */

#include "I2CAdapter.h"

#include "I2CException.h"

#include <linux/i2c-dev.h>

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>

using namespace std;

class I2CAdapter::Private
{
public:
  Private()
    : file(-1)
  {
  }

  void SetAddress(uint8_t address)
  {
    if (ioctl(file, I2C_SLAVE, address) == -1)
      throw I2CException(strerror(errno));
  }

  void Write(const vector<uint8_t>& data)
  {
    if (write(file, data.data(), data.size()) == -1)
      throw I2CException(strerror(errno));
  }

  vector<uint8_t> Read(vector<uint8_t>::size_type size)
  {
    vector<uint8_t> buffer(size);
    if (read(file, buffer.data(), buffer.size()) == -1)
      throw I2CException(strerror(errno));
    return buffer;
  }

  int file;
};

I2CAdapter::I2CAdapter()
  : d(new Private)
{
}

I2CAdapter::~I2CAdapter()
{
}

void I2CAdapter::Open(const string& filename)
{
  d->file = open(filename.c_str(), O_RDWR);
  if (d->file == -1)
    throw I2CException(strerror(errno));
}

bool I2CAdapter::IsOpen() const
{
  return d->file != -1;
}

void I2CAdapter::SetAddress(uint8_t address)
{
  d->SetAddress(address);
}

vector<uint8_t> I2CAdapter::Read(uint8_t subaddress, vector<uint8_t>::size_type size)
{
  d->Write({ subaddress });
  return d->Read(size);
}

void I2CAdapter::Write(uint8_t subaddress, const vector<uint8_t>& data)
{
  auto buffer = vector<uint8_t>({ subaddress });
  buffer.insert(buffer.end(), data.begin(), data.end());
  d->Write(buffer);
}

void I2CAdapter::Close()
{
  if (d->file == -1)
    return; // throw?
  if (close(d->file) == -1)
    throw I2CException(strerror(errno));
}
