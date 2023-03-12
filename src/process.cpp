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

Process::Process(int pid) { 
    this->m_PID         = pid; 
    this->m_RAM         = LinuxParser::Ram(pid);
    this->m_Command     = LinuxParser::Command(pid);
    this->m_User        = LinuxParser::User(pid);
    this->m_UpTime      = LinuxParser::UpTime(pid);
    this->m_Utilization =  0;
}

// TODO: Return this process's ID
int Process::Pid() { return this->m_PID; }

// TODO: Return this process's CPU utilization
long Process::CpuUtilization() { 
    long systemUptime       = LinuxParser::UpTime();
    long ProcesUptime       = LinuxParser::UpTime(m_PID);
    long TotalTimeActive    = LinuxParser::ActiveJiffies(m_PID);

    this->m_Utilization = TotalTimeActive/(systemUptime - ProcesUptime);

    return this->m_Utilization; 
}

// TODO: Return the command that generated this process
string Process::Command() { return this->m_Command; }

// TODO: Return this process's memory utilization
string Process::Ram() { return this->m_RAM; }

// TODO: Return the user (name) that generated this process
string Process::User() { return this->m_User; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return this->m_UpTime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return this->m_Utilization < a.m_Utilization;
}