#include "processor.h"


/*
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { return 0.0; }

*/

#include "linux_parser.h"
#include <numeric>
Processor::Processor() : total_time_pre(0.0), inactive_time_pre(0.0){};

float Processor::Utilization() { 
float inactive_time, total_time, utilization;
std::vector<long> cpuValues;
cpuValues =  LinuxParser::CpuUtilization();

// keys: 0:user, 1:nice, 2:system, 3:idle, 4:iowait
// 5:irq, 6:softirq, 7:steal, 8:guest, 9:guest_nice
inactive_time = cpuValues[3]+cpuValues[4];
total_time = std::accumulate(cpuValues.begin(), cpuValues.end(),0);

utilization = 1 - (inactive_time-inactive_time_pre)/(total_time-total_time_pre);

inactive_time_pre = inactive_time;
total_time_pre = total_time;

return utilization;

 }