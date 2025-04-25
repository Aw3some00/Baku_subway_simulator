#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <mutex>

class SystemMonitor {
public:
    SystemMonitor();
    void record_passengers(int boarding, int alighting);
    void log_energy_cost(double cost);
    void log_incident_cost(double cost);
    void print_summary();

private:
    long long total_riders_;
    int active_riders_;
    double energy_expense_;
    double incident_expense_;
    std::mutex rider_lock_;
    std::mutex cost_lock_;
};

#endif // SYSTEM_MONITOR_H
