/**
 * (c)2016 CDP Technologies AS
 */

#include "I2CAdapter.h"

#include <linux/i2c-dev.h>

#include <fcntl.h>
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
    // TODO error handling
    ioctl(file, I2C_SLAVE, address);
  }

  void Write(const vector<uint8_t>& data)
  {
    // TODO error handling
    write(file, data.data(), data.size());
  }

  vector<uint8_t> Read(vector<uint8_t>::size_type size)
  {
    // TODO error handling
    vector<uint8_t> buffer(size);
    read(file, buffer.data(), buffer.size());
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
  // TODO error handling
  d->file = open(filename.c_str(), O_RDWR);
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
  // TODO error handling
  close(d->file);
}
