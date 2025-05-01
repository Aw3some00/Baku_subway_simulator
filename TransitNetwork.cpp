#include "TransitNetwork.h"

TransitNetwork::Route::Route(const std::vector<std::string>& s, const std::string& h, size_t platform_count, size_t track_count, bool shuttle)
    : stops(new std::vector<std::string>(s)), hub(new std::string(h)), platform_count(platform_count), track_count(track_count), is_shuttle(shuttle) {}

TransitNetwork::Route::~Route() {
    delete stops;
    delete hub;
}
TransitNetwork::Route::Route(const Route& other)
    : stops(new std::vector<std::string>(*other.stops)),
    hub(new std::string(*other.hub)),
    platform_locks(),  // Don't copy mutexes, just create new ones
    track_locks(),     // Don't copy mutexes, just create new ones
    is_shuttle(other.is_shuttle) {
    // Initialize mutex vectors with correct size
    platform_locks = std::vector<std::mutex>(other.platform_locks.size());
    track_locks = std::vector<std::mutex>(other.track_locks.size());
}

TransitNetwork::Route& TransitNetwork::Route::operator=(const Route& other) {
    if (this != &other) {
        delete stops;
        delete hub;
        stops = new std::vector<std::string>(*other.stops);
        hub = new std::string(*other.hub);
        platform_locks = std::vector<std::mutex>(other.platform_locks.size());
        track_locks = std::vector<std::mutex>(other.track_locks.size());
        is_shuttle = other.is_shuttle;
    }
    return *this;
}


TransitNetwork::Route::Route(Route&& other) noexcept
    : stops(other.stops), hub(other.hub), platform_locks(std::move(other.platform_locks)),
    track_locks(std::move(other.track_locks)), is_shuttle(other.is_shuttle) {
    other.stops = nullptr;
    other.hub = nullptr;
}

TransitNetwork::Route& TransitNetwork::Route::operator=(Route&& other) noexcept {
    if (this != &other) {
        delete stops;
        delete hub;
        stops = other.stops;
        hub = other.hub;
        platform_locks = std::move(other.platform_locks);
        track_locks = std::move(other.track_locks);
        is_shuttle = other.is_shuttle;
        other.stops = nullptr;
        other.hub = nullptr;
    }
    return *this;
}

TransitNetwork::TransitNetwork() : routes_(new std::map<std::string, Route>), stop_distances_(new std::map<std::pair<std::string, std::string>, double>) {
    setup_routes();
    setup_distances();
}

TransitNetwork::~TransitNetwork() {
    delete routes_;
    delete stop_distances_;
}

TransitNetwork::TransitNetwork(const TransitNetwork& other) : routes_(new std::map<std::string, Route>), stop_distances_(new std::map<std::pair<std::string, std::string>, double>) {
    *routes_ = *other.routes_;
    *stop_distances_ = *other.stop_distances_;
}

TransitNetwork& TransitNetwork::operator=(const TransitNetwork& other) {
    if (this != &other) {
        delete routes_;
        delete stop_distances_;
        routes_ = new std::map<std::string, Route>(*other.routes_);
        stop_distances_ = new std::map<std::pair<std::string, std::string>, double>(*other.stop_distances_);
    }
    return *this;
}

TransitNetwork::TransitNetwork(TransitNetwork&& other) noexcept
    : routes_(other.routes_), stop_distances_(other.stop_distances_) {
    other.routes_ = nullptr;
    other.stop_distances_ = nullptr;
}

TransitNetwork& TransitNetwork::operator=(TransitNetwork&& other) noexcept {
    if (this != &other) {
        delete routes_;
        delete stop_distances_;
        routes_ = other.routes_;
        stop_distances_ = other.stop_distances_;
        other.routes_ = nullptr;
        other.stop_distances_ = nullptr;
    }
    return *this;
}

const std::map<std::string, TransitNetwork::Route>* TransitNetwork::routes() const {
    return routes_;
}

double TransitNetwork::distance_between(const std::string& start, const std::string& end) const {
    auto key = std::make_pair(start, end);
    auto reverse_key = std::make_pair(end, start);
    double distance = 0.0;
    if (stop_distances_->count(key)) {
        distance = stop_distances_->at(key);
    } else if (stop_distances_->count(reverse_key)) {
        distance = stop_distances_->at(reverse_key);
    }
    return distance;
}

