#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include"linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//Constructor
Process::Process(int pid){
      pid_ = pid;
};

// Return this process's ID
// this is a JUST a getter function!
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
// The eqution for this calulation comes from:
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float Process::CpuUtilization() {

    std::vector<long> cpuValues;

    float utime, stime, cutime, cstime, starttime, uptime, totaltime, seconds;
    
    cpuValues =  LinuxParser::TimeList(pid_);
    //keys: 0>>utime , 1>>stime , 2>>cutime , 3>>cstime 
    
    utime = cpuValues[0];
    stime = cpuValues[1];
    cutime = cpuValues[2];
    cstime = cpuValues[3];
  
    uptime = LinuxParser::UpTime();
    starttime = LinuxParser::UpTime(pid_);

    totaltime = utime + stime + cutime + cstime;
    seconds = uptime - starttime;
    return (totaltime/seconds);
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_);}

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_);}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& other) const { 
    long myRam = std::stol(LinuxParser::Ram(pid_));
    long otherRam = std::stol(LinuxParser::Ram(other.pid_));
    return myRam < otherRam;
}