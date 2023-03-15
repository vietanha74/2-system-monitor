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
    this->m_Utilization =  0.0;
    
}

int Process::Pid() { return this->m_PID; }

// Calculate CpuUtilization of process by Pid
float Process::CpuUtilization() { 
    long ProcesUptime       = LinuxParser::UpTime(m_PID);
    long TotalTimeActive    = LinuxParser::ActiveJiffies(m_PID);

    this->m_Utilization = float(TotalTimeActive)/float(ProcesUptime);

    return this->m_Utilization; 
}

//Get Command
string Process::Command() { return this->m_Command; }

//Get Ram
string Process::Ram() { return this->m_RAM; }

//Get User
string Process::User() { return this->m_User; }

//Get Uptime
long int Process::UpTime() { return this->m_UpTime; }

//Overload less than and greater than.
bool Process::operator<(Process const& a) const { 
    return this->m_Utilization < a.m_Utilization;
}
bool Process::operator>(Process const& a) const { 
    return this->m_Utilization > a.m_Utilization;
}