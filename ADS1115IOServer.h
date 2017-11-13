/**
 * (c)2017 CDP Technologies AS
 */

#ifndef I2CIO_ADS1115IOSERVER_H
#define I2CIO_ADS1115IOSERVER_H

#include <CDPSystem/Base/CDPComponent.h>

#include "ADS1115Reader.h"
#include <CDPAlarm/CDPAlarm.h>

namespace I2CIO {

class ADS1115IOServer : public CDPComponent, public OSAPIThread
{
public:
  ADS1115IOServer();
  ~ADS1115IOServer() = default;

private:
  ADS1115IOServer(const ADS1115IOServer& rhs) = delete;
  const ADS1115IOServer& operator=(const ADS1115IOServer& rhs) = delete;

  void Create(const char* fullName) override;
  void Configure(const char* componentXML) override;
  void Activate() override;
  void Suspend() override;
  void ProcessNull() override;
  void Main() override;
  void ReadValues();
  void SetPropertyHandler(CDPPropertyBase* prop) override;

  CDPProperty<std::string> m_adapter;
  CDPProperty<std::string> m_dataRate;
  CDPProperty<std::string> m_gain;
  CDPAlarm m_transmissionAlarm;
  std::vector<CDPSignal<short>> m_inputs;
  std::vector<CDPProperty<bool>> m_enableInputs;
  std::vector<std::atomic<bool>> m_atomicEnableInputs;
  ADS1115Reader m_reader;
  OSAPIEvent m_processEvent;
  std::atomic<int> m_mappedDataRate;
  std::atomic<int> m_mappedGain;
  std::atomic<int> m_periodMs;
};

}

#endif
