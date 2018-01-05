/**
 * (c)2017 CDP Technologies AS
 */

#include "ADS1115IOServer.h"

#include <array>
#include <map>

#include "I2CHelpers.h"

using namespace I2CIO;

const int INPUT_COUNT = 8;
const std::map<std::string, int> GAIN_MAP = {{"±6.144V", 0}, {"±4.096V", 1}, {"±2.048V", 2},
                                             {"±1.024V", 3}, {"±0.512V", 4}, {"±0.256V", 5}};
const std::map<std::string, int> DATA_RATE_MAP = {{"8SPS", 0}, {"16SPS", 1}, {"32SPS", 2}, {"64SPS", 3},
                                                  {"128SPS", 4}, {"250SPS", 5}, {"475SPS", 6}, {"860SPS", 7}};

ADS1115IOServer::ADS1115IOServer()
  : m_inputs(INPUT_COUNT)
  , m_enableInputs(INPUT_COUNT)
  , m_atomicEnableInputs(INPUT_COUNT)
  , m_mappedDataRate(DATA_RATE_MAP.at("128SPS"))
  , m_mappedGain(GAIN_MAP.at("±2.048V"))
  , m_periodMs(10)
  , m_commError(false)
{
}

namespace {
void ChangeHandler(std::atomic<int>& valueToChange, CDPPropertyBase* prop, const std::map<std::string, int>& valueMap)
{
  auto it = valueMap.find(prop->GetValue());
  if (it != valueMap.end())
    valueToChange = it->second;
  else
    CDPMessage("ADS1115IOServer: %s is not a valid value.", prop->GetValue().c_str());
}

std::string GetEnableInputName(const std::string& signalName)
{
  return "Enable" + signalName;
}
}

void ADS1115IOServer::Create(const char* fullName)
{
  CDPComponent::Create(fullName);
  m_transmissionAlarm.Create("TransmissionAlarm",this);
  m_adapter.Create("Adapter",this,CDPPropertyBase::e_Element,(CDPOBJECT_SETPROPERTY_HANDLER)nullptr,
                 (CDPOBJECT_VALIDATEPROPERTY_HANDLER)nullptr);
  m_adapter.AddNodeModeFlags(CDP::StudioAPI::eValueIsReadOnly);
  m_dataRate.Create("DataRate", this, CDPPropertyBase::e_Element);
  m_dataRate.SetPropertyChangeHandler([this] (CDPPropertyBase* prop) { ChangeHandler(m_mappedDataRate, prop, DATA_RATE_MAP); });
  m_gain.Create("Gain", this, CDPPropertyBase::e_Element);
  m_gain.SetPropertyChangeHandler([this] (CDPPropertyBase* prop) { ChangeHandler(m_mappedGain, prop, GAIN_MAP); });

  const char* inputNames[INPUT_COUNT] = {"A0A1", "A0A3", "A1A3", "A2A3", "A0", "A1", "A2", "A3"};
  for (int i = 0; i < INPUT_COUNT; ++i)
    m_inputs.at(i).Create(inputNames[i], this);

  for (int i = 0; i < INPUT_COUNT; ++i)
  {
    m_enableInputs.at(i).Create(GetEnableInputName(inputNames[i]).c_str(), this, CDPPropertyBase::e_Element);
    m_enableInputs.at(i).SetPropertyChangeHandler([=] (CDPPropertyBase* prop) { m_atomicEnableInputs[i] = *prop; });
  }
}

namespace {
void MapValue(std::atomic<int>& valueToChange, const CDPProperty<std::string>& prop,
              const std::map<std::string, int>& valueMap)
{
  auto it = valueMap.find(prop);
  if (it != valueMap.end())
    valueToChange = it->second;
}
}

void ADS1115IOServer::Configure(const char* componentXML)
{
  CDPComponent::Configure(componentXML);
  m_periodMs = 1000.0 / fs;
  MapValue(m_mappedDataRate, m_dataRate, DATA_RATE_MAP);
  MapValue(m_mappedGain, m_gain, GAIN_MAP);
  try
  {
    m_reader.Configure(m_adapter);
    CDPMessage("ADS1115IOServer: Configured adapter '%s'\n", m_adapter.c_str());
  }
  catch (const std::exception& e)
  {
    CDPMessage("ADS1115IOServer: Error when configuring adapter '%s': %s\n", m_adapter.c_str(), e.what());
  }
}

void ADS1115IOServer::Activate()
{
  CDPComponent::Activate();
  Start(GetPriority(), "ADS1115IOServer");
}

void ADS1115IOServer::Suspend()
{
  Stop();
  m_processEvent.Set();
  Stop(true);
  CDPComponent::Suspend();
}

void ADS1115IOServer::ProcessNull()
{
  m_processEvent.Set();
}

void I2CIO::ADS1115IOServer::Main()
{
  while (!Stopped())
  {
    try
    {
      ReadValues();
      if (m_commError)
      {
        m_commError = false;
        RunInComponentThread([=] () { m_transmissionAlarm.Clear(); });
      }
    }
    catch (std::exception const& e)
    {
      std::string messageCopy = e.what();
      if (!m_commError)
      {
        RunInComponentThread([=] ()
        {
          m_transmissionAlarm.Set();
          m_transmissionAlarm.SetAlarmText("ADS1115IOServer::Main: " + std::string(messageCopy));
        });
        m_commError = true;
      }
    }
    m_processEvent.Wait();
    m_processEvent.Reset();
  }
}

void ADS1115IOServer::ReadValues()
{
  std::array<short, INPUT_COUNT> values;
  for (int i = 0; i < INPUT_COUNT; ++i)
    if (m_atomicEnableInputs[i])
      values[i] = m_reader.Read(i, m_mappedGain, m_mappedDataRate);

  RunInComponentThread([=] ()
  {
    for (size_t i = 0; i < m_inputs.size(); ++i)
      if (m_atomicEnableInputs[i])
        m_inputs[i] = values[i];
  });
}

void I2CIO::ADS1115IOServer::SetPropertyHandler(CDPPropertyBase* prop)
{
  CDPComponent::SetPropertyHandler(prop);
  if (prop == &fs)
    m_periodMs = 1000.0 / fs;
}
