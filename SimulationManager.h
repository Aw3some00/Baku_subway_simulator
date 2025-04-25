#ifndef SIMULATION_MANAGER_H
#define SIMULATION_MANAGER_H

#include "TransitNetwork.h"
#include "TrainOperator.h"
#include "SystemMonitor.h"
#include <vector>
#include <thread>

class SimulationManager {
public:
    SimulationManager();
    void start_operations();
    void collect_train_counts(int& red_trains, int& green_trains, int& purple_trains, int& light_green_trains);
private:
    TransitNetwork network_;
    SystemMonitor monitor_;
    std::mutex output_mutex_;
    void show_welcome();
};

#endif // SIMULATION_MANAGER_H
