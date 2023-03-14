#include <string>
#include "format.h"
#include <chrono>
#include <sstream>
#include <iomanip>

using namespace std;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long s) { 
    std::chrono::seconds  seconds{s};

    std::chrono::hours hours  = 
        std::chrono::duration_cast<std::chrono::hours>(seconds);

    seconds -= std::chrono::duration_cast<std::chrono::seconds>(hours);

    std::chrono::minutes minutes  = 
        std::chrono::duration_cast<std::chrono::minutes>(seconds);

    seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);

    std::stringstream ss{};

    ss << setw(2) << std::setfill('0') << hours.count() 
    << std::setw(1) << ":"
    << setw(2) << std::setfill('0') << minutes.count() 
    << std::setw(1) << ":"
    << setw(2) << std::setfill('0') << seconds.count(); 

    return ss.str();
}