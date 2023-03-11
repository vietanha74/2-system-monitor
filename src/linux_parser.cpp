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
  string os, version, kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  string line;
  string key;
  string value;
  string unit;
  float MemTotal = 0.0;
  float MemFree = 0.0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream,line))
    {
      line.erase(std::remove(line.begin(), line.end(),' '),line.end());
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value >> unit;
      value.erase(std::remove(value.begin(), value.end(),'k'),value.end());
      value.erase(std::remove(value.begin(), value.end(),'B'),value.end());
      if(key == "MemTotal") MemTotal = std::stof(value);
      if(key == "MemFree") MemFree = std::stof(value);  
    } 
  }
  
  return (MemTotal-MemFree)/MemTotal; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long value = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open())
  {
    filestream>>value;
  }
  return value; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {  
  float kUser_time    = atof(CpuUtilization()[kUser_].c_str());
  float kNice_time    = atof(CpuUtilization()[kNice_].c_str());
  float kSystem_time  = atof(CpuUtilization()[kSystem_].c_str());
  float kIRQ_time     = atof(CpuUtilization()[kIRQ_].c_str());
  float kSoftIRQ_time = atof(CpuUtilization()[kSoftIRQ_].c_str());
  float kSteal_time   = atof(CpuUtilization()[kSteal_].c_str());
  return kUser_time + kNice_time + kSystem_time + kIRQ_time + kSoftIRQ_time + kSteal_time;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  float kIdle_time    = atof(CpuUtilization()[kIdle_].c_str()); 
  float kIOwait_time  = atof(CpuUtilization()[kIOwait_].c_str());
  return kIdle_time + kIOwait_time;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string cpu;
  string line;
  string value;
  vector<string> Jiffies;

  std::ifstream stream("/proc/stat");
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> cpu;
      if(cpu == "cpu")
      {
        while(linestream >> value)
        {
          Jiffies.push_back(value);
        }
      }
    }
  }
  return Jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key;
  string line;
  string value;
  vector<string> jiffies;
  int nTotalProcesses;

  std::ifstream stream("/proc/stat");
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "processes")
      {
        linestream >> value;
        nTotalProcesses = atoi(value.c_str());
        
      }
    }
  }
  return nTotalProcesses; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key;
  string line;
  string value;
  vector<string> jiffies;
  int nRunningProcesses;

  std::ifstream stream("/proc/stat");
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "procs_running")
      {
        linestream >> value;
        nRunningProcesses = atoi(value.c_str());
        
      }
    }
  }
  return nRunningProcesses;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string strCmdline;
  string strCmdlineFilename;

  strCmdlineFilename = kProcDirectory + to_string(pid) +kCmdlineFilename;
  std::ifstream stream(strCmdlineFilename);
  if(stream.is_open())
  {
    stream >> strCmdline;
  }
  return strCmdline; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string strCmdline;
  string strCmdlineFilename;
  string line;
  string key;
  string value;
  //{
  strCmdlineFilename = kProcDirectory + to_string(pid) +kStatusFilename;
  std::ifstream stream(strCmdlineFilename);
  if(stream.is_open())
  {
    while(getline(stream,line))  
    {
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "VmSize")
      {
          linestream >> value;
      }
    }  
  }

  return value; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string strCmdline;
  string strCmdlineFilename;
  string line;
  string key;
  string value;
  //{
  strCmdlineFilename = kProcDirectory + to_string(pid) +kStatusFilename;
  std::ifstream stream(strCmdlineFilename);
  if(stream.is_open())
  {
    while(getline(stream,line))  
    {
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "Uid")
      {
          linestream >> value;
      }
    }  
  }

  return value; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string strCmdline;
  string strCmdlineFilename;
  string line;
  string value;
  string key, x;
  string strUidFilename;
  string uid,user;

  // Get UID by PID
  strUidFilename = kProcDirectory + to_string(pid) + kStatusFilename;
  std::ifstream stream1(strUidFilename);
  if(stream1.is_open())
  {
    while(getline(stream1,line))  
    {
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream1(line);
      linestream1 >> key ;
      if(key == "Uid")
      {
        linestream1 >> value;
        uid = value;
      }
    }
  }
  // Get User  by UID
  strCmdlineFilename = kPasswordPath;
  std::ifstream stream2(strCmdlineFilename);
  if(stream2.is_open())
  {
    while(getline(stream2,line))  
    {
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream2(line);
      linestream2 >> key >> x >> value;
      if(value == uid) 
      {
        user = key;
        break;
      }
    }  
  }
  return user;
}


// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string strFilename;
  string line;
  vector<string> ListProcStat;
  string value;
  // Get UID by PID
  strFilename = kProcDirectory + to_string(pid) + kStatFilename;
  std::ifstream stream(strFilename);
  if(stream.is_open())
  {
    while(getline(stream,line))  
    {
      std::istringstream linestream(line);
      while(linestream >> value)
      {
          ListProcStat.push_back(value);
      }
    }
  }
  return (atof(ListProcStat[21].c_str()) / sysconf(_SC_CLK_TCK));
}
