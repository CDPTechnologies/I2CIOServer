/**
 * (c)2017 CDP Technologies AS
 */

#include "I2CAdapter.h"

using namespace std;

class I2CAdapter::Private
{
public:
  Private()
    : isOpen(false)
  {
  }

  bool isOpen;
};

I2CAdapter::I2CAdapter()
  : d(new Private)
{
}

I2CAdapter::~I2CAdapter()
{
}

void I2CAdapter::Open(const string&)
{
  d->isOpen = true;
}

bool I2CAdapter::IsOpen() const
{
  return d->isOpen;
}

void I2CAdapter::SetAddress(uint8_t)
{
}

vector<uint8_t> I2CAdapter::Read(uint8_t, vector<uint8_t>::size_type size)
{
  return vector<uint8_t>(size);
}

void I2CAdapter::Write(uint8_t, const vector<uint8_t>&)
{
}

void I2CAdapter::Close()
{
  d->isOpen = false;
}
