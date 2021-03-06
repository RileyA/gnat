#include "gnat.h"

#include "util/json.h"

#include <ctype.h>
#include <exception>

namespace gnat {

// Increment, and throw an exception if we've read too far.
void SafeIncrement(const char** string) {
  ++(*string);
  if (**string == '\0')
    throw std::exception();
}
//---------------------------------------------------------------------------

void SafeMatch(String match, const char** string) {
  for (int i = 0; i < match.size(); ++i) {
    if (i != 0) SafeIncrement(string);
    if (match[i] != **string) throw std::exception();
  }
}
//---------------------------------------------------------------------------

String RemoveWhitespace(String in) {
  bool saw_quote = false;
  String out = "";
  for (int i = 0; i < in.size(); ++i) {
    if (in[i] == '\\') {
      out.push_back(in[i++]);
      if (i == in.size())
        break;
      out.push_back(in[i]);
      continue;
    }
    if (in[i] == '"')
      saw_quote = !saw_quote;
    if (!isspace(in[i]) || saw_quote)
      out.push_back(in[i]);
  }
  return out;
}
//---------------------------------------------------------------------------

String JsonValue::ReadString(const char** string) {
  if (**string == '\0' || **string != '"') throw std::exception();
  String out = "";
  SafeIncrement(string);
  while (**string != '"') {
    if (**string == '\\') {
      out.push_back(**string);
      SafeIncrement(string);
    }
    out.push_back(**string);
    SafeIncrement(string);
  }
  return out;
}
//---------------------------------------------------------------------------

scoped_ptr<JsonValue> JsonValue::Parse(String text) {
  text = RemoveWhitespace(text);
  const char* position = text.c_str();

  // Assume a json file always contains a top-level object.
  if (*position != '{')
    return scoped_ptr<JsonValue>(NULL);

  scoped_ptr<JsonValue> out = scoped_ptr<JsonValue>(ParseValue(&position));

  // If we had more content afterwards, we're invalid.
  if (out.get() && *position != '}') {
    return scoped_ptr<JsonValue>(NULL);
  }

  return out;
}
//---------------------------------------------------------------------------

JsonValue::~JsonValue() {
  if (type_ == kObject) {
    for (Map<String, JsonValue *>::iterator it = entries_.begin();
         it != entries_.end(); ++it)
      delete it->second;
  } else if (type_ == kArray) {
    for (int i = 0; i < v.size(); ++i)
      delete v[i];
  }
}
//---------------------------------------------------------------------------

bool JsonValue::Has(int idx) {
  DCHECK(type_ == kArray);
  return idx >= 0 && idx < v.size();
}
//---------------------------------------------------------------------------

bool JsonValue::Has(const char* key) {
  DCHECK(type_ == kObject);
  return entries_.count(String(key));
}
//---------------------------------------------------------------------------

JsonValue& JsonValue::operator[](int idx) {
  if (type_ == kArray) {
    DCHECK(idx >= 0 && idx < v.size());
    return *v[idx];
  }
  NOTREACHED(); return *this;
}
//---------------------------------------------------------------------------

JsonValue& JsonValue::operator[](String key) {
  DCHECK(Has(key.c_str()));
  if (type_ == kObject)
    return *(entries_[key]);
  NOTREACHED(); return *this;
}
//---------------------------------------------------------------------------

JsonValue& JsonValue::operator[](const char* key) {
  DCHECK(Has(key));
  if (type_ == kObject)
    return *(entries_[String(key)]);
  NOTREACHED(); return *this;
}
//---------------------------------------------------------------------------

JsonValue::operator bool() const {
  if (type_ == kNull)
    return false;
  if (type_ == kBoolean)
    return data_.b;
  return true;
}
//---------------------------------------------------------------------------

JsonValue::operator const char*() const {
  if (type_ == kString)
    return &s[0];
  NOTREACHED(); return NULL;
}
//---------------------------------------------------------------------------

size_t JsonValue::size() {
  if (type_ == kArray)
    return v.size();
  if (type_ == kObject)
    return entries_.size();
  NOTREACHED(); return 0;
}
//---------------------------------------------------------------------------

JsonValue::JsonValue(Type type, const char** string) {
  type_ = type;
  switch (type) {
  case kBoolean:
    ParseBoolean(string);
    break;
  case kString:
    ParseString(string);
    break;
  case kNull:
    ParseNull(string);
    break;
  case kArray:
    ParseArray(string);
    break;
  case kObject:
    ParseObject(string);
    break;
  case kNumber:
    ParseNumber(string);
    break;
  }
}
//---------------------------------------------------------------------------

void JsonValue::ParseObject(const char** string) {
  if (**string != '{') throw std::exception();
  SafeIncrement(string);
  while(**string != '}') {
    if (**string != '"') throw std::exception();
    String key = ReadString(string);
    if (**string != '"') throw std::exception();
    SafeIncrement(string);
    if (**string != ':') throw std::exception();
    SafeIncrement(string);
    JsonValue* value = ParseValue(string);
    if (!value) throw std::exception();
    entries_[key] = value;
    SafeIncrement(string);
    if (**string == ',')
      SafeIncrement(string);
  }
  if (**string != '}') throw std::exception();
}
//---------------------------------------------------------------------------

void JsonValue::ParseArray(const char** string) {
  DCHECK(**string == '[');
  SafeIncrement(string);
  data_.a = kNull;
  while(**string != ']') {
    JsonValue* value = ParseValue(string);
    if (!value)
      throw std::exception();
    if (v.empty())
      data_.a = value->type();
    else if (value->type() != data_.a)
      throw std::exception();
    v.push_back(value);
    SafeIncrement(string);
    if (**string == ',')
      SafeIncrement(string);
  }
  if (**string != ']') throw std::exception();
}
//---------------------------------------------------------------------------

void JsonValue::ParseBoolean(const char** string) {
  data_.b = (**string == 't');
  if (data_.b)
    SafeMatch("true", string);
  else
    SafeMatch("false", string);
}
//---------------------------------------------------------------------------

void JsonValue::ParseNull(const char** string) {
  SafeMatch("null", string);
}
//---------------------------------------------------------------------------

void JsonValue::ParseString(const char** string) {
  s = ReadString(string);
}
//---------------------------------------------------------------------------

void JsonValue::ParseNumber(const char** string) {
  // TODO make this more robust
  data_.d = atof(*string);
  while(**string != ',' && **string != ']' && **string != '}')
    SafeIncrement(string);
  --(*string);
}
//---------------------------------------------------------------------------

JsonValue* JsonValue::ParseValue(const char** string) {
  JsonValue* value = NULL;
  try {
    switch (**string) {
    case '{':
      value = new JsonValue(kObject, string);
      break;
    case '[':
      value = new JsonValue(kArray, string);
      break;
    case '"':
      value = new JsonValue(kString, string);
      break;
    case 'f':
    case 't':
      value = new JsonValue(kBoolean, string);
      break;
    case 'n':
      value = new JsonValue(kNull, string);
      break;
    default:
      value = new JsonValue(kNumber, string);
      break;
    }
  } catch(std::exception e) {
    value = NULL;
  }
  return value;
}
//---------------------------------------------------------------------------

}  // namespace gnat
