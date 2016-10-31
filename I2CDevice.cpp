/**
 * (c)2016 CDP Technologies AS
 */

#include "I2CDevice.h"

#include "I2CAdapter.h"

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
