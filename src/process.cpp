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


Process::Process(int pid)
{
    this->m_PID         = pid;
    this->m_Command     = LinuxParser::Command(pid);
    this->m_RAM         = LinuxParser::Ram(pid);
    this->m_User        = LinuxParser::User(pid);
    this->m_UpTime      = LinuxParser::UpTime(pid);
}

// TODO: Return this process's ID
int Process::Pid() { return this->m_PID; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

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
     return true; 
}