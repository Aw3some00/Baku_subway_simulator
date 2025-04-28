#include "SimulationManager.h"
#include <iostream>
#include <thread>
#include <limits>
#include <sstream>
#include <iomanip>
#include <chrono>

void clear_display() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

SimulationManager::SimulationManager()
    : network_(), running(true), end_time_(), monitor_(), output_mutex_(), operators_() {}

void SimulationManager::show_welcome() {
    const char* transit_art[] = {
        "  🚉 ==== Baku Metro ==== 🚆",
        "  |  [  🚄  ]  |  🚈  |  ",
        "  |==============|======|  ",
        "  |  [  🚃  ]  |  🚇  |  ",
        "  |===🛤️🛤️🛤️🛤️===|======|  ",
        "  🚄 Welcome Aboard! 🚉"
    };

    clear_display();
    std::cout << "\n\n\n  🚉 Baku Metro Simulation 🚆\n\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    clear_display();
    std::cout << "\n\n";
    for (const char* line : transit_art) {
        std::string faint(line);
        for (char& c : faint) {
            if (c != ' ' && c != '|' && c != '=' && c != '[' && c != ']') c = ' ';
        }
        std::cout << "  " << faint << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        clear_display();
        std::cout << "\n\n";
        for (const char* prev_line : transit_art) {
            if (prev_line == line) break;
            std::cout << "  " << prev_line << "\n";
        }
        std::cout << "  " << line << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "\n  Initializing system... ⏳\n\n";

    const char* fans[] = {"🌀", "🔄", "⚙️"};
    for (int i = 0; i < 6; ++i) {
        clear_display();
        std::cout << "\n\n";
        for (const char* line : transit_art) {
            std::cout << "  " << line << "\n";
        }
        std::cout << "\n  Initializing system... ⏳\n";
        std::cout << "  Fans: " << fans[(i % 3)] << " " << fans[(i + 1) % 3] << " " << fans[(i + 2) % 3] << "\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    clear_display();
}

void SimulationManager::collect_train_counts(int& red_trains, int& green_trains, int& purple_trains, int& light_green_trains) {
    clear_display();
    std::cout << "🚉 Baku Metro Control Center 🚆\n";
    std::cout << "Enter the number of trains for each line (0-10 per line):\n\n";

    auto get_input = [](const std::string& line_name, const std::string& emoji) {
        int count;
        while (true) {
            std::cout << emoji << " " << line_name << " line: ";
            std::cin >> count;
            if (std::cin.fail() || count < 0 || count > 10) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "❌ Invalid input! Please enter a number between 0 and 10.\n";
            } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return count;
            }
        }
    };

    red_trains = get_input("Red", "🟥");
    green_trains = get_input("Green", "🟩");
    purple_trains = get_input("Purple", "🟪");
    light_green_trains = get_input("Light Green", "🟦");

    clear_display();
    std::cout << "🚄 Preparing Metro System... 🚉\n\n";
    const char* loading_steps[] = {
        "Loading Red line routes... 🟥",
        "Loading Green line routes... 🟩",
        "Loading Purple line routes... 🟪",
        "Loading Light Green routes... 🟦",
        "Initializing stations... 🛤️",
        "Configuring train schedules... 🚆",
        "Setting up mutex locks... 🔒",
        "Calibrating system monitor... 📊",
        "Finalizing train assignments... 🚄",
        "Starting metro operations... 🎉"
    };
    const char* spinners[] = {"🔄", "⏳", "⚙️", "🔧"};
    const char* steam[] = {"💨", "🌫️"};
    const char* fans[] = {"🌀", "🔄", "⚙️"};
    int step_count = sizeof(loading_steps) / sizeof(loading_steps[0]);
    int percent_per_step = 100 / step_count;

    for (int i = 0; i < step_count; ++i) {
        int percent = (i + 1) * percent_per_step;
        std::cout << "[";
        int pos = (percent / 10) % 6;
        for (int j = 0; j < 6; ++j) {
            if (j == pos) std::cout << "🚆";
            else if (j < percent / 20) std::cout << "=";
            else std::cout << " ";
        }
        std::cout << "] " << percent << "% " << steam[i % 2] << "\n";
        std::cout << spinners[i % 4] << " " << loading_steps[i] << "\n";
        std::cout << "Fans: " << fans[(i % 3)] << " " << fans[(i + 1) % 3] << " " << fans[(i + 2) % 3] << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (i < step_count - 1) {
            clear_display();
            std::cout << "🚄 Preparing Metro System... 🚉\n\n";
        }
    }

    clear_display();
    std::cout << "✅ Train Configuration Confirmed:\n";
    std::cout << "🟥 Red line: " << red_trains << " trains 🚆\n";
    std::cout << "🟩 Green line: " << green_trains << " trains 🚆\n";
    std::cout << "🟪 Purple line: " << purple_trains << " trains 🚆\n";
    std::cout << "🟦 Light Green line: " << light_green_trains << " trains 🚆\n\n";
    std::cout << "Preparing to set simulation end time... ⏰\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

void SimulationManager::collect_end_time() {
    clear_display();
    std::cout << "🚉 Baku Metro Control Center 🚆\n";
    std::cout << "Enter simulation end time (HH:MM, 24-hour format, e.g., 14:30): ";
    std::string time_input;
    int hours, minutes;
    char colon;
    std::getline(std::cin, time_input);
    std::istringstream iss(time_input);
    if (!(iss >> hours >> colon >> minutes) || colon != ':' || hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        std::cout << "Invalid time format. Using default end time (in 10 minutes).\n";
        hours = 0;
        minutes = 10;
    }
    std::cout << "Simulation will end at " << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes << ".\n";

    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    struct tm now_tm = *localtime(&now_time_t);

    struct tm end_tm = now_tm;
    end_tm.tm_hour = hours;
    end_tm.tm_min = minutes;
    end_tm.tm_sec = 0;
    end_time_ = std::chrono::system_clock::from_time_t(mktime(&end_tm));

    if (end_time_ < now) {
        end_time_ += std::chrono::hours(24);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    clear_display();
}

void SimulationManager::start_operations() {
    show_welcome();
    int red_trains, green_trains, purple_trains, light_green_trains;
    collect_train_counts(red_trains, green_trains, purple_trains, light_green_trains);
    collect_end_time();

    std::vector<std::thread> threads;
    int train_id = 1;

    auto add_trains = [&](const std::string& line, int count) {
        for (int i = 0; i < count; ++i) {
            bool is_forward = (i % 2 == 0);
            operators_.emplace_back(train_id++, line, is_forward, network_, output_mutex_);
            threads.emplace_back(&TrainOperator::start_journey, &operators_.back());
        }
    };

    add_trains("Red", red_trains);
    add_trains("Green", green_trains);
    add_trains("Purple", purple_trains);
    add_trains("Light Green", light_green_trains);

    // Monitor end time and stop operators if time is reached
    std::thread monitor_thread([this]() {
        while (running) {
            if (std::chrono::system_clock::now() >= end_time_) {
                stop_operators();
                running = false;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    for (auto& thread : threads) {
        thread.join();
    }

    if (monitor_thread.joinable()) {
        monitor_thread.join();
    }

    monitor_.print_summary();
}

void SimulationManager::stop_operators() {
    for (auto& op : operators_) {
        op.running= false;
    }
}
