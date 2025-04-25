
#include "TransitNetwork.h"

TransitNetwork::TransitNetwork() {
    setup_routes();
    setup_distances();
}

const std::map<std::string, TransitNetwork::Route>& TransitNetwork::routes() const {
    return routes_;
}

double TransitNetwork::distance_between(const std::string& start, const std::string& end) const {
    auto key = std::make_pair(start, end);
    auto reverse_key = std::make_pair(end, start);
    if (stop_distances_.count(key)) return stop_distances_.at(key);
    if (stop_distances_.count(reverse_key)) return stop_distances_.at(reverse_key);
    return 0.0;
}

void TransitNetwork::setup_distances() {
    stop_distances_ = {
        {{"Icheri Sheher", "Sahil"}, 0.9}, {{"Sahil", "Icheri Sheher"}, 0.9},
        {{"Sahil", "28 May"}, 0.5}, {{"28 May", "Sahil"}, 0.5},
        {{"28 May", "Ganjlik"}, 1.6}, {{"Ganjlik", "28 May"}, 1.6},
        {{"Ganjlik", "Nariman Narimanov"}, 2.1}, {{"Nariman Narimanov", "Ganjlik"}, 2.1},
        {{"Nariman Narimanov", "Bakmil"}, 1.3}, {{"Bakmil", "Nariman Narimanov"}, 1.3},
        {{"Bakmil", "Ulduz"}, 1.9}, {{"Ulduz", "Bakmil"}, 1.9},
        {{"Ulduz", "Koroglu"}, 2.3}, {{"Koroglu", "Ulduz"}, 2.3},
        {{"Koroglu", "Kara Karaev"}, 1.8}, {{"Kara Karaev", "Koroglu"}, 1.8},
        {{"Kara Karaev", "Neftchilar"}, 1.2}, {{"Neftchilar", "Kara Karaev"}, 1.2},
        {{"Neftchilar", "Khalglar Dostlugu"}, 1.1}, {{"Khalglar Dostlugu", "Neftchilar"}, 1.1},
        {{"Khalglar Dostlugu", "Ahmedli"}, 1.6}, {{"Ahmedli", "Khalglar Dostlugu"}, 1.6},
        {{"Ahmedli", "Azi Aslanov"}, 1.3}, {{"Azi Aslanov", "Ahmedli"}, 1.3},
        {{"Nizami", "Elmlar Akademiyasy"}, 1.2}, {{"Elmlar Akademiyasy", "Nizami"}, 1.2},
        {{"Elmlar Akademiyasy", "Inshaatchilar"}, 0.9}, {{"Inshaatchilar", "Elmlar Akademiyasy"}, 0.9},
        {{"Inshaatchilar", "20 January"}, 1.3}, {{"20 January", "Inshaatchilar"}, 1.3},
        {{"20 January", "Memar Ajami"}, 1.8}, {{"Memar Ajami", "20 January"}, 1.8},
        {{"Memar Ajami", "Nasimi"}, 2.66}, {{"Nasimi", "Memar Ajami"}, 2.66},
        {{"Nasimi", "Azadlig Prospekti"}, 2.1}, {{"Azadlig Prospekti", "Nasimi"}, 2.1},
        {{"Azadlig Prospekti", "Darnagul"}, 1.1}, {{"Darnagul", "Azadlig Prospekti"}, 1.1},
        {{"28 May", "Nizami"}, 1.7}, {{"Nizami", "28 May"}, 1.7},
        {{"Khojasan", "Avtovagzal"}, 2.0}, {{"Avtovagzal", "Khojasan"}, 2.0},
        {{"Avtovagzal", "8 Noyabr"}, 1.5}, {{"8 Noyabr", "Avtovagzal"}, 1.5},
        {{"Jafar Jabbarly", "Hatai"}, 1.0}, {{"Hatai", "Jafar Jabbarly"}, 1.0}
    };
}

void TransitNetwork::setup_routes() {
    routes_["Red"] = {
        {"Icheri Sheher", "Sahil", "28 May", "Ganjlik", "Nariman Narimanov",
         "Bakmil", "Ulduz", "Koroglu", "Kara Karaev", "Neftchilar",
         "Khalglar Dostlugu", "Ahmedli", "Azi Aslanov"},
        "Bakmil",
        std::vector<std::mutex>(12),
        std::vector<std::mutex>(12)
    };
    routes_["Green"] = {
        {"Darnagul", "Azadlig Prospekti", "Nasimi", "Memar Ajami", "20 January",
         "Inshaatchilar", "Elmlar Akademiyasy", "Nizami", "28 May",
         "Ganjlik", "Nariman Narimanov", "Bakmil", "Ulduz", "Koroglu",
         "Kara Karaev", "Neftchilar", "Khalglar Dostlugu", "Ahmedli", "Azi Aslanov"},
        "Bakmil",
        std::vector<std::mutex>(18),
        std::vector<std::mutex>(18)
    };
    routes_["Purple"] = {
        {"Khojasan", "Avtovagzal", "8 Noyabr"},
        "Khojasan",
        std::vector<std::mutex>(2),
        std::vector<std::mutex>(2)
    };
    routes_["Light Green"] = {
        {"Jafar Jabbarly", "Hatai"},
        "N/A",
        std::vector<std::mutex>(1),
        std::vector<std::mutex>(1),
        true
    };
}
