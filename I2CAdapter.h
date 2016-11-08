/**
 * (c)2016 CDP Technologies AS
 */

#ifndef I2CADAPTER_H
#define I2CADAPTER_H

#include <memory>
#include <string>
#include <vector>

class I2CAdapter
{
public:
  I2CAdapter();
  ~I2CAdapter();

  void Open(const std::string& filename);
  bool IsOpen() const;
  void SetAddress(uint8_t address);
  std::vector<uint8_t> Read(uint8_t subaddress, std::vector<uint8_t>::size_type size);
  void Write(uint8_t subaddress, const std::vector<uint8_t>& data);
  void Close();

private:
  I2CAdapter(const I2CAdapter& rhs) = delete;
  const I2CAdapter& operator= (const I2CAdapter& rhs) = delete;

  class Private;
  std::unique_ptr<Private> d;
};

#endif
