#ifndef gnat_util_timer_H
#define gnat_util_timer_H

#include "gnat.h"
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

namespace gnat {

class Timer {
 public:
  Timer();

  void Start();
  void Stop();

  Real GetTimeSeconds();
  Real GetTimeMilliseconds();

 private:

  bool timing_;
  Real elapsed_;

#ifdef WIN32
  // TODO
#else
  struct timeval start_;
#endif
};
  

}  // nmamespace gnat

#endif  // gnat_util_timer_H
