#include "TrainOperator.h"
#include "SystemMonitor.h"
#include <random>
#include <chrono>
#include <thread>
#include <ctime>
#include <iostream>
#include <cmath>

extern SystemMonitor monitor;

TrainOperator::TrainOperator(int id, const std::string& route, bool is_forward, const TransitNetwork& network, std::mutex& output)
    : operator_id_(id), route_name_(route), forward_direction_(is_forward), network_(network), output_mutex_(output) {

    data_.riders = 0;
    data_.max_riders = 500;
    data_.total_km = 0.0;
}

TrainOperator::~TrainOperator() {
}

TrainOperator::TrainOperator(const TrainOperator& other)
    : operator_id_(other.operator_id_), route_name_(other.route_name_),
    forward_direction_(other.forward_direction_), network_(other.network_),
    output_mutex_(other.output_mutex_), data_(other.data_) {}

TrainOperator& TrainOperator::operator=(const TrainOperator& other) {
    if (this != &other) {
        operator_id_ = other.operator_id_;
        route_name_ = other.route_name_;
        forward_direction_ = other.forward_direction_;
        data_ = other.data_;
    }
    return *this;
}

TrainOperator::TrainOperator(TrainOperator&& other) noexcept
    : operator_id_(other.operator_id_), route_name_(std::move(other.route_name_)),
    forward_direction_(other.forward_direction_), network_(other.network_),
    output_mutex_(other.output_mutex_), data_(std::move(other.data_)) {}

TrainOperator& TrainOperator::operator=(TrainOperator&& other) noexcept {
    if (this != &other) {
        operator_id_ = other.operator_id_;
        route_name_ = std::move(other.route_name_);
        forward_direction_ = other.forward_direction_;
        data_ = std::move(other.data_);
    }
    return *this;
}

