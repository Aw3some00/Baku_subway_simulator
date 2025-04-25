#ifndef TRANSIT_NETWORK_H
#define TRANSIT_NETWORK_H

#include <string>
#include <vector>
#include <map>
#include <mutex>

class TransitNetwork {
public:
    TransitNetwork();
    struct Route {
        std::vector<std::string> stops;
        std::string terminal;
        std::vector<std::mutex>forward_locks;
        std::vector<std::mutex>reverse_locks;
        bool is_shuttle = false;
    };
    const std::map<std::string, Route>& routes() const;
    double distance_between(const std::string& start, const std::string& end) const;

private:
    std::map<std::string, Route> routes_;
    std::map<std::pair<std::string, std::string>, double, std::less<>>stop_distances_;
    void setup_routes();
    void setup_distances();
};

#endif // TRANSIT_NETWORK_H
