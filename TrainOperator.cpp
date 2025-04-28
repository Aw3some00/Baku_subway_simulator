#include "TrainOperator.h"
#include <iostream>
#include <thread>
#include <random>
#include <algorithm>

TrainOperator::TrainOperator(int id, const std::string& route, bool is_forward, const TransitNetwork& network, std::mutex& output)
    : operator_id_(id), route_name_(route), forward_direction_(is_forward),
    network_(network), output_mutex_(output), data_(), running(true) {}

TrainOperator::~TrainOperator() {}

TrainOperator::TrainOperator(const TrainOperator& other)
    : operator_id_(other.operator_id_), route_name_(other.route_name_),
    forward_direction_(other.forward_direction_), network_(other.network_),
    output_mutex_(other.output_mutex_), data_(other.data_), running(other.running) {}

TrainOperator& TrainOperator::operator=(const TrainOperator& other) {
    if (this != &other) {
        operator_id_ = other.operator_id_;
        route_name_ = other.route_name_;
        forward_direction_ = other.forward_direction_;

        data_ = other.data_;
        running = other.running;
    }
    return *this;
}

TrainOperator::TrainOperator(TrainOperator&& other) noexcept
    : operator_id_(other.operator_id_), route_name_(std::move(other.route_name_)),
    forward_direction_(other.forward_direction_), network_(other.network_),
    output_mutex_(other.output_mutex_), data_(std::move(other.data_)), running(other.running) {}

TrainOperator& TrainOperator::operator=(TrainOperator&& other) noexcept {
    if (this != &other) {
        operator_id_ = other.operator_id_;
        route_name_ = std::move(other.route_name_);
        forward_direction_ = other.forward_direction_;


        data_ = std::move(other.data_);
        running= other.running;
    }
    return *this;
}

void TrainOperator::secure_log(const std::string& message) {
    std::lock_guard<std::mutex> lock(output_mutex_);
    std::cout << "Train " << operator_id_ << ": " << message << std::endl;
}

bool TrainOperator::is_high_traffic_time() {
    return false; // Placeholder
}

int TrainOperator::estimate_travel_time(double distance) {
    return static_cast<int>(distance * 60 * 1000); // Milliseconds
}

void TrainOperator::return_to_hub(const std::vector<std::string>& stops, int current_stop, const TransitNetwork::Route& route) {
    std::string hub = *route.hub;
    int hub_index = -1;
    for (std::vector<std::string>::size_type i = 0; i < stops.size(); ++i) {
        if (stops[i] == hub) {
            hub_index = static_cast<int>(i);
            break;
        }
    }

    if (hub_index == -1) {
        secure_log("Error: Hub " + hub + " not found in route " + route_name_);
        return;
    }

    if (hub_index < 0 || hub_index >= static_cast<int>(route.platform_locks.size())) {
        secure_log("Error: Invalid hub_index " + std::to_string(hub_index) +
                   " for platform_locks size " + std::to_string(route.platform_locks.size()));
        return;
    }

    if (current_stop < 0 || current_stop >= static_cast<int>(route.platform_locks.size())) {
        secure_log("Error: Invalid current_stop " + std::to_string(current_stop) +
                   " for platform_locks size " + std::to_string(route.platform_locks.size()));
        return;
    }

    secure_log("Returning to hub " + hub + " from stop " + stops[current_stop] +
               ", hub_index: " + std::to_string(hub_index) +
               ", platform_locks size: " + std::to_string(route.platform_locks.size()));

    int direction = (hub_index > current_stop) ? 1 : -1;
    while (current_stop != hub_index) {
        {
            std::unique_lock<std::mutex> stop_lock(route.platform_locks[current_stop]);
            int next_index = current_stop + direction;
            if (next_index < 0 || next_index >= static_cast<int>(stops.size())) {
                secure_log("Error: Next stop index " + std::to_string(next_index) +
                           " out of bounds for stops size " + std::to_string(stops.size()));
                break;
            }
            std::string next_stop = stops[next_index];
            secure_log("Moving from " + stops[current_stop] + " to " + next_stop);
            double distance = network_.distance_between(stops[current_stop], next_stop);
            int travel_time = estimate_travel_time(distance);
            std::this_thread::sleep_for(std::chrono::milliseconds(travel_time));
            data_.total_km += distance;
        }
        current_stop += direction;
        if (current_stop < 0 || current_stop >= static_cast<int>(route.platform_locks.size())) {
            secure_log("Error: current_stop " + std::to_string(current_stop) +
                       " out of bounds for platform_locks size " + std::to_string(route.platform_locks.size()));
            break;
        }
    }

    if (current_stop == hub_index) {
        if (current_stop < 0 || current_stop >= static_cast<int>(route.platform_locks.size())) {
            secure_log("Error: Invalid current_stop " + std::to_string(current_stop) +
                       " for final lock, platform_locks size " + std::to_string(route.platform_locks.size()));
            return;
        }
        std::unique_lock<std::mutex> lock(route.platform_locks[current_stop]);
        secure_log("Arrived at " + hub + " 🚉");
    } else {
        secure_log("Failed to reach hub " + hub + ", stopped at " + stops[current_stop]);
    }
}

