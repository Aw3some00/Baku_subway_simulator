# Baku Subway Simulator

The Baku Subway Simulator is a C++17 console application simulating the operations of the Baku Metro system. It models train movements, passenger dynamics, fuel and incident costs, and multi-threaded operations using mutexes for thread safety. The simulator includes an animated interface for system initialization and train configuration.

## Features
- **Multi-threaded Train Operations**: Simulates trains on four lines (Red, Green, Purple, Light Green) with realistic travel times and passenger boarding/alighting.
- **Passenger Management**: Tracks passengers, doubling counts during peak hours (7-9 AM, 5-7 PM).
- **Cost Tracking**: Monitors fuel (0.1 AZN/km), incident (50 AZN), and maintenance (500 AZN) costs, calculating net profit (0.5 AZN/passenger).
- **Animated Interface**: Displays ASCII art, progress bars, and fan animations during initialization.
- **Thread Safety**: Uses mutexes for concurrent station and output access.
- **Memory Management**: Employs raw pointers (`std::string*`, `std::vector<T>*`, `std::map<K,V>*`) with Rule of Five for dynamic data structures.

## Project Structure
```
Baku_subway_simulator/
├── main.cpp
├── SimulationManager.h
├── SimulationManager.cpp
├── TrainOperator.h
├── TrainOperator.cpp
├── TransitNetwork.h
├── TransitNetwork.cpp
├── SystemMonitor.h
├── SystemMonitor.cpp
├── commands.cpp
├── BakuSubwaySimulator.pro
├── README.md
```

## Prerequisites
- **C++17 Compiler**: GCC, Clang, or MSVC.
- **Qt Creator**: For building and running (Qt 5 or 6).
- **Standard Library**: Requires `<thread>`, `<mutex>`, `<chrono>`, `<random>`.
- **OS**: Developed on macOS (arm64), compatible with Windows/Linux (uses `system("cls")` or `system("clear")`).

## Building the Project
### Using Qt Creator
1. **Open Project**:
   - Launch Qt Creator.
   - Open `/Users/fuad/qt_projects/metro_final/Baku_subway_simulator/BakuSubwaySimulator.pro`.
2. **Configure Kit**:
   - Select a C++17-compatible kit (e.g., Clang/GCC).
   - Set build directory (e.g., `/Users/fuad/qt_projects/metro_final/Baku_subway_simulator/build`).
3. **Build and Run**:
   - Build: `Build` → `Build Project` or `Cmd+B` (macOS).
   - Run: `Cmd+R`.

### Using CMake
1. **Clone Repository** (if hosted):
   ```bash
   git clone https://github.com/Aw3some00/Baku_subway_simulator.git
   cd Baku_subway_simulator
   ```
2. **Create Build Directory**:
   ```bash
   mkdir build
   cd build
   ```
3. **Generate Build Files**:
   ```bash
   cmake ..
   ```
4. **Build**:
   ```bash
   cmake --build .
   ```
5. **Run**:
   ```bash
   ./BakuSubwaySimulator
   ```

### Manual Compilation
```bash
g++ -std=c++17 main.cpp SimulationManager.cpp TrainOperator.cpp TransitNetwork.cpp SystemMonitor.cpp commands.cpp -o BakuSubwaySimulator
./BakuSubwaySimulator
```

## Usage
1. **Welcome Screen**: Displays animated ASCII art and initialization.
2. **Configure Trains**: Input number of trains (0-10) per line (Red, Green, Purple, Light Green).
3. **Simulation**: Runs for 10 minutes (5-minute shifts), logging:
   - Train movements (arrival/departure, travel times).
   - Passenger boarding/alighting (randomized, peak-hour doubling).
   - Faults (1% chance per segment, 50 AZN).
   - Fuel costs (0.1 AZN/km).
4. **Summary**: Prints total passengers, revenue, expenses, and net profit.

## Example Output
```
🚉 Baku Metro Simulation 🚆

[=====🚆 ] 100% 💨
🎉 Starting metro operations...

🚉 Baku Metro Control Center 🚆
Enter the number of trains for each line (0-10 per line):

🟥 Red line: 2
🟩 Green line: 2
🟪 Purple line: 1
🟦 Light Green line: 1

🚆 Train 1 (Red) departing from Bakmil 🚉
...

Total passengers served: 1234
Revenue: 617.00 AZN
Fuel expenses: 245.50 AZN
Incident expenses: 100.00 AZN
Maintenance cost: 500.00 AZN
Total expenses: 845.50 AZN
Net profit: -228.50 AZN
```

## Recent Changes
- **TransitNetwork::setup_routes**: Updated to use `routes_->emplace` for efficient insertion with `std::string*` keys, ensuring correct `Route` constructor calls.
- **TrainOperator::start_journey**: Fixed to use `route_name_` (a `std::string*`) directly when accessing `routes_->at(route_name_)`.
- **TrainOperator::is_high_traffic_time**: Corrected `localtime(&current_time)` to fix time calculation bug.
- **Memory Management**: Ensured proper deallocation of `std::string*` keys in `TransitNetwork` destructor.

## Notes
- **Memory Management**: Uses raw pointers as requested. Consider `std::unique_ptr` for safer memory handling in future versions.
- **Thread Safety**: Mutexes ensure concurrent access to stations and output. `stop_locks` map in `TrainOperator.cpp` uses `std::string` keys, compatible with `Route::stops`.
- **Scalability**: Supports up to 10 trains per line. Adjust `TrainData::max_riders` for larger passenger loads.
- **Extensibility**: Add routes/stations by updating `TransitNetwork::setup_routes` and `setup_distances`.

## Troubleshooting
- **Compilation Errors**: Ensure C++17 (`-std=c++17`) and all files are included.
- **Memory Leaks**: Use Valgrind (`valgrind ./BakuSubwaySimulator`) to detect leaks.
- **Qt Creator Issues**: Verify `.pro` file includes all sources and correct compiler kit.

## Contributing
Fork the repository, create a feature branch, and submit a pull request. Ensure C++17 compliance and proper memory management.

## License
MIT License (add `LICENSE` file for formal licensing).

## Contact
For issues, open a GitHub issue at [github.com/Aw3some00/Baku_subway_simulator/issues](https://github.com/Aw3some00/Baku_subway_simulator/issues).