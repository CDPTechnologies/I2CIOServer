/**
 * (c)2016 CDP Technologies AS
 */

#ifndef I2CIOSERVER_H
#define I2CIOSERVER_H

#include <IO/IOServer.h>

class I2CIOServer : public IOServer
{
public:
  I2CIOServer();
  ~I2CIOServer() override;

  void Create(const char* fullName) override;
  void ProcessNull() override;
  void ProcessOffline() override;
  void ProcessOnline() override;
  bool IsCommProblem() override;
  void Activate() override;
  void Suspend() override;
  void FillNodeChildren(CDP::StudioAPI::NodeStream& serializer) const override;
  std::string GetNodeTypeName() const override;

protected:
  bool HandleXMLElement(XMLElementEx* pEx) override;

private:
  I2CIOServer(const I2CIOServer& rhs) = delete;
  const I2CIOServer& operator= (const I2CIOServer& rhs) = delete;

  class Private;
  std::unique_ptr<Private> d;
};

#endif
