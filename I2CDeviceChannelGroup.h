/**
 * (c)2016 CDP Technologies AS
 */

#ifndef I2CDEVICECHANNELGROUP_H
#define I2CDEVICECHANNELGROUP_H

#include <IO/ServerIO/ChannelManager.h>

#include <StudioAPI/CDPNode.h>

#include <CDPSystem/Base/CDPBaseObject.h>

class I2CAdapter;

class I2CDeviceChannelGroup : public CDP::StudioAPI::CDPNode
{
public:
  I2CDeviceChannelGroup();
  virtual ~I2CDeviceChannelGroup();

  void Configure(XMLElementEx* element, CDPComponent* owner, ServerIO::ChannelManager* channelManager);
  void Process(I2CAdapter& adapter);

  virtual const std::string GetNodeName() const override;
  virtual std::string GetNodeTypeName() const override;
  virtual void FillNodeChildren(CDP::StudioAPI::NodeStream &serializer) const override;

private:
  I2CDeviceChannelGroup(const I2CDeviceChannelGroup& rhs) = delete;
  const I2CDeviceChannelGroup& operator= (const I2CDeviceChannelGroup& rhs) = delete;

  class Private;
  std::unique_ptr<Private> d;
};

#endif
