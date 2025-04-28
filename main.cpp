#include "SimulationManager.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>

int main() {
    std::string time_input;
    int hours, minutes;
    char colon;
    std::cout << "Enter simulation end time (HH:MM, 24-hour format, e.g., 14:30): ";
    std::getline(std::cin, time_input);
    std::istringstream iss(time_input);
    if (!(iss >> hours >> colon >> minutes) || colon != ':' || hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        std::cout << "Invalid time format. Using default end time (in 10 minutes).\n";
        hours = 0;
        minutes = 10;
    }
    std::cout << "Simulation will end at " << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes << ".\n";

    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    struct tm now_tm = *localtime(&now_time_t);

    struct tm end_tm = now_tm;
    end_tm.tm_hour = hours;
    end_tm.tm_min = minutes;
    end_tm.tm_sec = 0;
    auto end_time = std::chrono::system_clock::from_time_t(mktime(&end_tm));

    if (end_time < now) {
        end_time += std::chrono::hours(24);
    }

    SimulationManager manager;
    manager.start_operations();

    return 0;
}
