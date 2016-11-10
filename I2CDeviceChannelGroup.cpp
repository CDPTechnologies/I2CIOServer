/**
 * (c)2016 CDP Technologies AS
 */

#include "I2CDeviceChannelGroup.h"

#include "I2CAdapter.h"
#include "I2CException.h"
#include "I2CHelpers.h"

#include <IO/ServerIO/DeltaValidatorSendTrigger.h>
#include <IO/ServerIO/ICDPChannel.h>
#include <IO/ServerIO/ISendTrigger.h>

#include <StudioAPI/NodeStream.h>

#include <CDPSystem/Base/CDPComponent.h>
#include <CDPSystem/Base/CDPProperty.h>

using namespace CDP::StudioAPI;
using namespace ServerIO;
using namespace std;

// Calculates buffer begin (inclusive) offset and end (exclusive) offset for
// each channel
static vector<pair<size_t, size_t>> CalcChannelDataOffsets(
    const vector<ICDPChannel*>& channels)
{
  vector<pair<size_t, size_t>> offsets;

  auto size = 0u;
  auto bitCount = 0u;

  for (auto c : channels)
  {
    auto s = c->GetDataSize();
    if (c->GetNodeValueType() == CDPValueType::eBOOL)
    {
      if (bitCount == 0)
        size += s;
      if (++bitCount == 8)
        bitCount = 0;
    }
    else
    {
      size += s;
      bitCount = 0;
    }
    offsets.push_back(make_pair(size - s, size));
  }

  return offsets;
}

class SendTrigger : public ISendTrigger
{
public:
  SendTrigger()
    : m_isFlagged(true)
  {
  }

  void FlagForSend() override
  {
    m_isFlagged = true;
  }

  bool IsFlaggedForSend() const
  {
    return m_isFlagged;
  }

  void UnflagForSend()
  {
    m_isFlagged = false;
  }

private:
  bool m_isFlagged;
};

class I2CDeviceChannelGroup::Private
{
public:
  Private()
    : channelDataSize(0)
  {
  }

  void ConfigureChannels(XMLElementEx* element, CDPComponent* owner, ChannelManager* channelManager)
  {
    auto Channel = "Channel";
    for (auto c = element->GetChild(Channel); c != nullptr; c = c->GetNext(Channel))
      ConfigureChannel(c, owner, channelManager);
  }

  void ConfigureChannel(XMLElementEx* element, CDPComponent* owner, ChannelManager* channelManager)
  {
    auto dummy = 0.0;
    auto module = 0;
    auto group = 0;
    auto channelInput = !input;

    if (auto c = channelManager->NewICDPChannel(element, dummy, module, group, channelInput))
    {
      auto channelName = name + '.' + element->GetAttributeValue("Name").c_str();
      auto channelNumber = atoi(element->GetAttributeValue("Nr").c_str());

      c->Create(channelName.c_str(), owner);
      c->SetChannelParameters(channelInput, module, group, channelNumber);
      c->Configure(element);
      c->RegisterValidator(new DeltaValidatorSendTrigger(&sendTrigger));

      channelManager->RegisterCDPChannel(c); // takes ownership

      channels.push_back(c);
    }
  }

  void ConfigureChannelData()
  {
    auto offsets = CalcChannelDataOffsets(channels);
    if (offsets.empty())
      return;

    channelDataSize = offsets.back().second;
    channelData.reset(new uint8_t[channelDataSize]);
    for (auto i = 0u; i < channels.size(); ++i)
      channels[i]->SetDataPointer(&channelData[offsets[i].first]);
  }

  void ReadAdapter(I2CAdapter& adapter)
  {
    vector<uint8_t> buffer(channelDataSize);
    if (!TryReadAdapter(adapter, buffer))
      return;
    copy(buffer.begin(), buffer.end(), channelData.get());
  }

  bool TryReadAdapter(I2CAdapter& adapter, std::vector<uint8_t>& buffer) const
  {
    try
    {
      buffer = adapter.Read(address, buffer.size());
      return true;
    }
    catch (const I2CException& e)
    {
      MessageLine("I2CDeviceChannelGroup: Cannot read from address %s: %s", c_str(address), e.what());
      return false;
    }
  }

  void WriteAdapter(I2CAdapter& adapter)
  {
    vector<uint8_t> buffer(channelDataSize);
    copy(channelData.get(), channelData.get() + channelDataSize, buffer.data());
    if (!sendTrigger.IsFlaggedForSend() || !TryWriteAdapter(adapter, buffer))
      return;
    sendTrigger.UnflagForSend();
  }

  bool TryWriteAdapter(I2CAdapter& adapter, const std::vector<uint8_t>& buffer) const
  {
    try
    {
      adapter.Write(address, buffer);
      return true;
    }
    catch (const I2CException& e)
    {
      MessageLine("I2CDeviceChannelGroup: Cannot write to address %s: %s", c_str(address), e.what());
      return false;
    }
  }

  vector<ICDPChannel*> channels;
  unique_ptr<uint8_t[]> channelData;
  size_t channelDataSize;

  CDPProperty<string> name;
  CDPProperty<bool> input;
  CDPProperty<unsigned char> address;

  SendTrigger sendTrigger;
};

I2CDeviceChannelGroup::I2CDeviceChannelGroup()
  : d(new Private)
{
}

I2CDeviceChannelGroup::~I2CDeviceChannelGroup()
{
}

void I2CDeviceChannelGroup::Configure(XMLElementEx* element, CDPComponent* owner, ChannelManager* channelManager)
{
  ConfigureProperty(d->name, "Name", owner, element);
  ConfigureProperty(d->input, "Input", owner, element);
  ConfigureProperty(d->address, "Address", owner, element);

  d->ConfigureChannels(element, owner, channelManager);
  d->ConfigureChannelData();
}

void I2CDeviceChannelGroup::Process(I2CAdapter& adapter)
{
  if (d->input)
    d->ReadAdapter(adapter);
  else
    d->WriteAdapter(adapter);
}

const string I2CDeviceChannelGroup::GetNodeName() const
{
  return d->name;
}

string I2CDeviceChannelGroup::GetNodeTypeName() const
{
  return "I2CDeviceChannelGroup";
}

void I2CDeviceChannelGroup::FillNodeChildren(NodeStream &serializer) const
{
  CDPNode::FillNodeChildren(serializer);
  serializer.StdContainerStreamer(d->channels);
  serializer << AdoptedChild(d->name)
             << AdoptedChild(d->input)
             << AdoptedChild(d->address);
}
