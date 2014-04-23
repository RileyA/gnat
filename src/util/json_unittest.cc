#include "gtest/gtest.h"
#include "util/json.h"
#include "util/scoped_ptr.h"

namespace gnat {

  TEST(JsonTest, RemoveWhitespace) {
    String str = "{ \"hello world\"\t\n\\\"  a  \\\" } ";
    String expected = "{\"hello world\"\\\"a\\\"}";
    EXPECT_EQ(expected, JsonValue::RemoveWhitespace(str));
  }

  TEST(JsonTest, ReadString) {
    String str = "\"-\\\"-\"";
    const char* s = str.c_str();
    EXPECT_EQ(String("-\\\"-"), JsonValue::ReadString(&s));
  }

  TEST(JsonTest, ReadEmpty) {
    String str = "{}";
    scoped_ptr<JsonValue> value = JsonValue::Parse(str);
    EXPECT_EQ(value->size(), 0);
  }

  TEST(JsonTest, ReadSimple) {
    String str = "{\"abc\":true}";
    scoped_ptr<JsonValue> value = JsonValue::Parse(str);
    EXPECT_EQ(value->size(), 1);
    EXPECT_EQ((*value)["abc"], true);
  }

  TEST(JsonTest, ReadStrings) {
    String str = "{\"abc\":\"foo\",\"def\":\"bar\"}";
    scoped_ptr<JsonValue> value = JsonValue::Parse(str);
    EXPECT_EQ(value->size(), 2);
    EXPECT_EQ(0, strcmp("foo", (*value)["abc"]));
    EXPECT_EQ(0, strcmp("bar", (*value)["def"]));
  }

  TEST(JsonTest, ReadArrayOfStrings) {
    String str = "{\"abc\" : [\"foo\",\"bar\"]}";
    scoped_ptr<JsonValue> value = JsonValue::Parse(str);
    EXPECT_EQ(value->size(), 1);
    EXPECT_EQ((*value)["abc"].size(), 2);
    EXPECT_EQ(0, strcmp("foo", (*value)["abc"][0]));
    EXPECT_EQ(0, strcmp("bar", (*value)["abc"][1]));
  }

  TEST(JsonTest, ReadInvalid) {
    String str = "{\"abc\" : [\"foo\",\"bar\", false]}";
    scoped_ptr<JsonValue> value = JsonValue::Parse(str);
    EXPECT_FALSE(value.get());
  }
}
