#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include "format.h"

using namespace std;


Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
    processes_  = {};
    int RAM     = 0;
    const vector<int> &Pids = LinuxParser::Pids();

    for(const int &pid : Pids)
    {
        RAM = LinuxParser::GetRam(pid);
        if(RAM > 0)
        {
            processes_.emplace_back(pid); 
        }
    }
    std::sort(processes_.begin(),processes_.end(),std::greater<Process>());

    return processes_; 
}

std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
}

int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() { 
    return (long int)(LinuxParser::UpTime());
}
