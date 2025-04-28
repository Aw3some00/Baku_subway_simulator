#ifndef TRAIN_OPERATOR_H
#define TRAIN_OPERATOR_H

#include <string>
#include <mutex>
#include "TransitNetwork.h"

struct TrainData {
    int riders;
    double total_km;
    int max_riders;

    TrainData() : riders(0), total_km(0.0), max_riders(500) {}
};

class TrainOperator {
public:
    TrainOperator(int id, const std::string& route, bool is_forward, const TransitNetwork& network, std::mutex& output);
    ~TrainOperator();
    TrainOperator(const TrainOperator& other);
    TrainOperator& operator=(const TrainOperator& other);
    TrainOperator(TrainOperator&& other) noexcept;
    TrainOperator& operator=(TrainOperator&& other) noexcept;

    void return_to_hub( const std::vector<std::string>& stops, int current_stop, const TransitNetwork::Route& route);
    void start_journey();
    bool running;
private:
    void secure_log(const std::string& message);
    bool is_high_traffic_time();
    int estimate_travel_time(double distance);

    int operator_id_;
    std::string route_name_;
    bool forward_direction_;
    const TransitNetwork& network_;
    std::mutex& output_mutex_;
    TrainData data_;
};

#endif
