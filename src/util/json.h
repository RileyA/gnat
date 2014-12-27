#ifndef gnat_util_json_H
#define gnat_util_json_H

#include "gnat.h"

#include "util/scoped_ptr.h"

namespace gnat {

// A simple and not-very-robust JSON parser. Parsing with Parse() returns
// an instance of this object, and you can reach any child values that it
// may have through the getters below.
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

  static scoped_ptr<JsonValue> Parse(String text);

  ~JsonValue();

  /////////////////////////////////////////////////////////////////////
  // Functions for arrays and/or objects, calling with something that
  // isn't an array or object is undefined.

  // If we're an array, get whether we have an index.
  bool Has(int idx);

  // If we're an object, get whether we have a given key.
  bool Has(const char* key);

  // Bracket getters for objects/arrays.
  JsonValue& operator[](int idx);
  JsonValue& operator[](String key);
  JsonValue& operator[](const char* key);

  // Length of array or object.
  size_t size();

  /////////////////////////////////////////////////////////////////////
  // Primitive getters

  // Implicit conversion operators.
  operator bool() const;
  operator const char*() const;

  Type type() { return type_; }

  String string() {
    DCHECK(type_ == kString);
    return s;
  }

  int integer() {
    DCHECK(type_ == kNumber);
    return round(data_.d);
  }

  double floating() {
    DCHECK(type_ == kNumber);
    return data_.d;
  }

  /////////////////////////////////////////////////////////////////////

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
