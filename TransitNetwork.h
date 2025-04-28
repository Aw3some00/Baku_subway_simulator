#ifndef TRANSIT_NETWORK_H
#define TRANSIT_NETWORK_H

#include <map>
#include <string>
#include <vector>
#include <mutex>

class TransitNetwork {
public:
    struct Route {
        std::vector<std::string>* stops;
        std::string* hub;
        std::vector<std::mutex> platform_locks;
        std::vector<std::mutex> track_locks;
        size_t platform_count=platform_locks.size();
        size_t track_count= track_locks.size();
        bool is_shuttle;

        Route(const std::vector<std::string>& s, const std::string& h, size_t platform_count,size_t  track_count, bool shuttle = false);
        ~Route();
        Route(const Route&);
        Route& operator=(const Route&);
        Route(Route&&) noexcept;
        Route& operator=(Route&&) noexcept;
    };

    TransitNetwork();
    ~TransitNetwork();
    TransitNetwork(const TransitNetwork&);
    TransitNetwork& operator=(const TransitNetwork&);
    TransitNetwork(TransitNetwork&&) noexcept;
    TransitNetwork& operator=(TransitNetwork&&) noexcept;

    const std::map<std::string, Route>* routes() const;
    double distance_between(const std::string& start, const std::string& end) const;

private:
    void setup_routes();
    void setup_distances();
    std::map<std::string, Route>* routes_;
    std::map<std::pair<std::string, std::string>, double>* stop_distances_;
};

#endif
