#ifndef gnat_util_json_H
#define gnat_util_json_H

#include "gnat.h"

namespace gnat {

class Json {
 public:

  enum Type {
    kString,
    kNumber,
    kBoolean,
    kNull,
    kObject,
    kArray,
  };

  class Value {
   public:
    Value(Type type);
    Value(Type type, const char** string);
    virtual ~Value();
    Type type() { return type_; }

    virtual Value& operator[](int idx) {
      if (type_ == kArray) {
        DCHECK(idx >= 0 && idx < values_.size());
        return *values_[idx];
      }
      NOTREACHED();
      return *this;
    }

    virtual size_t NumElements() {
      if (type_ == kArray)
        return values_.size();
      NOTREACHED();
      return 0;
    }

    operator bool() const {
      if (type_ == kNull)
        return false;
      if (type_ == kBoolean)
        return value_.b;
    }

    operator const char*() const {
      if (type_ == kString)
        return &s[0];
      else
        NOTREACHED();
    }

   private:
    Type type_;
    union {
      bool b;
      int i;
      double d;
      Type array_type_;
    } value_;
    String s;
    Vector<Value*> values_;

    friend class Json;
  };

  class Object : public Value {
   public:
    Object(const char** string);
    size_t NumElements() { return values_.size(); }
    virtual Value& operator[](String key) {
      return *(values_[key]);
    }
    Value& operator[](const char* key) {
      return *(values_[String(key)]);
    }
   private:
    Map<String, Value*> values_;
    friend class Json;
  };

  static String RemoveWhitespace(String in);
  static String ReadString(const char** string);
  static Value* ParseValue(const char** string);

  // Parse a JSON string, returns NULL if invalid or empty. The result is
  // heap-allocated and the caller is responsible for cleanup.
  static Object* Parse(String text);

};

}  // namespace gnat

#endif
