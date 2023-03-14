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
using std::all_of;

template <typename T> 
T findValueByKey(std::string const  &PathName, std::string const &keyFilter)
{
  string strLine;
  string strKey;
  T Tvalue;

  std::ifstream filestream(PathName);
  if (filestream.is_open()) {
    while (std::getline(filestream, strLine)) {
      std::istringstream linestream(strLine);
      //while (linestream >> strKey >> Tvalue) {
        linestream >> strKey >> Tvalue;
        if (strKey == keyFilter) {
          return Tvalue;
        }
     // }
    }
  }
  return Tvalue;
};

template <typename T> 
T getValueOfFile(std::string const  &PathName)
{
  string strLine;
  T Tvalue;

  std::ifstream filestream(PathName);
  if (filestream.is_open()) {
    getline(filestream, strLine);
    std::istringstream linestream(strLine);
    linestream >> Tvalue;
  }
  return Tvalue;
}



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
  string  strMemTotal   = "MemTotal:";
  string  strMemFree    = "MemFree:";
  float   fMemTotal     = 0.0;
  float   fMemFree      = 0.0;

  string strPath(kProcDirectory + kMeminfoFilename);

  fMemTotal  = findValueByKey<float>(strPath,strMemTotal);
  fMemFree   = findValueByKey<float>(strPath,strMemFree);
  
  return (fMemTotal-fMemFree)*1.0/fMemTotal; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string strFileName;
  string line;
  string value;
  vector<string> list;
  long utime;
  long stime;
  long cutime;
  long cstime;
  long total;

  strFileName = kProcDirectory + to_string(pid) +kStatFilename;
  std::ifstream stream(strFileName);

  if(stream.is_open())
  {
    while(getline(stream,line))
    {
      std::istringstream linestream(line);
      while(linestream >> value)
      {
        list.push_back(value);        
      }
    }
  }

  utime   = std::stol(list[13].c_str());
  stime   = std::stol(list[14].c_str());
  cutime  = std::stol(list[15].c_str());
  cstime  = std::stol(list[16].c_str());

  total = utime + stime + cutime + cstime;

  return long(total/ sysconf(_SC_CLK_TCK)); 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {  
  long kUser_time    = atof(CpuUtilization()[kUser_].c_str());
  long kNice_time    = atof(CpuUtilization()[kNice_].c_str());
  long kSystem_time  = atof(CpuUtilization()[kSystem_].c_str());
  long kIRQ_time     = atof(CpuUtilization()[kIRQ_].c_str());
  long kSoftIRQ_time = atof(CpuUtilization()[kSoftIRQ_].c_str());
  long kSteal_time   = atof(CpuUtilization()[kSteal_].c_str());
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
  string strCpu;
  string strLine;
  string strValue;
  vector<string> Jiffies;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(getline(stream,strLine))
    {
      std::istringstream linestream(strLine);
      linestream >> strCpu;
      if(strCpu == "cpu")
      {
        while(linestream >> strValue)
        {
          Jiffies.push_back(strValue);
        }
      }
    }
  }
  return Jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string strPath;
  string strKey;
  int nTotalProcesses;

  strKey  = "processes";
  strPath = (kProcDirectory + kStatFilename);

  nTotalProcesses = findValueByKey<int>(strPath,strKey);

  return nTotalProcesses; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string strPath;
  string strKey;
  int nRunningProcesses;

  strKey  = "procs_running";
  strPath = (kProcDirectory + kStatFilename);

  nRunningProcesses = findValueByKey<int>(strPath,strKey);

  return nRunningProcesses;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string strCmdline;
  string strPath;

  strPath     = kProcDirectory + to_string(pid) +kCmdlineFilename;
  strCmdline  = getValueOfFile<string>(strPath);

  return strCmdline; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string strCmdline;
  string strPath;
  string strLine;
  string strKey;
  string strValue;
  //{
  strPath = kProcDirectory + to_string(pid) +kStatusFilename;
  std::ifstream stream(strPath);
  if(stream.is_open())
  {
    while(getline(stream,strLine))  
    {
      std::replace(strLine.begin(),strLine.end(),':',' ');
      std::istringstream linestream(strLine);
      linestream >> strKey;
      if(strKey == "VmRSS")
      {
          linestream >> strValue;
      }
    }
    
  }
  return strValue; 
}

int LinuxParser::GetRam(int pid)
{
  string strRam = LinuxParser::Ram(pid);
  return atoi(strRam.c_str());
}


string LinuxParser::Uid(int pid) { 
  string strPath;
  string strKey;
  
  strPath = kProcDirectory + to_string(pid) +kStatusFilename;
  strKey  = "Uid:";

  return findValueByKey<string>(strPath,strKey); 
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
  uid = Uid(pid);
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

long LinuxParser::UpTime() { 
  string  strPath;
  long    lUptime;
  
  strPath   = (kProcDirectory + kUptimeFilename);
  lUptime   = getValueOfFile<long>(strPath);

  return lUptime;
}

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
  return long(std::stol(ListProcStat[21].c_str()) / sysconf(_SC_CLK_TCK));
}
