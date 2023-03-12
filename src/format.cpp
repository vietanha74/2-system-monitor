#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string hour;
    string min;
    string sec;

    //Convert second to hour/min/sec
    hour    = std::to_string(seconds/3600);
    min     = std::to_string((seconds%3600)/60);
    sec     = std::to_string((seconds%3600)%60);

    //Format out put
    hour.insert(0,2 - hour.length(),'0');
    min.insert(0,2 - min.length(),'0');
    sec.insert(0,2 - sec.length(),'0');

    return hour +":"+min + ":"+sec; 
}