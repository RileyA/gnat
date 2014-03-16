#include "gnat.h"
#include "util/timer.h"

namespace gnat {

Timer::Timer()
  : timing_(false),
    elapsed_(0.0) {}
//---------------------------------------------------------------------------

void Timer::Start() {
  gettimeofday(&start_, 0);
  timing_ = true;
}
//---------------------------------------------------------------------------

void Timer::Stop() {
  DCHECK(timing_);
  elapsed_ = GetTimeMilliseconds();
  timing_ = false;
}
//---------------------------------------------------------------------------

Real Timer::GetTimeSeconds() {
  return timing_ ? (GetTimeMilliseconds() * 1000.0) : elapsed_;
}
//---------------------------------------------------------------------------

Real Timer::GetTimeMilliseconds() {
  if (!timing_)
    return elapsed_;
  struct timeval now;
  gettimeofday(&now, 0);
  long seconds  = now.tv_sec  - start_.tv_sec;
  long useconds = now.tv_usec - start_.tv_usec;
  return (seconds * 1000.f + useconds / 1000.f);
}
//---------------------------------------------------------------------------

}  // namespace gnat
