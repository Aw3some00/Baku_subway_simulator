#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <string>
#include <random>
#include <fstream>

std::mutex station_mutex;
std::condition_variable station_cv;
int trains_in_tunnel = 0;
const int max_trains_in_tunnel = 2;

std::mutex independent_mutex;
std::condition_variable independent_cv;
int independent_trains_in_tunnel = 0;
const int max_independent_trains = 2;

struct StationState {
    int forward_trains = 0;
    int backward_trains = 0;
    int passengers = 0;
};
std::unordered_map<std::string, StationState> station_states;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(0, 1);
std::uniform_int_distribution<> passenger_dis(0, 1500);

std::ofstream violet_file("violet.txt", std::ios::app);
std::ofstream yellow_file("yellow.txt", std::ios::app);
std::mutex file_mutex;
std::mutex console_mutex;

void loadingScreen() {
    std::cout << "🚇 Bakı Metropoliteni" << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::string messages[] = {
        "🔄 Loading...",
        "🔄 Loading routes...",
        "🔄 Checking trains...",
        "✅ System is ready!",
        "🚇 Starting"
    };

    for (const std::string& message : messages) {
        std::cout << message << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void print_message(const std::string& message, bool is_purple, bool is_yellow, const std::string& direction) {
    std::string full_message = message + " [Direction: " + direction + "]";
    std::lock_guard<std::mutex> lock(file_mutex);
    if (is_purple) {
        violet_file << full_message << "\n";
        violet_file.flush();
    } else if (is_yellow) {
        yellow_file << full_message << "\n";
        yellow_file.flush();
    } else {
        std::lock_guard<std::mutex> console_lock(console_mutex);
        std::cout << full_message << "\n";
    }
}

void Station(const std::string& train_id, const std::string& name, bool is_forward, bool is_purple, bool is_yellow, const std::string& direction) {
    std::unique_lock<std::mutex> lock(is_purple || is_yellow ? independent_mutex : station_mutex);
    auto& cv = (is_purple || is_yellow) ? independent_cv : station_cv;
    int& tunnel_count = (is_purple || is_yellow) ? independent_trains_in_tunnel : trains_in_tunnel;
    const int max_tunnel = (is_purple || is_yellow) ? max_independent_trains : max_trains_in_tunnel;

    cv.wait(lock, [&]() {
        StationState& state = station_states[name];
        if (name == "Khojasan" || name == "IcheriSheher") {
            return tunnel_count < max_tunnel && (state.forward_trains + state.backward_trains) == 0;
        }
        return tunnel_count < max_tunnel &&
               (is_forward ? state.forward_trains == 0 : state.backward_trains == 0);
    });

    tunnel_count++;
    StationState& state = station_states[name];
    if (is_forward) state.forward_trains++;
    else state.backward_trains++;


    if (state.passengers == 0) {
        state.passengers = passenger_dis(gen);
    }

    std::string message = "Train " + train_id + " arrived at station " + name +
                          " (Trains in tunnel: " + std::to_string(tunnel_count) + ", Passengers: " + std::to_string(state.passengers) + ")";
    print_message(message, is_purple, is_yellow, direction);
    lock.unlock();


    int stop_time = 4 + (state.passengers / 100);
    std::this_thread::sleep_for(std::chrono::seconds(stop_time));

    lock.lock();
    message = "Train " + train_id + " departed from station " + name;
    print_message(message, is_purple, is_yellow, direction);

    if (is_forward) state.forward_trains--;
    else state.backward_trains--;
    tunnel_count--;


    state.passengers = std::max(0, state.passengers - passenger_dis(gen));

    message = "Train " + train_id + " left tunnel (Trains in tunnel: " + std::to_string(tunnel_count) + ")";
    print_message(message, is_purple, is_yellow, direction);
    lock.unlock();

    cv.notify_all();
}

void visit_bakmil(const std::string& train_id, const std::string& direction) {
    std::unique_lock<std::mutex> lock(station_mutex);

    station_cv.wait(lock, [&]() {
        StationState& state = station_states["Bakmil"];
        return trains_in_tunnel < max_trains_in_tunnel && (state.forward_trains + state.backward_trains) < 2; // До 2 поездов в депо
    });

    trains_in_tunnel++;
    StationState& state = station_states["Bakmil"];
    if (direction == "Icheri Sheher" || direction == "Darnagul") {
        state.forward_trains++;
    } else {
        state.backward_trains++;
    }

    std::string message = "Train " + train_id + " arrived at depot Bakmil for maintenance (Trains in tunnel: " + std::to_string(trains_in_tunnel) + ")";
    print_message(message, false, false, direction);
    lock.unlock();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    lock.lock();
    message = "Train " + train_id + " departed from depot Bakmil after maintenance";
    print_message(message, false, false, direction);

    if (direction == "Icheri Sheher" || direction == "Darnagul") {
        state.forward_trains--;
    } else {
        state.backward_trains--;
    }
    trains_in_tunnel--;
    message = "Train " + train_id + " left tunnel (Trains in tunnel: " + std::to_string(trains_in_tunnel) + ")";
    print_message(message, false, false, direction);
    lock.unlock();

    station_cv.notify_all();
}

#define DEFINE_STATION(name) \
    void name(const std::string& train_id, bool is_forward, bool is_purple, const std::string& direction) { Station(train_id, #name, is_forward, is_purple, false, direction); } \
    void name##_yellow(const std::string& train_id, bool is_forward, bool is_purple, const std::string& direction) { Station(train_id, #name, is_forward, is_purple, true, direction); }

DEFINE_STATION(HaziAslanov)
DEFINE_STATION(Ahmadli)
DEFINE_STATION(KhalglarDostlugu)
DEFINE_STATION(Neftchilar)
DEFINE_STATION(GaraGarayev)
DEFINE_STATION(Koroglu)
DEFINE_STATION(Ulduz)
DEFINE_STATION(Bakmil)
DEFINE_STATION(Narimanov)
DEFINE_STATION(Ganjlik)
DEFINE_STATION(May28)
DEFINE_STATION(Sahil)
DEFINE_STATION(IcheriSheher)
DEFINE_STATION(JafarJabbarly)
DEFINE_STATION(Nizami)
DEFINE_STATION(ElmlerAkademiyasi)
DEFINE_STATION(Inshaatchilar)
DEFINE_STATION(Jan20)
DEFINE_STATION(MemarAjami)
DEFINE_STATION(Nasimi)
DEFINE_STATION(AzadligProspekti)
DEFINE_STATION(Avtovagzal)
DEFINE_STATION(Khojasan)
DEFINE_STATION(Darnagul)
DEFINE_STATION(Noyabr8)
DEFINE_STATION(AvtoVogzal)
DEFINE_STATION(MemarAjami2)
DEFINE_STATION(KhojasanPurple)
DEFINE_STATION(Khatai)

void train(const std::string& train_id, const std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>>& route, bool is_forward, bool is_purple, bool is_yellow, const std::string& direction) {
    for (size_t i = 0; i < route.size(); ++i) {
        route[i](train_id, is_forward, is_purple, direction);
        if (i < route.size() - 1) {
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
}

void travel_from_depot(const std::string& train_id,
                       const std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>>& route_to_end,
                       const std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>>& route_to_start,
                       const std::string& depot_name,
                       bool is_purple,
                       bool is_yellow,
                       const std::string& start_station,
                       const std::string& end_station) {
    std::string direction;
    int max_trips = is_purple ? 2 : (is_yellow ? 1 : 2);

    if (is_yellow) {
        direction = end_station;
        print_message("Train " + train_id + " starts its journey from depot " + depot_name + ".", is_purple, is_yellow, direction);

        int trips = 0;
        bool going_to_khatai = true;

        while (trips < max_trips) {
            if (going_to_khatai) {
                print_message("Train " + train_id + " heads towards " + end_station + " (Trip " + std::to_string(trips + 1) + ").", is_purple, is_yellow, direction);
                train(train_id, route_to_end, true, is_purple, is_yellow, direction);
                direction = start_station;
                print_message("Train " + train_id + " reached " + end_station + " and now heads towards " + start_station + ".", is_purple, is_yellow, direction);
                going_to_khatai = false;
            } else {
                print_message("Train " + train_id + " heads towards " + start_station + " (Trip " + std::to_string(trips + 1) + ").", is_purple, is_yellow, direction);
                train(train_id, route_to_start, true, is_purple, is_yellow, direction);
                direction = end_station;
                print_message("Train " + train_id + " reached " + start_station + " and now heads towards " + end_station + ".", is_purple, is_yellow, direction);
                going_to_khatai = true;
                trips++;
            }
        }

        print_message("Train " + train_id + " has completed " + std::to_string(max_trips) + " trips and is returning to depot " + depot_name + ".", is_purple, is_yellow, direction);
        if (going_to_khatai) {
            train(train_id, route_to_end, true, is_purple, is_yellow, "Depot");
            train(train_id, route_to_start, true, is_purple, is_yellow, "Depot");
        } else {
            train(train_id, route_to_start, true, is_purple, is_yellow, "Depot");
        }
    } else if (is_purple) {
        direction = end_station;
        print_message("Train " + train_id + " starts its journey from depot " + depot_name + ".", is_purple, is_yellow, direction);

        int trips = 0;
        bool going_to_noyabr = true;

        while (trips < max_trips) {
            if (going_to_noyabr) {
                print_message("Train " + train_id + " heads towards " + end_station + " (Trip " + std::to_string(trips + 1) + ").", is_purple, is_yellow, direction);
                train(train_id, route_to_end, true, is_purple, is_yellow, direction);
                direction = start_station;
                print_message("Train " + train_id + " reached " + end_station + " and now heads towards " + start_station + ".", is_purple, is_yellow, direction);
                going_to_noyabr = false;
            } else {
                print_message("Train " + train_id + " heads towards " + start_station + " (Trip " + std::to_string(trips + 1) + ").", is_purple, is_yellow, direction);
                train(train_id, route_to_start, true, is_purple, is_yellow, direction);
                direction = end_station;
                print_message("Train " + train_id + " reached " + start_station + " and now heads towards " + end_station + ".", is_purple, is_yellow, direction);
                going_to_noyabr = true;
                trips++;
            }
        }

        print_message("Train " + train_id + " has completed " + std::to_string(max_trips) + " trips and is returning to depot " + depot_name + ".", is_purple, is_yellow, direction);
        if (going_to_noyabr) {
            train(train_id, route_to_end, true, is_purple, is_yellow, "Depot");
            train(train_id, route_to_start, true, is_purple, is_yellow, "Depot");
        } else {
            train(train_id, route_to_start, true, is_purple, is_yellow, "Depot");
        }
    } else {
        direction = (dis(gen) == 0 ? "Hazi Aslanov" : end_station);
        print_message("Train " + train_id + " starts its journey from depot " + depot_name + ".", is_purple, is_yellow, direction);

        int trips = 0;
        bool to_hazi = direction == "Hazi Aslanov";

        while (trips < max_trips) {
            if (to_hazi) {
                print_message("Train " + train_id + " heads towards Hazi Aslanov (Trip " + std::to_string(trips + 1) + ").", is_purple, is_yellow, direction);
                train(train_id, route_to_end, true, is_purple, is_yellow, direction);
                direction = end_station;
                print_message("Train " + train_id + " turns around and travels back from Hazi Aslanov.", is_purple, is_yellow, direction);
                train(train_id, route_to_start, true, is_purple, is_yellow, direction);
            } else {
                print_message("Train " + train_id + " heads towards " + end_station + " (Trip " + std::to_string(trips + 1) + ").", is_purple, is_yellow, direction);
                train(train_id, route_to_start, true, is_purple, is_yellow, direction);
                direction = "Hazi Aslanov";
                print_message("Train " + train_id + " turns around and travels back from " + end_station + ".", is_purple, is_yellow, direction);
                train(train_id, route_to_end, true, is_purple, is_yellow, direction);
            }
            trips++;
        }

        print_message("Train " + train_id + " has completed " + std::to_string(max_trips) + " trips and is returning to depot " + depot_name + ".", is_purple, is_yellow, direction);
        if (to_hazi) {
            train(train_id, route_to_end, true, is_purple, is_yellow, "Depot");
            std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> reverse_route_to_start = route_to_start;
            std::reverse(reverse_route_to_start.begin() + 1, reverse_route_to_start.end());
            train(train_id, reverse_route_to_start, true, is_purple, is_yellow, "Depot");
        } else {
            train(train_id, route_to_start, true, is_purple, is_yellow, "Depot");
            std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> reverse_route_to_end = route_to_end;
            std::reverse(reverse_route_to_end.begin() + 1, reverse_route_to_end.end());
            train(train_id, reverse_route_to_end, true, is_purple, is_yellow, "Depot");
        }
    }
}

void manage_bakmil_visits(const std::vector<std::string>& train_ids) {
    std::uniform_int_distribution<> train_dis(0, train_ids.size() - 1);
    while (true) {
        int train_idx = train_dis(gen);
        std::string train_id = train_ids[train_idx];
        bool is_red = train_id.find("red") != std::string::npos;
        std::string direction = is_red ? "Icheri Sheher" : "Darnagul";

        visit_bakmil(train_id, direction);
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}

int main() {
    const int red_trains = 10;
    const int green_trains = 10;
    const int purple_trains = 1;
    const int yellow_trains = 1;

    if (!violet_file.is_open()) {
        std::cerr << "Error: Could not open violet.txt" << std::endl;
        return 1;
    }
    if (!yellow_file.is_open()) {
        std::cerr << "Error: Could not open yellow.txt" << std::endl;
        return 1;
    }
    std::cout << "Welcome to BakuSubway simulator\n";
    std::cout << "Green and red line info will be in console, Yellow and Violet line will be shown in Yellow.txt, Violet.txt files \n";
    loadingScreen();

    std::vector<std::thread> trains;
    std::vector<std::string> bakmil_trains;

    std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> red_route_to_hazi = {
        Bakmil, Ulduz, Koroglu, GaraGarayev, Neftchilar, KhalglarDostlugu, Ahmadli, HaziAslanov
    };
    std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> red_route_to_icheri = {
        Bakmil, Narimanov, Ganjlik, May28, Sahil, IcheriSheher
    };

    std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> green_route_to_hazi = {
        Bakmil, Ulduz, Koroglu, GaraGarayev, Neftchilar, KhalglarDostlugu, Ahmadli, HaziAslanov
    };
    std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> green_route_to_darnagul = {
        Bakmil, Narimanov, Ganjlik, May28, Nizami, ElmlerAkademiyasi, Inshaatchilar, Jan20,
        MemarAjami, Nasimi, AzadligProspekti, Avtovagzal, Khojasan, Darnagul
    };

    std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> purple_route_to_noyabr = {
        KhojasanPurple, AvtoVogzal, MemarAjami2, Noyabr8
    };
    std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> purple_route_to_khojasan = {
        Noyabr8, AvtoVogzal, MemarAjami2, KhojasanPurple
    };

    std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> yellow_route_to_khatai = {
        JafarJabbarly_yellow, Khatai_yellow
    };
    std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> yellow_route_to_jafar = {
        Khatai_yellow, JafarJabbarly_yellow
    };

    for (int i = 0; i < red_trains; i++) {
        std::string train_id = "red_train" + std::to_string(i + 1);
        bakmil_trains.push_back(train_id);
        trains.push_back(std::thread([train_id, &red_route_to_hazi, &red_route_to_icheri]() {
            travel_from_depot(train_id, red_route_to_hazi, red_route_to_icheri, "Bakmil", false, false, "Hazi Aslanov", "Icheri Sheher");
        }));
    }

    for (int i = 0; i < green_trains; i++) {
        std::string train_id = "green_train" + std::to_string(i + 1);
        bakmil_trains.push_back(train_id);
        trains.push_back(std::thread([train_id, &green_route_to_hazi, &green_route_to_darnagul]() {
            travel_from_depot(train_id, green_route_to_hazi, green_route_to_darnagul, "Bakmil", false, false, "Hazi Aslanov", "Darnagul");
        }));
    }

    for (int i = 0; i < purple_trains; i++) {
        std::string train_id = "purple_train" + std::to_string(i + 1);
        trains.push_back(std::thread([train_id, &purple_route_to_noyabr, &purple_route_to_khojasan]() {
            travel_from_depot(train_id, purple_route_to_noyabr, purple_route_to_khojasan, "KhojasanPurple", true, false, "KhojasanPurple", "8 Noyabr");
        }));
    }

    for (int i = 0; i < yellow_trains; i++) {
        std::string train_id = "yellow_train" + std::to_string(i + 1);
        trains.push_back(std::thread([train_id, &yellow_route_to_khatai, &yellow_route_to_jafar]() {
            travel_from_depot(train_id, yellow_route_to_khatai, yellow_route_to_jafar, "Jafar Jabbarly", false, true, "Jafar Jabbarly", "Khatai");
        }));
    }

    std::thread bakmil_manager(manage_bakmil_visits, bakmil_trains);

    for (auto& t : trains) {
        t.join();
    }

    bakmil_manager.join();

    std::cout << "All trains have returned to depots. Program terminated." << std::endl;
    return 0;
}