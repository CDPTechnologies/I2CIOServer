/**
 * (c)2016 CDP Technologies AS
 */

#include "I2CDevice.h"

#include "I2CAdapter.h"
#include "I2CException.h"
#include "I2CHelpers.h"

#include <CDPSystem/Base/CDPComponent.h>
#include <CDPSystem/Base/CDPProperty.h>

#include <IO/ServerIO/ICDPChannel.h>

#include <StudioAPI/NodeStream.h>

using namespace CDP::StudioAPI;
using namespace ServerIO;
using namespace std;

static const string Name = "Name";

class I2CDevice::Private
{
public:
  vector<ServerIO::ICDPChannel*> channels;

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
  auto prefix = element->GetAttributeValue(Name);

  d->name.Create(prefix.append(".Name").c_str(), owner);
  d->name.Configure(element->FindAttribute("Name"));
  d->name.AddNodeModeFlags(eValueIsReadOnly);
  d->name.SetNodeReparented(true);

  d->model.Create(prefix.append(".Model").c_str(), owner);
  d->model.Configure(element->FindAttribute("Model"));
  d->model.AddNodeModeFlags(eValueIsReadOnly);
  d->model.SetNodeReparented(true);

  d->address.Create(prefix.append(".Address").c_str(), owner);
  d->address.Configure(element->FindAttribute("Address"));
  d->address.AddNodeModeFlags(eValueIsReadOnly);
  d->address.SetNodeReparented(true);

  for (auto c = element->GetChild("CDPChannel"); c != nullptr; c = c->GetNext("CDPChannel"))
    d->channels.push_back(channelManager->HandleCDPChannelElement(c));
}

I2CDevice::~I2CDevice()
{
}

unsigned char I2CDevice::GetAddress() const
{
  return d->address;
}

void I2CDevice::Initialize(I2CAdapter&)
{
}

void I2CDevice::Process(I2CAdapter&)
{
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
  serializer.StdContainerStreamer(d->channels);
  serializer << AdoptedChild(d->name)
             << AdoptedChild(d->model)
             << AdoptedChild(d->address);
}

bool I2CDevice::TrySetAdapterAddress(I2CAdapter& adapter) const
{
  try
  {
    adapter.SetAddress(GetAddress());
    return true;
  }
  catch (const I2CException& e)
  {
    MessageLine("I2CDevice: Cannot set address to %s: %s", GetAddress(), e.what());
    return false;
  }
}


bool I2CDevice::TryWriteAdapter(I2CAdapter& adapter, uint8_t subaddress, const std::vector<uint8_t>& data) const
{
  try
  {
    adapter.Write(subaddress, data);
    return true;
  }
  catch (const I2CException& e)
  {
    MessageLine("I2CDevice: Cannot write to address %s: %s", subaddress, e.what());
    return false;
  }
}

bool I2CDevice::TryReadAdapter(I2CAdapter& adapter, uint8_t subaddress, std::vector<uint8_t>& buffer) const
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
