#include "SystemMonitor.h"
#include <iostream>
#include <iomanip>

SystemMonitor monitor; // Global instance

SystemMonitor::SystemMonitor() : total_riders_(0), active_riders_(0), energy_expense_(0.0), incident_expense_(0.0) {}

void SystemMonitor::record_passengers(int boarding, int alighting) {
    std::lock_guard<std::mutex> lock(rider_lock_);
    active_riders_ = active_riders_ - alighting + boarding;
    if (active_riders_ < 0) active_riders_ = 0;
    if (boarding > 0) total_riders_ += boarding;
}

void SystemMonitor::log_energy_cost(double cost) {
    std::lock_guard<std::mutex> lock(cost_lock_);
    energy_expense_ += cost;
}

void SystemMonitor::log_incident_cost(double cost) {
    std::lock_guard<std::mutex> lock(cost_lock_);
    incident_expense_ += cost;
}

void SystemMonitor::print_summary() {
    const double ticket_price = 0.5;
    const double upkeep_cost = 500.0;
    double income = total_riders_ * ticket_price;
    double total_expense = energy_expense_ + incident_expense_ + upkeep_cost;
    double profit = income - total_expense;

    std::lock_guard<std::mutex> lock(rider_lock_);
    std::cout << "Total passengers served: " << total_riders_ << std::endl;
    std::cout << "Revenue: " << std::fixed << std::setprecision(2) << income << " Bucks" << std::endl;
    std::cout << "Fuel expenses: " << std::fixed << std::setprecision(2) << energy_expense_ << " Bucks" << std::endl;
    std::cout << "Incident expenses: " << std::fixed << std::setprecision(2) << incident_expense_ << " Bucks" << std::endl;
    std::cout << "Maintenance cost: " << std::fixed << std::setprecision(2) << upkeep_cost << " Bucks" << std::endl;
    std::cout << "Total expenses: " << std::fixed << std::setprecision(2) << total_expense << " Bucks" << std::endl;
    std::cout << "Net profit: " << std::fixed << std::setprecision(2) << profit << " Bucks" << std::endl;
}
