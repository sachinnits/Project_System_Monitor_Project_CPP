#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;


//it is a constructor fucntion 
Process::Process(int pid){
    this->pid = pid;
}
// function to Return this process's ID
int Process::Pid() { 
  return pid;
}

// function to Return this process's CPU utilization
float Process::CpuUtilization() { return LinuxParser::CpuUtilization(pid);  }

// function to Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid);}

// function to Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// function to Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// function to Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// function to Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const
{ return true; }
