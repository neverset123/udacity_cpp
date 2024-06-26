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

Process::Process(int id): pid(id) {};

// TODO: Return this process's ID
int Process::Pid()
{
    return pid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization()
{
    return LinuxParser::CpuUtilization(pid);
}

// TODO: Return the command that generated this process
string Process::Command()
{
    return LinuxParser::Command(pid);
}

// TODO: Return this process's memory utilization
string Process::Ram() 
{
    return LinuxParser::Ram(pid);
}

// TODO: Return the user (name) that generated this process
string Process::User()
{
    return LinuxParser::User(pid);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime()
{
    return LinuxParser::UpTime(pid);
}
