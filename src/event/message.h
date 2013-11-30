#ifndef gnat_message_any_H
#define gnat_message_any_H

#include "gnat.h"

namespace gnat {

/** A nearly abstract Message class, used to package a MessageAny<> */
class Message {
 public:
  virtual ~Message() {}
};

/** A Message holding some arbitrary data */
template<class T> class MessageAny : public Message
{
public:
  T data;
  MessageAny(T _data) : data(_data) {}
};

} // namespace gnat

#endif

