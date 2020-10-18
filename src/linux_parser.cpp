#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os,version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >>version>> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Read and return the system memory utilization
//Memory Utilization = 1.0 - (free_memory / (total_memory - buffers))
float LinuxParser::MemoryUtilization() { 

float MemTotal, MemFree, Buffers,value;
string label, line;
std::ifstream filestream (kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      //replace ":" with " "
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      //asign the value to three float variables 
      while (linestream >> label >> value) {
        if (label == "MemTotal") {
          MemTotal = value;
        }
        if (label == "MemFree") {
          MemFree = value;
        }
        if (label == "Buffers") {
          Buffers = value;
          break;
        }
      }
    }
  }
  return 1.0 - (MemFree / (MemTotal - Buffers));
}

// UpTime for the whole system
long LinuxParser::UpTime() { 

string line;
long uptime;
std::ifstream stream (kProcDirectory + kUptimeFilename);

  if (stream.is_open()){
    // get line in stream and asign it as line
    std::getline(stream, line);
    // declare a istringstream variabel named linestream with line
    std::istringstream linestream(line);
    linestream>>uptime;

  } 
return uptime;
}


// I did not use these functions so I commended them out
/*
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }
*/


// Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  vector<long> cpuValues;
  string line;
  string key;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >>iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
        if (key == "cpu") {
          cpuValues.push_back(user);
          cpuValues.push_back(nice);
          cpuValues.push_back(system);
          cpuValues.push_back(idle);
          cpuValues.push_back(iowait);
          cpuValues.push_back(irq);
          cpuValues.push_back(softirq);
          cpuValues.push_back(steal);
          cpuValues.push_back(guest);
          cpuValues.push_back(guest_nice);
          break;
        }
      }
    }
  }
  return cpuValues;
}


//return the total number of processes
int LinuxParser::TotalProcesses() { 
int value;
string label, line;
std::ifstream filestream (kProcDirectory + kStatFilename);

  if (filestream.is_open()) {

    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      //asign the value to three float variables 
      while (linestream >> label >> value) {
        if (label == "processes") {
          return value;
          break;
        }
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { int value;
string label, line;
std::ifstream filestream (kProcDirectory + kStatFilename);

  if (filestream.is_open()) {

    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      //asign the value to three float variables 
      while (linestream >> label >> value) {
        if (label == "procs_running") {
          return value;
          break;
        }
      }
    }
  }
  return 0;
}


string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}


string LinuxParser::Ram(int pid) { 

std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
string line, label, ram;
long value;

if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
          std::replace(line.begin(), line.end(), ':', ' ');
          std::istringstream linestream(line);
          
          while (linestream >> label >> value) {
            if (label == "VmSize") {
            return std::to_string(value*0.001);
            break;
          }
        }
      }
   }
  // KB->MB
  return "";
}


string LinuxParser::Uid(int pid) { 
    string line, label, value; 
    std::ifstream filestream(kProcDirectory + std::to_string(pid) +kStatusFilename);
    if (filestream.is_open()) {
    
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      //asign the value to three float variables 
      while (linestream >> label >> value) {
        if (label == "Uid") {
          
          return value;
          break;
        }
      }
    }
  }
    return "";
 }


string LinuxParser::User(int pid) {
  std::string line, key, value, uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          return key;
          break;
        }
      }
    }
  }
  return "";
}

//UpTime for a single processor
long LinuxParser::UpTime(int pid) {
  std::string line, value;
  std::vector<std::string> value_list;
  long time;
  std::ifstream filestream(kProcDirectory +std::to_string(pid) + kStatFilename);
      if (filestream.is_open()) {
          std::getline(filestream, line);
          std::stringstream sstream(line);
        //get the values inside the string variales and insert them in value_list
          while (std::getline(sstream, value, ' '))
          {
          value_list.push_back(value); 
         }
          time = std::stol(value_list[21]) / sysconf(_SC_CLK_TCK);
    }
  return time;
}

// this function returns the utime, stime, cutime, cstime values of a processor
vector<long> LinuxParser::TimeList(int pid) {
  std::string line, value;
  vector<long> time_list;
  long time_14, time_15, time_16, time_17;
  std::vector<std::string> value_list;
  std::ifstream filestream(kProcDirectory +std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::stringstream sstream(line);
    while (std::getline(sstream, value, ' ')) {
      value_list.push_back(value); 
    }

    time_14 = std::stol(value_list[13]) / sysconf(_SC_CLK_TCK);
    time_list.push_back(time_14);
    time_15 = std::stol(value_list[14]) / sysconf(_SC_CLK_TCK);
    time_list.push_back(time_15);
    time_16 = std::stol(value_list[15]) / sysconf(_SC_CLK_TCK);
    time_list.push_back(time_16);
    time_17 = std::stol(value_list[16]) / sysconf(_SC_CLK_TCK);
    time_list.push_back(time_17);
    }
  return time_list;
}