void TrainOperator::secure_log(const std::string& message) {
    std::lock_guard<std::mutex> lock(output_mutex_);
    std::cout << message << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

bool TrainOperator::is_high_traffic_time() {
    auto now = std::chrono::system_clock::now();
    time_t current_time = std::chrono::system_clock::to_time_t(now);
    struct tm* time_info = localtime(&current_time);
    return (time_info->tm_hour >= 7 && time_info->tm_hour < 9) || (time_info->tm_hour >= 17 && time_info->tm_hour < 19);
}

int TrainOperator::estimate_travel_time(double distance) {
    const double speed_kmh = 40.0;
    const double sim_scale = 120.0;
    // Проверка входных параметров
    if (distance <= 0 || std::isnan(distance) || std::isinf(distance) || speed_kmh == 0 || sim_scale == 0) {
        secure_log("Error: Invalid parameters in estimate_travel_time (distance=" + std::to_string(distance) + ")");
        return 250;
    }
    int real_seconds = static_cast<int>((distance / speed_kmh) * 3600);
    int sim_ms = static_cast<int>((real_seconds / sim_scale) * 1000);
    return std::max(250, sim_ms);
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
        {"Icheri Sheher", 300},{"Memar Acemi 2",300}, {"Sahil", 250}, {"28 May", 400}, {"Ganjlik", 200},
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

    static std::map<std::string, std::mutex> stop_locks;
    static std::mutex init_mutex;
    {
        std::lock_guard<std::mutex> lock(init_mutex);
        for (const auto& stop : stops) {
            stop_locks.emplace(std::piecewise_construct,
                               std::forward_as_tuple(stop),
                               std::forward_as_tuple());
        }
        stop_locks.emplace(std::piecewise_construct,
                           std::forward_as_tuple("Jafar Jabbarly"),
                           std::forward_as_tuple());
    }

    auto sim_start = std::chrono::steady_clock::now();
    const auto sim_limit = std::chrono::minutes(10);
    const auto shift_limit = std::chrono::minutes(5);
    const double fuel_cost_per_km = 0.1;
    int shift_number = 1;

    secure_log("🚆 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") departing from " + hub + " 🚉");

    while (std::chrono::steady_clock::now() - sim_start < sim_limit) {
        auto shift_start = std::chrono::steady_clock::now();
        std::string shift_message = "⏰ Train " + std::to_string(operator_id_) + " (" + route_name_ + ") shift " +
                                    std::to_string(shift_number) + " started ";
        if (route_name_ == "Red") {
            shift_message += "\U0001F534 \u2705";
        } else if (route_name_ == "Green") {
            shift_message += "\U0001F7E2 \u2705";
        } else if (route_name_ == "Purple") {
            shift_message += "\U0001F7E3 \u2705";
        } else if (route_name_ == "Light Green") {
            shift_message += "\U0001F49A \u2705";
        }
        secure_log(shift_message);

        while (std::chrono::steady_clock::now() - shift_start < shift_limit &&
               std::chrono::steady_clock::now() - sim_start < sim_limit) {
            // Проверка корректности current_stop
            if (current_stop < 0 || current_stop >= static_cast<int>(stops.size())) {
                secure_log("Error: Invalid stop index " + std::to_string(current_stop));
                return;
            }

            std::unique_lock<std::mutex> stop_lock(stop_locks[stops[current_stop]]);

            std::string next_stop = (current_stop + direction >= 0 && current_stop + direction < static_cast<int>(stops.size())) ?
                                        stops[current_stop + direction] : "End of Route";

            std::string arrive_message = "🛤️ Train " + std::to_string(operator_id_) + " (" + route_name_ + ") reached " +
                                         stops[current_stop] + ", heading to " + next_stop + " 🚅";
            if (route_name_ == "Red") {
                arrive_message += " \U0001F534";
            } else if (route_name_ == "Green") {
                arrive_message += " \U0001F7E2";
            } else if (route_name_ == "Purple") {
                arrive_message += " \U0001F7E3";
            } else if (route_name_ == "Light Green") {
                arrive_message += " \U0001F49A";
            }
            secure_log(arrive_message);

            // Проверка stop_traffic
            if (stop_traffic.find(stops[current_stop]) == stop_traffic.end()) {
                secure_log("Error: Stop " + stops[current_stop] + " not found in stop_traffic!");
                return;
            }
            if (stop_traffic[stops[current_stop]] == 0) {
                secure_log("Error: Zero traffic value for stop " + stops[current_stop]);
                return;
            }

            int riders_off = std::min(data_.riders, rider_rng(rng));
            int riders_on = rider_rng(rng) % stop_traffic[stops[current_stop]];
            riders_on = is_high_traffic_time() ? riders_on * 2 : riders_on;


       /*     secure_log("Debug: riders_off=" + std::to_string(riders_off) + ", riders_on=" + std::to_string(riders_on) +
                       ", data_.riders=" + std::to_string(data_.riders) + ", stop=" + stops[current_stop]);*/

            monitor.record_passengers(riders_on, riders_off);
            data_.riders = std::min(data_.max_riders, data_.riders - riders_off + riders_on);

            std::string passenger_message = "👥 Train " + std::to_string(operator_id_) + " (" + route_name_ + "): " +
                                            std::to_string(riders_off) + " alighted 🚶, " + std::to_string(riders_on) +
                                            " boarded 🧳, current: " + std::to_string(data_.riders) + " passengers";
            if (route_name_ == "Red") {
                passenger_message += " \U0001F534";
            } else if (route_name_ == "Green") {
                passenger_message += " \U0001F7E2";
            } else if (route_name_ == "Purple") {
                passenger_message += " \U0001F7E3";
            } else if (route_name_ == "Light Green") {
                passenger_message += " \U0001F49A";
            }
            secure_log(passenger_message);

            int stop_duration = (20 + (rand() % 21)) * 1000 / 120;
            std::this_thread::sleep_for(std::chrono::milliseconds(stop_duration));

            secure_log("🚪 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") leaving " + stops[current_stop] + " 👋");

            stop_lock.unlock();

            if (current_stop + direction >= 0 && current_stop + direction < static_cast<int>(stops.size())) {
                double distance = network_.distance_between(stops[current_stop], stops[current_stop + direction]);
                if (distance <= 0 || std::isnan(distance) || std::isinf(distance)) {
                    secure_log("Error: Invalid distance between " + stops[current_stop] + " and " + stops[current_stop + direction]);
                    return;
                }
                data_.total_km += distance;
                int travel_time = estimate_travel_time(distance);
                secure_log("🚄 Train " + std::to_string(operator_id_) + " traveling to " + stops[current_stop + direction] +
                           " (" + std::to_string(travel_time / 1000.0) + "s) 🕒");
                std::this_thread::sleep_for(std::chrono::milliseconds(travel_time));
            }

            if (fault_rng(rng) < 0.01) {
                secure_log("⚠️ Train " + std::to_string(operator_id_) + " (" + route_name_ + ") experienced a fault 🛠️, cost: 300 bucks 💸");
                monitor.log_incident_cost(50.0);
            }

            current_stop += direction;
            if (current_stop < 0 || current_stop >= static_cast<int>(stops.size())) {
                direction = -direction;
                current_stop += 2 * direction;
            }
        }

        monitor.log_energy_cost(data_.total_km * fuel_cost_per_km);

        if (!is_shuttle) {
            secure_log("🏁 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") shift " +
                       std::to_string(shift_number) + " completed, returned to " + hub + " 🏠");
        } else {
            std::unique_lock<std::mutex> lock(stop_locks[stops[current_stop]]);
            secure_log("🏁 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") shift " +
                       std::to_string(shift_number) + " completed, stationed at " + stops[current_stop] + " 🚉");
        }
        shift_number++;
    }

    monitor.log_energy_cost(data_.total_km * fuel_cost_per_km);

    if (!is_shuttle) {
        secure_log("🎉 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") simulation ended, at " + hub + " 🏁");
    } else {
        std::unique_lock<std::mutex> lock(stop_locks[stops[current_stop]]);
        secure_log("🎉 Train " + std::to_string(operator_id_) + " (" + route_name_ + ") simulation ended, at " + stops[current_stop] + " 🏁");
    }
}
