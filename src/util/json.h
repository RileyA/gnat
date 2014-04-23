#ifndef gnat_util_json_H
#define gnat_util_json_H

#include "gnat.h"

namespace gnat {

class JsonValue {
 public:
  enum Type {
    kString,
    kNumber,
    kBoolean,
    kNull,
    kObject,
    kArray,
  };

  static JsonValue* Parse(String text);

  ~JsonValue();

  // Bracket getters for objects/arrays.
  JsonValue& operator[](int idx);
  JsonValue& operator[](String key);
  JsonValue& operator[](const char* key);
  size_t size();

  // Implicit conversion operators.
  operator bool() const;
  operator const char*() const;

  Type type() { return type_; }

  static String RemoveWhitespace(String in);
  static String ReadString(const char** string);

 private:
  JsonValue(Type type, const char** string);

  void ParseObject(const char** string);
  void ParseArray(const char** string);
  void ParseBoolean(const char** string);
  void ParseNull(const char** string);
  void ParseString(const char** string);
  void ParseNumber(const char** string);

  static JsonValue* ParseValue(const char** string);

  Type type_;

  // Some combination of the below will be used depending on |type_|.
  union {
    bool b;
    double d;
    Type a;
  } data_;
  String s;
  Vector<JsonValue*> v;
  Map<String, JsonValue*> entries_;
};

}  // namespace gnat

#endif