void TransitNetwork::setup_distances() {
    *stop_distances_ = {
        {{"Icheri Sheher", "Sahil"}, 0.9},
        {{"Sahil", "Icheri Sheher"}, 0.9},
        {{"Sahil", "28 May"}, 0.5},
        {{"28 May", "Sahil"}, 0.5},
        {{"28 May", "Ganjlik"}, 1.6},
        {{"Ganjlik", "28 May"}, 1.6},
        {{"Ganjlik", "Nariman Narimanov"}, 2.1},
        {{"Nariman Narimanov", "Ganjlik"}, 2.1},
        {{"Nariman Narimanov", "Bakmil"}, 1.3},
        {{"Bakmil", "Nariman Narimanov"}, 1.3},
        {{"Bakmil", "Ulduz"}, 1.9},
        {{"Ulduz", "Bakmil"}, 1.9},
        {{"Ulduz", "Koroglu"}, 2.3},
        {{"Koroglu", "Ulduz"}, 2.3},
        {{"Koroglu", "Kara Karaev"}, 1.8},
        {{"Kara Karaev", "Koroglu"}, 1.8},
        {{"Kara Karaev", "Neftchilar"}, 1.2},
        {{"Neftchilar", "Kara Karaev"}, 1.2},
        {{"Neftchilar", "Khalglar Dostlugu"}, 1.1},
        {{"Khalglar Dostlugu", "Neftchilar"}, 1.1},
        {{"Khalglar Dostlugu", "Ahmedli"}, 1.6},
        {{"Ahmedli", "Khalglar Dostlugu"}, 1.6},
        {{"Ahmedli", "Azi Aslanov"}, 1.3},
        {{"Azi Aslanov", "Ahmedli"}, 1.3},
        {{"Nizami", "Elmlar Akademiyasy"}, 1.2},
        {{"Elmlar Akademiyasy", "Nizami"}, 1.2},
        {{"Elmlar Akademiyasy", "Inshaatchilar"}, 0.9},
        {{"Inshaatchilar", "Elmlar Akademiyasy"}, 0.9},
        {{"Inshaatchilar", "20 January"}, 1.3},
        {{"20 January", "Inshaatchilar"}, 1.3},
        {{"20 January", "Memar Ajami"}, 1.8},
        {{"Memar Ajami", "20 January"}, 1.8},
        {{"Memar Ajami", "Nasimi"}, 2.66},
        {{"Nasimi", "Memar Ajami"}, 2.66},
        {{"Nasimi", "Azadlig Prospekti"}, 2.1},
        {{"Azadlig Prospekti", "Nasimi"}, 2.1},
        {{"Azadlig Prospekti", "Darnagul"}, 1.1},
        {{"Darnagul", "Azadlig Prospekti"}, 1.1},
        {{"28 May", "Nizami"}, 1.7},
        {{"Nizami", "28 May"}, 1.7},



        {{"Khojasan", "Avtovagzal"}, 2.0},
        {{"Avtovagzal", "Memar Acemi 2"}, 2.0},
        {{"Memar Acemi 2", "8 Noyabr"}, 1.5},
        {{"8 Noyabr","Memar Acemi 2"}, 1.5},
        {{"Memar Acemi 2","Avtovagzal"}, 2.0},
        {{"Avtovagzal","Khojasan"}, 2.0},
        \

        {{"Jafar Jabbarly", "Hatai"}, 1.0},
        {{"Hatai", "Jafar Jabbarly"}, 1.0}
    };
}

void TransitNetwork::setup_routes() {
    routes_->emplace("Red", Route(
                                {"Icheri Sheher", "Sahil", "28 May", "Ganjlik", "Nariman Narimanov",
                                 "Bakmil", "Ulduz", "Koroglu", "Kara Karaev", "Neftchilar",
                                 "Khalglar Dostlugu", "Ahmedli", "Azi Aslanov"},
                                "Bakmil",
                                13,
                                12
                                ));
    routes_->emplace("Green", Route(
                                  {"Darnagul", "Azadlig Prospekti", "Nasimi", "Memar Ajami", "20 January",
                                   "Inshaatchilar", "Elmlar Akademiyasy", "Nizami", "28 May",
                                   "Ganjlik", "Nariman Narimanov", "Bakmil", "Ulduz", "Koroglu",
                                   "Kara Karaev", "Neftchilar", "Khalglar Dostlugu", "Ahmedli", "Azi Aslanov"},
                                  "Bakmil",
                                  19,
                                  18
                                  ));
    routes_->emplace("Purple", Route(
                                   {"Khojasan", "Avtovagzal","Memar Acemi 2","8 Noyabr"},
                                   "Khojasan",
                                   4,
                                   3
                                   ));
    routes_->emplace("Light Green", Route(
                                        {"Jafar Jabbarly", "Hatai"},
                                        "Hatai",
                                        2,
                                        1,
                                        true
                                        ));
}
