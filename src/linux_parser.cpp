#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include<stdio.h>
#include<ctype.h>

#include "linux_parser.h"

using std::atoi;  // convert string to int
using std::atol;  // convert to long
using std::stof;  // convert to float
using std::stol;
using std::stoi;
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
        /*
         PRETTY_NAME=
           A pretty operating system name in a format suitable for presentation to the user. May
           or may not contain a release code name or OS version of some kind, as suitable. If not
           set, defaults to "PRETTY_NAME="Linux"". Example: "PRETTY_NAME="Fedora 17 (Beefy
           Miracle)"".
        */
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
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
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

// function to Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  /*
  string memor_Total = "MemTotal:";
  string memor_Free = "MemFree:";
  float memory_Total = findValueByKey<float>(memTotal, kMeminfoFilename);// "/proc/memInfo"
  float Free = findValueByKey<float>(memFree, kMeminfoFilename);
  return (Total - Free) / Total;
  */
  string memory_key;//string to store the key of memory
  string memory_total;//string to store the total memory being utilised by a program
  string memory_free;//string to store the memory that is free post utilisation of programs
  string kb;//string kb denotes kilo bytes unit of mesaurement for storage
  string line;//string to store the line that we will get from the stream
  //create a input file stream from the path for proc directory + the os memory usage 
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  //to check that we are able to open the stream
  if (stream.is_open()) {
    // Get Total Memory
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> memory_key >> memory_total >> kb;

    // Get Free Memory
    std::getline(stream, line);
    linestream.str(line);
    linestream.clear();
    linestream >> memory_key >> memory_free >> kb;
  }
  //stof is used for conversion of string type to float type
  //Memory used = total memory - free memory(post utilisation)
  float memory_tot = 0.0f;
  if(memory_total!="")
  {
      memory_tot = stof(memory_total);
  }
  float memory_fr = 0.0f;
  if(memory_free!="")
  {
      memory_fr = stof(memory_free);
  }
  float memory_used = memory_tot - memory_fr;
  // Percentage used = (total memory - free memory(post utilisation)) / total memory
  if(memory_tot!=0.0f)
  {
      float percentage_memory_used = memory_used / memory_tot;
     return percentage_memory_used;
  }
  else
    return memory_tot;
}

// function to Read and return the system uptime
long LinuxParser::UpTime() { 
  string up_time;//to store the uptime of the system
  string idle_time;//time for which the system has been idle
  string line;//string to store the line that we will get from the stream
  //create a input file stream from the path for proc directory + the up time directory 
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  //to check that we are able to open the stream
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>up_time>>idle_time;
  }
  //stol is used for conversion of string type to long type
  if(up_time!="")
     return stol(up_time);
  else{
      long temp = 0;
      return temp;
  }
 
}

// function to Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  //Its only usage is to store the number of ticks occurred since system start-up.
  vector<string> jiffies = CpuUtilization();
  long total_jiffies_since_boot = 0;
  for (auto s : jiffies) {
    total_jiffies_since_boot += atol(s.c_str());
  }
  return total_jiffies_since_boot;
  //return 0;
}

// function to Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  string line;//string to store the line that we will get from the stream
  string var;
  string utime; // CPU time spent in user code, measured in clock ticks
  string stime; //CPU time spent in kernel code, measured in clock ticks
  string cutime; //Waited-for children's CPU time spent in user code (in clock ticks)
  string cstime; //Waited-for children's CPU time spent in kernel code (in clock ticks)
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  //to check that we are able to open the stream
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for(int i = 1; i < 14; i++) {
        linestream >> var; // Stop at 13th token
      }
      linestream >> utime >> stime >> cutime >> cstime; // Grab active jiffies
      //stol is used for conversion of string type to long type
      long totaltime = stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
      return totaltime;
    }
  }
	return -1; // if not return -1
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// function to Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
  //time for which the system has been idle + time spent waiting in CPU for I/O
  long idle_jiffies_since_boot = atol(jiffies[3].c_str());

  return idle_jiffies_since_boot;
}

