#ifndef gnat_message_any_H
#define gnat_message_any_H

#include "gnat.h"

namespace gnat {

/** A nearly abstract Message class, used to package a MessageAny<> */
class Message {
 public:
  virtual ~Message() {}
};
//---------------------------------------------------------------------------

/** A Message holding some arbitrary data */
template<class T> class MessageAny : public Message {
public:
  T data;
  MessageAny(T _data) : data(_data) {}
};
//---------------------------------------------------------------------------

template<class T>
const MessageAny<T>* message_any_cast(const Message& msg) {
  if(const MessageAny<T>* message = dynamic_cast<const MessageAny<T>* >(&msg))
    return message;
  else
    return 0;
}
//---------------------------------------------------------------------------

// This requires that |msg| has valid data.
template<class T>
T message_cast(const Message& msg) {
  const MessageAny<T>* message =
     dynamic_cast<const MessageAny<T>* >(&msg)
  DCHECK(message);
  return message->data;
}

} // namespace gnat

#endif

