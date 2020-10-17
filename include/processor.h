#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  
  Processor();

 private:
 float total_time_pre;
 float inactive_time_pre;
};

#endif