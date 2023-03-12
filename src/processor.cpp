#include "processor.h"
#include "linux_parser.h"
using namespace std;

Processor::Processor()
{
    this->m_PreIdleTime     = LinuxParser::IdleJiffies();
    this->m_PreActiveTime   = LinuxParser::ActiveJiffies();
    this->m_PreTotalTime    = LinuxParser::Jiffies();
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float CpuUtilization;
    long const CurrentIdleTime    = LinuxParser::IdleJiffies();
    long const CurrentActiveTime  = LinuxParser::ActiveJiffies();
    long const CurrentTotalTime   = LinuxParser::Jiffies();

    long deltaIdle                = CurrentIdleTime - this->m_PreIdleTime ;
    long deltaTotal               = CurrentTotalTime - this->m_PreTotalTime;
    
    update(CurrentIdleTime, CurrentActiveTime, CurrentTotalTime);
    
    if(deltaTotal == 0) {
        CpuUtilization = 0.0f;
    }
    else{
        CpuUtilization = (deltaTotal - deltaIdle)*1.0/deltaTotal;
    }
    
    return CpuUtilization; 
}

void Processor::update(long newIdleTime, long newActiveTime,long newTotalTime)
{
    this->m_PreIdleTime     = newIdleTime;
    this->m_PreActiveTime   = newActiveTime;
    this->m_PreTotalTime    = newTotalTime;
}