// function to Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  
  vector<string> v{};
  string user; 
  /*User :The CPU is running code in user-mode. This includes your application code. Note that if an application tries to read from disk or write to network, it actually goes to sleep while the kernel performs that work, and wakes up the application again.*/
  string nice;//The user code can be executed in “normal” priority, or various degrees of “below normal” priority.
  string system;//The CPU is running kernel code. This includes device drivers and kernel modules.
  string idle;//time for which the system has been idle
  string iowait;//time spent by a process waiting for I/O in CPU
  //IRQ and SoftIRQ: The kernel is servicing interrupt requests (IRQs).
  string irq,softirq;
  string steal;//When running in a virtualized environment, the hypervisor may “steal” cycles that are meant for your CPUs and give them to another, for various reasons. This time is accounted for as steal.
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }
  v.push_back(user);
  v.push_back(nice);
  v.push_back(system);
  v.push_back(idle);
  v.push_back(iowait);
  v.push_back(irq);
  v.push_back(softirq);
  v.push_back(steal);

  return v;
}

// function to Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string name = "processes";
  string key, value;//key-value pair 
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      break;
    }
  }
  return atoi(value.c_str());
}

// function to Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  
  string key, value;//key-value pair
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "procs_running"){
        stream.close();
        return stoi(value);
      }
    }
  }
  return 0;
}
//function to return the amount of cpu utilised by a particular pid
float LinuxParser::CpuUtilization(int pid) {
  string line;
  string value;
  float result;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  getline(stream, line);
  string str = line;
  std::istringstream buf(str);
  std::istream_iterator<string> beg(buf), end;
  vector<string> values(beg, end);  // done!
  // acquiring relevant times for calculation of active occupation of CPU for
  // selected process
  float utime = (float)UpTime(pid);// CPU time spent in user code, measured in clock ticks
  float stime = 0.0f;//CPU time spent in kernel code, measured in clock ticks
  if(values[14]!=""){
      stime = stof(values[14]);
  }
  //float cutime = stof(values[15]);//Waited-for children's CPU time spent in user code (in clock ticks)
  //float cstime = stof(values[16]);//Waited-for children's CPU time spent in kernel code (in clock ticks)
  float starttime = 0.0f;
  if(values[21]!=""){
      starttime = stof(values[21]);
  }
  float uptime = UpTime();  // system uptime
  float freq = sysconf(_SC_CLK_TCK);//The sysconf(_SC_CLK_TCK) C++ function call may also be used to return the hertz value.
  float total_time = utime + stime;
  float seconds = uptime - (starttime / freq);
  result = ((total_time / freq) / seconds);
  return result;
}
// function to Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string command;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, command);
  }
  return command;
}

// function to Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string name = "VmSize";
  string key, value;//key-value pair 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      break;
    }
  }
  //Alternative Implementation(CHange suggested by previous reviewer) deleting last 3 characters was his idea
//Since variable value is of type string so to simplify things you can directly delete the last three characters instead of 
//dividing by 1024 (~1000) but you should note that here you are compromising with accuracy!
  if(value!=""){
      value = to_string((stof(value.c_str())/1000));
      return value.substr(0, 6);
  }
  else
  {
    return value;
  }
}

// function to Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string name = "Uid";
  string key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      break;
    }
  }
  return value;
}

// function to Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string name = "x:" + Uid(pid);
  string title, value;
  std::ifstream stream(kPasswordPath);
  string line;
  while (std::getline(stream, line)) {
    if (line.find(name) != std::string::npos) {
      value = line.substr(0, line.find(":"));
    }
  }
  return value;
}

// function to Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  std::istream_iterator<string> beg(linestream), end;
  vector<string> values(beg, end);
  /* CHANGE SUGGESTED BY LAST UDACITY REVIEWER
  The 22nd value that you are grabbing is the time the process started after system boot
That means in order to get the unit of time it has been running since start you need to subtract it from the UpTime() of the system and so you need to do as follows:

int upTimePid = UpTime() - stol(var)/sysconf(_SC_CLK_TCK);
return upTimePid;
  */
  //The sysconf(_SC_CLK_TCK) C++ function call may also be used to return the hertz value.
  long seconds = UpTime() - stol(values[21]) / sysconf(_SC_CLK_TCK);
  return seconds;
}