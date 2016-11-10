/**
 * (c)2016 CDP Technologies AS
 */

#ifndef I2CHELPERS_H
#define I2CHELPERS_H

#include <StudioAPI/NodeStream.h>

#include <CDPSystem/Base/CDPProperty.h>

#include <CDPDefines.h>

#include <string>

template <typename T>
static const char* c_str(const T& t)
{
  return static_cast<const std::string&>(t).c_str();
}

template <typename ... T>
static void MessageLine(const std::string& format, const T& ... args)
{
  CDPMessage((format + '\n').c_str(), args ...);
}

template <typename T>
static void ConfigureProperty(
    CDPProperty<T>& property, const std::string& name,
    CDPBaseObject* owner, XMLElementEx* element)
{
  const std::string NameAttribute = "Name";

  auto prefix = element->GetAttributeValue(NameAttribute);
  property.Create(prefix.append(".").append(name).c_str(), owner);
  property.Configure(element->FindAttribute(name));
  property.AddNodeModeFlags(CDP::StudioAPI::eValueIsReadOnly);
  property.SetNodeReparented(true);
}

#endif
