#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <cctype>
#include <pwd.h>

#include "linux_parser.h"
#include "format.h"

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
  for (const auto & entry : std::filesystem::directory_iterator(kProcDirectory)) {
      if (entry.is_directory() && std::isdigit(entry.path().filename().string()[0])) {
          pids.push_back(std::stoi(entry.path().filename().string()));
      }
  }
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  string line;
  string key;
  string mem_total;
  string mem_free;
  float mem_util{0.0f};

  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream1(line);
      linestream1 >> key >> mem_total;
      std::getline(filestream, line);
      std::istringstream linestream2(line);
      linestream2 >> key >> mem_free;
      mem_util = (std::stof(mem_total)-std::stof(mem_free))/std::stof(mem_total);
  }
  return mem_util;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime()
{
  string line;
  string uptime;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> uptime;
  }
  return std::stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
  return ActiveJiffies() + IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{
  string line;
  string jiffies;
  size_t i{0};
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kUptimeFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      while(i<22)
      {
        linestream >> jiffies;
        i++;
      }
  }
  return std::stol(jiffies)/sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  string line;
  string uptime;
  string idletime;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> uptime >> idletime;
  }
  return std::stol(uptime) * sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  string line;
  string uptime;
  string idletime;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> uptime >> idletime;
  }
  return std::stol(idletime) * sysconf(_SC_CLK_TCK);
}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization()
{
  string line;
  string cpu;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }
  int Idle = std::stoi(idle) + std::stoi(iowait);
  int NonIdle = std::stoi(user) + std::stoi(nice) + std::stoi(system) + std::stoi(irq) + std::stoi(softirq) + std::stoi(steal);
  int Total = Idle + NonIdle;
  return (float)NonIdle/Total;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string num_process;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
      while(std::getline(filestream, line))
      {
        std::istringstream linestream(line);
        linestream >> key >> num_process;
        if(key=="processes")
        {
          return std::stoi(num_process);
        }
      }
  }
  return -1;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{
  string line;
  string key;
  string num_process_running;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
      while(std::getline(filestream, line))
      {
        std::istringstream linestream(line);
        linestream >> key >> num_process_running;
        if(key=="procs_running")
        {
          return std::stoi(num_process_running);
        }
      }
  }
  return -1;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
  string line;
  string cmdline;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> cmdline;
  }
  return cmdline;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
  string line;
  string key;
  string ram;
  size_t i{0};
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while(i<18)
    {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key >> ram;
      i++;
    }
  }
  return std::to_string(std::stoi(ram)/1000);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
{
  string line;
  string key;
  string uid;
  size_t i{0};
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while(i<9)
    {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key >> uid;
      i++;
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{
  std::string uid = Uid(pid);
  string line;
  string key;
  std::string token1, token2;
  string user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> token1 >> token2;
      if(token2==uid)
      {
        return user;
      }
    }
  }
  // some uid may not exist in /etc/passwd
  return getpwuid(std::stoi(uid))->pw_name;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{
  string line;
  string uptime;
  size_t i{0};
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      while(i<22)
      {
        uptime = "";
        linestream >> uptime;
        i++;
      }
  }
  return std::stol(uptime)/sysconf(_SC_CLK_TCK);
}

float LinuxParser::CpuUtilization(int pid)
{
  long utime{0}, stime{0}, cutime{0}, cstime{0}, starttime{0}, total_time{0}, uptime{0}, elapsed_seconds{0};
  string token;
  size_t i{0};
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
      while(i<13)
      {
        filestream >> token;
        i++;
      }
      filestream >> utime >> stime >> cutime >> cstime;
      i += 4;
      while(i<21)
      {
        filestream >> token;
        i++;
      }
      filestream >> starttime;
  }
  std::ifstream filestream1(kProcDirectory+kUptimeFilename);
  filestream1 >> uptime;
  total_time = utime + stime + cutime + cstime;
  elapsed_seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
  return 100*((total_time/sysconf(_SC_CLK_TCK))/(float)elapsed_seconds);
}