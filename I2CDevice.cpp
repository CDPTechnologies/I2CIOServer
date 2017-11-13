/**
 * (c)2017 CDP Technologies AS
 */

#include "I2CDevice.h"

#include "I2CAdapter.h"
#include "I2CDeviceChannelGroup.h"
#include "I2CException.h"
#include "I2CHelpers.h"

#include <CDPSystem/Base/CDPComponent.h>
#include <CDPSystem/Base/CDPProperty.h>

#include <StudioAPI/NodeStream.h>

using namespace CDP::StudioAPI;
using namespace ServerIO;
using namespace std;

class I2CDevice::Private
{
public:
  void ConfigureChannelGroups(XMLElementEx* element, CDPComponent* owner, ChannelManager* channelManager)
  {
    for (auto rg = element->GetChild("ChannelGroup"); rg != nullptr; rg = rg->GetNext("ChannelGroup"))
      ConfigureChannelGroup(rg, owner, channelManager);
  }

  void ConfigureChannelGroup(XMLElementEx* element, CDPComponent* owner, ChannelManager* channelManager)
  {
    auto cg = new I2CDeviceChannelGroup(name);
    cg->Configure(element, owner, channelManager);
    channelGroups.push_back(cg);
  }

  bool TrySetAdapterAddress(I2CAdapter& adapter) const
  {
    try
    {
      adapter.SetAddress(address);
      return true;
    }
    catch (const I2CException& e)
    {
      MessageLine("I2CDevice: Cannot set address to %s: %s", c_str(address), e.what());
      return false;
    }
  }

  bool TryReadAdapter(I2CAdapter& adapter, uint8_t subaddress, std::vector<uint8_t>& buffer) const
  {
    try
    {
      buffer = adapter.Read(subaddress, buffer.size());
      return true;
    }
    catch (const I2CException& e)
    {
      MessageLine("I2CDevice: Cannot read from address %s: %s", subaddress, e.what());
      return false;
    }
  }

  vector<I2CDeviceChannelGroup*> channelGroups;

  CDPProperty<string> name;
  CDPProperty<string> model;
  CDPProperty<unsigned char> address;
};

I2CDevice::I2CDevice()
  : d(new Private)
{
}

void I2CDevice::Configure(XMLElementEx* element, CDPComponent* owner, ChannelManager* channelManager)
{
  ConfigureProperty(d->name, "Name", owner, element);
  ConfigureProperty(d->model, "Model", owner, element);
  ConfigureProperty(d->address, "Address", owner, element);

  d->ConfigureChannelGroups(element, owner, channelManager);
}

I2CDevice::~I2CDevice()
{
}

void I2CDevice::Process(I2CAdapter& adapter)
{
  if (!d->TrySetAdapterAddress(adapter))
    return;
  for (auto& cg : d->channelGroups)
    cg->Process(adapter);
}

const string I2CDevice::GetNodeName() const
{
  return d->name;
}

string I2CDevice::GetNodeTypeName() const
{
  return d->model;
}

void I2CDevice::FillNodeChildren(NodeStream &serializer) const
{
  CDPNode::FillNodeChildren(serializer);
  serializer.StdContainerStreamer(d->channelGroups);
  serializer << AdoptedChild(d->name)
             << AdoptedChild(d->model)
             << AdoptedChild(d->address);
}
