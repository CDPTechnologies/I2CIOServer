/**
 * (c)2017 CDP Technologies AS
 */

#ifndef I2CDEVICE_H
#define I2CDEVICE_H

#include <CDPSystem/Base/CDPBaseObject.h>

#include <StudioAPI/CDPNode.h>

#include <IO/ServerIO/ChannelManager.h>

class I2CAdapter;

class I2CDevice : public CDP::StudioAPI::CDPNode
{
public:
  I2CDevice();
  virtual ~I2CDevice();

  void Configure(XMLElementEx* element, CDPComponent* owner, ServerIO::ChannelManager* channelManager);
  void Process(I2CAdapter& adapter);

  virtual const std::string GetNodeName() const override;
  virtual std::string GetNodeTypeName() const override;
  virtual void FillNodeChildren(CDP::StudioAPI::NodeStream &serializer) const override;

private:
  I2CDevice(const I2CDevice& rhs) = delete;
  const I2CDevice& operator= (const I2CDevice& rhs) = delete;

  class Private;
  std::unique_ptr<Private> d;
};

#endif
