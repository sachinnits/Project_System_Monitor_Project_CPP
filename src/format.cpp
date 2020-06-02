#include <string>

#include "format.h"

using std::string;
using std::to_string;
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) {
  long HH = seconds / 60 / 60;
  long MM = (seconds / 60) % 60;
  long SS = seconds % 60;
  string elapsedTime = to_string(HH) + ":" + to_string(MM) + ":" + to_string(SS);
  return elapsedTime;
}