#ifndef TRAIN_OPERATOR_H
#define TRAIN_OPERATOR_H

#include "TransitNetwork.h"
#include <string>
#include <mutex>

class TrainOperator {
public:
    TrainOperator(int id, const std::string& route, bool is_forward, const TransitNetwork& network, std::mutex& output);
    void start_journey();

private:
    int operator_id_;
    std::string route_name_;
    bool forward_direction_;
    const TransitNetwork& network_;
    std::mutex& output_mutex_;
    struct TrainData {
        int riders = 0;
        const int max_riders = 1600;
        double total_km = 0.0;
    };
    TrainData data_;
    void secure_log(const std::string& message);
    bool is_high_traffic_time();
    int estimate_travel_time(double distance);
};

#endif // TRAIN_OPERATOR_H
