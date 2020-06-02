#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  // TODO: Declare any necessary private members
  Processor();//custom defalut constructor
  long prev_idle;//time for which cpu is idle
  long prev_total;//total time for cpu utilization
 private:
};

#endif