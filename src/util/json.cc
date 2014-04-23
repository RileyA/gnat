#include "gnat.h"

#include "util/json.h"

#include <ctype.h>
#include <exception>

namespace gnat {

const char** safe_increment(const char** string) {
  ++(*string);
  if (**string == '\0')
    throw std::exception();
}
//---------------------------------------------------------------------------

void safe_match(String match, const char** string) {
  for (int i = 0; i < match.size(); ++i) {
    if (i != 0) safe_increment(string);
    if (match[i] != **string) throw std::exception();
  }
}
//---------------------------------------------------------------------------

JsonValue* JsonValue::Parse(String text) {
  text = RemoveWhitespace(text);
  const char* position = text.c_str();

  // Assume a json file always contains a top-level object.
  if (*position != '{')
    return NULL;

  JsonValue* out = ParseValue(&position);

  // If we had more content afterwards, we're invalid.
  if (out && *position != '}') {
    delete out;
    return NULL;
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

JsonValue& JsonValue::operator[](int idx) {
  if (type_ == kArray) {
    DCHECK(idx >= 0 && idx < v.size());
    return *v[idx];
  }
  NOTREACHED(); return *this;
}
//---------------------------------------------------------------------------

JsonValue& JsonValue::operator[](String key) {
  if (type_ == kObject)
    return *(entries_[key]);
  NOTREACHED(); return *this;
}
//---------------------------------------------------------------------------

JsonValue& JsonValue::operator[](const char* key) {
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
  NOTREACHED(); return false;
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
  safe_increment(string);
  while(**string != '}') {
    if (**string != '"') throw std::exception();
    String key = ReadString(string);
    if (**string != '"') throw std::exception();
    safe_increment(string);
    if (**string != ':') throw std::exception();
    safe_increment(string);
    JsonValue* value = ParseValue(string);
    if (!value) throw std::exception();
    entries_[key] = value;
    safe_increment(string);
    if (**string == ',')
      safe_increment(string);
  }
  if (**string != '}') throw std::exception();
}
//---------------------------------------------------------------------------

void JsonValue::ParseArray(const char** string) {
  DCHECK(**string == '[');
  safe_increment(string);
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
    safe_increment(string);
    if (**string == ',')
      safe_increment(string);
  }
  if (**string != ']') throw std::exception();
}
//---------------------------------------------------------------------------

void JsonValue::ParseBoolean(const char** string) {
  data_.b = (**string == 't');
  if (data_.b)
    safe_match("true", string);
  else
    safe_match("false", string);
}
//---------------------------------------------------------------------------

void JsonValue::ParseNull(const char** string) {
  safe_match("null", string);
}
//---------------------------------------------------------------------------

void JsonValue::ParseString(const char** string) {
  s = ReadString(string);
}
//---------------------------------------------------------------------------

void JsonValue::ParseNumber(const char** string) {
  // TODO make this more robust
  data_.d = atof(*string);
  while(**string != ',' && **string != ']', **string != '}')
    safe_increment(string);
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
      value = new JsonValue(kBoolean, string);
      break;
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

String JsonValue::RemoveWhitespace(String in) {
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
  safe_increment(string);
  while (**string != '"') {
    if (**string == '\\') {
      out.push_back(**string);
      safe_increment(string);
    }
    out.push_back(**string);
    safe_increment(string);
  }
  return out;
}
//---------------------------------------------------------------------------

}

