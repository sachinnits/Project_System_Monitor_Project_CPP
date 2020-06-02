#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using namespace std;//CHANGE SUGGESTED BY THE LAST UDACITY REVIEWER

System::System(){
  cpu_ = Processor();
}
// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  refresh_Proccesses_List();
  return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

void System::refresh_Proccesses_List(){
    processes_.clear();
    vector<int> problem_ids = LinuxParser::Pids();
    for(int problem_id: problem_ids){
      /*CHANGE SUGGESTED BY THE LAST UDACITY REVIEWER
Try using emplace_back instead of push_back, it is much more efficient than push_back.
push_back constructs a temporary object which then will need to get moved into the vector v whereas emplace_back just forwards the argument and construct it directly in place with no copies or moves needed.
In short, it is a good habit if you are using emplace_back instead of push_back
      */
        processes_.emplace_back(Process(problem_id));
    }
}