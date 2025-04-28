Baku Subway Simulator
The Baku Subway Simulator is a C++17 console application that simulates the operations of the Baku Metro system. It models train movements, passenger boarding/alighting, fuel and incident costs, and multi-threaded operations using mutexes for thread safety. The simulator provides an animated interface for initializing the system and configuring train counts per line.
Features

Multi-threaded Train Operations: Simulates multiple trains on four lines (Red, Green, Purple, Light Green) with realistic travel times and passenger dynamics.
Passenger Management: Tracks boarding and alighting passengers, adjusting for high-traffic hours (7-9 AM, 5-7 PM).
Cost Tracking: Monitors fuel costs (0.1 AZN/km), incident costs (50 AZN per fault), and maintenance costs (500 AZN), calculating net profit based on ticket revenue (0.5 AZN/passenger).
Animated Interface: Displays a welcome animation with ASCII art, a progress bar, and fan animations during initialization.
Thread Safety: Uses mutexes to manage concurrent access to stations and output.
Memory Management: Uses pointers (std::string*, std::vector<T>*, std::map<K,V>*) for dynamically-sized data structures with proper memory management (Rule of Five).

Project Structure
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

Prerequisites

C++17 Compiler: GCC, Clang, or MSVC.
Qt Creator: For building and running the project (configured with Qt 5 or 6).
Standard Library: Must support <thread>, <mutex>, <chrono>, and <random>.
Operating System: Developed on macOS (arm64), compatible with Windows and Linux (uses system("cls") or system("clear") for console clearing).

Building the Project
Using Qt Creator

Open the Project:
Launch Qt Creator.
Open the project file: /Users/fuad/qt_projects/metro_final/Baku_subway_simulator/BakuSubwaySimulator.pro.


Configure the Kit:
Ensure a C++17-compatible kit (e.g., Clang or GCC) is selected.
Set the build directory (e.g., /Users/fuad/qt_projects/metro_final/Baku_subway_simulator/build).


Build and Run:
Click "Build" → "Build Project" or press Cmd+B (macOS).
Click "Run" or press Cmd+R to start the simulator.



Using CMake (Alternative)

Clone the Repository (if hosted on GitHub):git clone https://github.com/Aw3some00/Baku_subway_simulator.git
cd Baku_subway_simulator


Create a Build Directory:mkdir build
cd build


Generate Build Files with CMake:cmake ..


Build the Project:cmake --build .


Run the Simulator:./BakuSubwaySimulator



Manual Compilation
g++ -std=c++17 main.cpp SimulationManager.cpp TrainOperator.cpp TransitNetwork.cpp SystemMonitor.cpp commands.cpp -o BakuSubwaySimulator
./BakuSubwaySimulator

Usage

Welcome Screen: The simulator starts with an animated ASCII art display and system initialization.
Configure Trains: Enter the number of trains (0-10) for each line (Red, Green, Purple, Light Green) when prompted.
Simulation: The simulator runs for 10 minutes, with each train operating in 5-minute shifts. It logs:
Train movements (arrival, departure, travel times).
Passenger boarding/alighting (randomized, doubled during peak hours).
Faults (1% chance per segment, 50 AZN cost).
Fuel costs (0.1 AZN/km).


Summary: After 10 minutes, a financial summary is printed, showing total passengers, revenue, expenses, and net profit.

Example Output
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

Modifications
The project was updated to use pointers for all dynamically-sized data structures:

TrainOperator:
std::string route_name_ → std::string* route_name_.
Added Rule of Five for memory management.
Fixed copy assignment to avoid reassigning std::mutex& output_mutex_.


TransitNetwork:
std::map<std::string, Route> routes_ → std::map<std::string*, Route>* routes_.
std::map<std::pair<std::string, std::string>, double> stop_distances_ → std::map<std::pair<std::string*, std::string*>, double>* stop_distances_.
Route::std::vector<std::string> stops → std::vector<std::string>* stops.
Route::std::string hub → std::string* hub.
Added Rule of Five for Route and TransitNetwork.
Fixed distance_between to avoid invalid pair dereferencing.
Updated Route constructor to initialize std::vector<std::mutex> by size instead of copying.


Impact:
Improved explicit memory control for large datasets.
Required updates to member functions for pointer dereferencing (e.g., *route_name_, *stops).
Ensured deep copies and move semantics to prevent memory leaks.



Notes

Memory Management: The use of raw pointers requires careful handling. Consider std::unique_ptr for safer memory management in future iterations.
Thread Safety: Mutexes ensure safe concurrent access to stations and output. The stop_locks map in TrainOperator.cpp uses std::string keys, which are compatible with the pointer-based stops vector.
Scalability: The simulator supports up to 10 trains per line. Increase max_riders in TrainData for larger passenger loads.
Extensibility: Add new routes or stations by updating TransitNetwork::setup_routes() and setup_distances().

Troubleshooting

Compilation Errors: Ensure C++17 is enabled (-std=c++17). Verify all header and source files are included.
Memory Leaks: Use Valgrind (valgrind ./BakuSubwaySimulator) to check for leaks.
Qt Creator Issues: Ensure the .pro file includes all source files and the correct compiler kit is selected.

Contributing
Contributions are welcome! Please fork the repository, create a feature branch, and submit a pull request. Ensure code follows C++17 standards and includes proper memory management.
License
This project is licensed under the MIT License. See the LICENSE file for details (not provided in the original project; consider adding one).
Contact
For issues or suggestions, contact the project maintainer at GitHub Issues.
