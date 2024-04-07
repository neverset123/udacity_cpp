#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) 
{
    std::stringstream hh, mm, ss;
    int hour = seconds/3600%24;
    seconds %= 3600;
    int min = seconds/60;
    int sec = seconds%60;
    
    hh << std::setw(2) << std::setfill('0') << hour;
    mm << std::setw(2) << std::setfill('0') << min;
    ss << std::setw(2) << std::setfill('0') << sec;
    return hh.str() + ":" + mm.str() + ":" + ss.str();
}