#include "gtest/gtest.h"
#include "util/json.h"

namespace gnat {

  TEST(JsonTest, RemoveWhitespace) {
    String str = "{ \"hello world\"\t\n\\\"  a  \\\" } ";
    String expected = "{\"hello world\"\\\"a\\\"}";
    EXPECT_EQ(expected, Json::RemoveWhitespace(str));
  }

  TEST(JsonTest, ReadString) {
    String str = "\"-\\\"-\"";
    const char* s = str.c_str();
    EXPECT_EQ(String("-\\\"-"), Json::ReadString(&s));
  }

  TEST(JsonTest, ReadEmpty) {
    String str = "{}";
    Json::Object* obj = Json::Parse(str);
    EXPECT_EQ(obj->NumElements(), 0);
    delete obj;
  }

  TEST(JsonTest, ReadSimple) {
    String str = "{\"abc\":true}";
    Json::Object* obj = Json::Parse(str);
    EXPECT_EQ(obj->NumElements(), 1);
    EXPECT_EQ((*obj)["abc"], true);
    delete obj;
  }

  TEST(JsonTest, ReadStrings) {
    String str = "{\"abc\":\"foo\",\"def\":\"bar\"}";
    Json::Object* obj = Json::Parse(str);
    EXPECT_EQ(obj->NumElements(), 2);
    EXPECT_EQ(0, strcmp("foo", (*obj)["abc"]));
    EXPECT_EQ(0, strcmp("bar", (*obj)["def"]));
    delete obj;
  }

  TEST(JsonTest, ReadArrayOfStrings) {
    String str = "{\"abc\" : [\"foo\",\"bar\"]}";
    Json::Object* obj = Json::Parse(str);
    EXPECT_EQ(obj->NumElements(), 1);
    EXPECT_EQ((*obj)["abc"].NumElements(), 2);
    EXPECT_EQ(0, strcmp("foo", (*obj)["abc"][0]));
    EXPECT_EQ(0, strcmp("bar", (*obj)["abc"][1]));
    delete obj;
  }

  TEST(JsonTest, ReadInvalid) {
    String str = "{\"abc\" : [\"foo\",\"bar\", false]}";
    Json::Object* obj = Json::Parse(str);
    EXPECT_FALSE(obj);
  }
}
