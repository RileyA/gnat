#include "gnat.h"

#include "util/json.h"

#include <ctype.h>
#include <exception>

namespace gnat {

Json::Value::Value(Json::Type type) : type_(type) {}

Json::Value::Value(Type type, const char** string) {
  type_ = type;
  switch (type) {
  case kBoolean:
    value_.b = (**string == 't');
    if (value_.b)
      *string += 3;
    else
      *string += 4;
    break;
  case kString:
    s = ReadString(string);
    break;
  case kNull:
    *string += 3;
    break;
  case kArray:
    DCHECK(**string == '[');
    ++(*string);
    value_.array_type_ = kNull;
    while(**string != ']') {
      Value* value = ParseValue(string);
      if (!value)
        throw std::exception();
      if (values_.empty())
        value_.array_type_ = value->type();
      else if (value->type() != value_.array_type_)
        throw std::exception();
      values_.push_back(value);
      ++(*string);
      if (**string == ',')
        ++(*string);
    }
  case kNumber:
    // TODO
    break;
  }
}

Json::Value::~Value() {}

Json::Object::Object(const char** string)
  : Json::Value(kObject) {
  if (**string != '{')
    throw std::exception();
  ++(*string);
  while(**string != '}') {
    if (**string != '"')
      throw std::exception();
    String key = ReadString(string);
    if (**string != '"')
      throw std::exception();
    ++(*string);
    if (**string != ':')
      throw std::exception();
    ++(*string);

    Value* value = ParseValue(string);
    if (!value)
      throw std::exception();

    values_[key] = value;
    ++(*string);
    if (**string == ',')
      ++(*string);
  }
}

String Json::RemoveWhitespace(String in) {
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

String Json::ReadString(const char** string) {
  DCHECK(**string == '"');
  String out = "";
  ++(*string);
  while (**string != '"') {
    if (**string == '\\') {
      out.push_back(**string);
      ++(*string);
    }
    out.push_back(**string);
    ++(*string);
  }
  return out;
}

Json::Object* Json::Parse(String text) {
  text = RemoveWhitespace(text);
  const char* position = text.c_str();

  // Assume a json file always contains a top-level object.
  if (*position != '{')
    return NULL;

  Json::Object* obj;
  try {
    // Parse the main object
    obj = new Json::Object(&position);
  } catch(std::exception e) {
    return NULL;
  }
  
  // If we had more content afterwards we're invalid.
  if (*position != '}') {
    delete obj;
    return NULL;
  }

  return obj;
}

Json::Value* Json::ParseValue(const char** string) {
  Json::Value* value = NULL;
  // Now for the value
  switch (**string) {
  case '{':
    value = new Object(string);
    break;
  case '[':
    value = new Value(kArray, string);
    break;
  case '"':
    value = new Value(kString, string);
    break;
  case 'f':
    if (*(*string + 1) != 'a' || 
        *(*string + 2) != 'l' ||
        *(*string + 3) != 's' ||
        *(*string + 4) != 'e') {
      throw std::exception();
    } else {
      value = new Value(kBoolean, string);
    }
    break;
  case 't':
    if (*(*string + 1) != 'r' || 
        *(*string + 2) != 'u' ||
        *(*string + 3) != 'e') {
      throw std::exception();
    } else {
      value = new Value(kBoolean, string);
    }
    break;
  case 'n':
    if (*(*string + 1) != 'u' || 
        *(*string + 2) != 'l' ||
        *(*string + 3) != 'l')
      throw std::exception();
    else
      value = new Value(kNull, string);
    break;
  default:
    // number
    if (**string == '-' || (**string >= '0' && **string <= '9'))
      value = new Value(kNumber, string);
    else
      throw std::exception();
    break;
  }
  return value;
}

}