void TrainOperator::start_journey() {
    const auto* routes = network_.routes();
    if (routes->find(route_name_) == routes->end()) {
        secure_log("Error: Route " + route_name_ + " not found!");
        return;
    }

    const auto& route = routes->at(route_name_);
    auto stops = *route.stops;
    if (stops.empty()) {
        secure_log("Error: No stops in route " + route_name_);
        return;
    }

    std::string hub = *route.hub;
    int hub_index = 0;
    for (std::vector<std::string>::size_type i = 0; i < stops.size(); ++i) {
        if (stops[i] == hub) {
            hub_index = static_cast<int>(i);
            break;
        }
    }

    int current_stop = (hub_index != 0) ? hub_index : (forward_direction_ ? 0 : static_cast<int>(stops.size()) - 1);
    int direction = forward_direction_ ? 1 : -1;
    bool is_shuttle = route.is_shuttle;

    std::map<std::string, int> stop_traffic = {
        {"Icheri Sheher", 300}, {"Sahil", 250}, {"28 May", 400}, {"Ganjlik", 200},
        {"Nariman Narimanov", 220}, {"Bakmil", 100}, {"Ulduz", 150}, {"Koroglu", 250},
        {"Kara Karaev", 180}, {"Neftchilar", 150}, {"Khalglar Dostlugu", 200}, {"Ahmedli", 220},
        {"Azi Aslanov", 180}, {"Jafar Jabbarly", 200}, {"Hatai", 100},
        {"Khojasan", 80}, {"Avtovagzal", 180}, {"8 Noyabr", 220},
        {"Nizami", 250}, {"Elmlar Akademiyasy", 200}, {"Inshaatchilar", 180},
        {"20 January", 220}, {"Memar Ajami", 230}, {"Nasimi", 210},
        {"Azadlig Prospekti", 190}, {"Darnagul", 170}
    };

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> rider_rng(0, 100);
    std::uniform_real_distribution<> fault_rng(0.0, 1.0);

    const double fuel_cost_per_km = 0.1;
    int shift_number = 1;

    secure_log("🚆 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") departing from " + hub + " 🚉");

    while (running) {
        secure_log("⏰ Train " + std::to_string(operator_id_) + " (" + route_name_ + ") shift " + std::to_string(shift_number) + " started 🟢");

        while (running) {
            if (current_stop < 0 || current_stop >= static_cast<int>(route.platform_locks.size())) {
                secure_log("Error: Invalid current_stop " + std::to_string(current_stop) +
                           " for platform_locks size " + std::to_string(route.platform_locks.size()));
                running = false;
                break;
            }
            secure_log("Locking platform at stop " + stops[current_stop] +
                       ", current_stop: " + std::to_string(current_stop) +
                       ", platform_locks size: " + std::to_string(route.platform_locks.size()));
            std::unique_lock<std::mutex> stop_lock(route.platform_locks[current_stop]);

            std::string next_stop = (current_stop + direction >= 0 && current_stop + direction < static_cast<int>(stops.size())) ?
                                        stops[current_stop + direction] : "End of Route";

            secure_log("🛤️ Train " + std::to_string(operator_id_) + " (" + route_name_ + ") reached " + stops[current_stop] +
                       ", heading to " + next_stop + " 🚅");

            int riders_off = std::min(data_.riders, rider_rng(rng));
            int riders_on = rider_rng(rng) % stop_traffic[stops[current_stop]];
            riders_on = is_high_traffic_time() ? riders_on * 2 : riders_on;

            data_.riders = std::min(data_.max_riders, data_.riders - riders_off + riders_on);

            secure_log("👥 Train " + std::to_string(operator_id_) + " (" + route_name_ + "): " +
                       std::to_string(riders_off) + " alighted 🚶, " + std::to_string(riders_on) + " boarded 🧳, current: " +
                       std::to_string(data_.riders) + " passengers");

            int stop_duration = (20 + (rider_rng(rng) % 21)) * 1000 / 120;
            std::this_thread::sleep_for(std::chrono::milliseconds(stop_duration));

            secure_log("🚪 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") leaving " + stops[current_stop] + " 👋");

            stop_lock.unlock();

            if (current_stop + direction >= 0 && current_stop + direction < static_cast<int>(stops.size())) {
                double distance = network_.distance_between(stops[current_stop], stops[current_stop + direction]);
                data_.total_km += distance;
                int travel_time = estimate_travel_time(distance);
                secure_log("🚄 Train " + std::to_string(operator_id_) + " traveling to " + stops[current_stop + direction] +
                           " (" + std::to_string(travel_time / 1000.0) + "s) 🕒");
                std::this_thread::sleep_for(std::chrono::milliseconds(travel_time));
            }

            if (fault_rng(rng) < 0.01) {
                secure_log("⚠️ Train " + std::to_string(operator_id_) + " (" + route_name_ + ") experienced a fault 🛠️, cost: 50 AZN 💸");
                running = false;
                break;
            }

            current_stop += direction;
            if (current_stop < 0 || current_stop >= static_cast<int>(stops.size())) {
                direction = -direction;
                current_stop = std::max(0, std::min(static_cast<int>(stops.size()) - 1, current_stop + 2 * direction));
                if (!is_shuttle) {
                    break;
                }
            }
        }

        if (!running) {
            secure_log("Preparing to return to hub, current_stop: " + std::to_string(current_stop) +
                       ", stops size: " + std::to_string(stops.size()) +
                       ", platform_locks size: " + std::to_string(route.platform_locks.size()));
            return_to_hub(stops, current_stop, route);
            break;
        }

        if (!is_shuttle) {
            secure_log("🏁 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") shift " +
                       std::to_string(shift_number) + " completed, returned to " + hub + " 🏠");
        } else {
            if (current_stop < 0 || current_stop >= static_cast<int>(route.platform_locks.size())) {
                secure_log("Error: Invalid current_stop " + std::to_string(current_stop) +
                           " for shift completion lock, platform_locks size " + std::to_string(route.platform_locks.size()));
            } else {
                std::unique_lock<std::mutex> lock(route.platform_locks[current_stop]);
                secure_log("🏁 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") shift " +
                           std::to_string(shift_number) + " completed, stationed at " + stops[current_stop] + " 🚉");
            }
        }
        shift_number++;
    }

    if (!is_shuttle) {
        secure_log("🎉 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") simulation ended, at " + hub + " 🏁");
    } else {
        if (current_stop < 0 || current_stop >= static_cast<int>(route.platform_locks.size())) {
            secure_log("Error: Invalid current_stop " + std::to_string(current_stop) +
                       " for simulation end lock, platform_locks size " + std::to_string(route.platform_locks.size()));
        } else {
            std::unique_lock<std::mutex> lock(route.platform_locks[current_stop]);
            secure_log("🎉 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") simulation ended, at " + stops[current_stop] + " 🏁");
        }
    }
}
