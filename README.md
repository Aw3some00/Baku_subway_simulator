# Baku Subway Simulator 🚆🚉

[![Build Status](https://img.shields.io/badge/build-passing-white)](https://shields.io/)
[![License](https://img.shields.io/badge/license-MIT-blue)](https://opensource.org/licenses/MIT)
![Last Commit](https://img.shields.io/github/last-commit/your-username/baku-subway-simulator)
![Version](https://img.shields.io/badge/version-v1.0.0-blue)

![Stars](https://img.shields.io/github/stars/your-username/baku-subway-simulator?style=social)

## Table of Contents
- [Description](#-description)
- [Program Functionality](#-program-functionality)
- [Input Data and Example](#-input-data)
- [Features](#-features)
- [Installation on g++](#-installation-on-g-compiler)
- [Installation on CMake](#-installation-on-cmake-compiler)
- [Technical Details](#technical-details-)
- [Functions Description](#functions-description-)
  - [TrainOperator](#trainoperator)
  - [TransitNetwork](#transitnetwork)
- [Contributing](#-contributing)
- [License](#-license)
- [Contact](#-contact)
- [Support Us on Patreon](#-support-us-on-patreon)

## 🚆 Description

**Make Sure That**:
- You have a C++17-compliant compiler (e.g., GCC, MSVC, Clang).
- Qt 5+ is installed for `qmake` builds (optional for CMake).
- CMake 3.15+ is installed for CLion.
- Your terminal supports UTF-8 (`export LANG=en_US.UTF-8` on Linux/macOS).
- CLion or Qt Creator is installed for the best experience.

Welcome to the **Baku Subway Simulator**! 🎉 This C++ console application simulates the Baku Metro system, modeling train movements, passenger dynamics, and station interactions across four lines: Red, Green, Purple, and Light Green. For the best experience, run it in **CLion’s Terminal tab** or **Qt Creator’s Terminal tab** to ensure full emoji support (🚆, 🟢, ✅) and proper console clearing. **Avoid running natively in Qt Creator’s output window** due to limited emoji and clearing support. Running natively in CLion’s output window is possible but not recommended for optimal immersion.

The program:
1. Simulates concurrent train operations on multiple lines.
2. Manages passenger boarding/alighting with randomized traffic.
3. Logs events with colorful emojis for visual feedback.
4. Supports building as static or dynamic libraries for integration.

---

## ⚡ Program Functionality

- **Multithreaded Train Simulation**: Runs each train in a separate thread with mutex-protected platform access 🔒.
- **Dynamic Passenger Management**: Simulates randomized passenger boarding 🧳 and alighting 🚶 based on station traffic.
- **Emoji-Enhanced Logging**: Uses Unicode emojis (🚆, 🔴, ✅) for clear, visually appealing logs 📜.
- **Fault Detection**: Simulates random train faults (1% chance per stop) with cost penalties 🛠️.
- **Real-Time Feedback**: Displays train movements, passenger updates, and shift completions in real time ⏳.
- **Library Support**: Can be built as static or dynamic libraries for use in other applications 📚.

### Step-by-Step Process:
1. **Input Train Data** 📥: Enter the number of trains for each line and a simulation start time.
2. **Simulate Train Operations** 🚄: Trains depart from hubs (e.g., Bakmil), move between stops, and handle passengers.
3. **Log Events** 📜: Detailed logs show shift starts, passenger updates, and faults with emojis.
4. **Handle Faults** ⚠️: Random faults may halt trains, logging repair costs.
5. **Display Results** 🎉: Logs summarize shift completions, passenger counts, and simulation end.

---

## 📥 Input Data

The program accepts:
1. **Number of Trains (N)**: Positive integers for each line (Red, Green, Purple, Light Green).
2. **Simulation Start Time**: A time in `HH:MM` format (e.g., `22:30`).

### 📥📥 Example Input Data

**Enter the number of trains for Red line:** 2  
**Enter the number of trains for Green line:** 7  
**Enter the number of trains for Purple line:** 3  
**Enter the number of trains for Light Green line:** 1  
**Enter the simulation start time (HH:MM):** 22:30

---

### 📥📥 Entered Data:
- **Red Line**: 2 trains
- **Green Line**: 7 trains
- **Purple Line**: 3 trains
- **Light Green Line**: 1 train
- **Start Time**: 22:30

---

### 🚀 Simulation in Progress...

    🚉 ==== Baku Metro ==== 🚆
          |  [  🚄  ]  |  🚈  |  
          |==============|======|  
          |  [  🚃  ]  |  🚇  |  
          |===🛤️🛤️🛤️🛤️===|======|  
          🚄 Welcome Aboard! 🚉"

**Simulation started!**

---

### 🔥 Results:
```
Train 1: 🚆 Train 1 (Red) departing from Bakmil 🚉
Train 1: ⏰ Train 1 (Red) shift 1 started 🔴 ✅
Train 1: 👥 Train 1 (Red) 🔴: 10 alighted 🚶, 20 boarded 🧳, current: 30 passengers ✅
Train 11: ⏰ Train 11 (Green) shift 1 started 🟢 ✅
Train 11: 👥 Train 11 (Green) 🟢: 15 alighted 🚶, 25 boarded 🧳, current: 40 passengers ✅
```

---

## ⚡ Features
- **Concurrent Train Operations**: Multithreaded simulation of multiple trains 🚄.
- **Passenger Dynamics**: Randomized boarding/alighting based on station traffic 🧳.
- **Visual Logging**: Emoji-driven logs for train events (🚆, 🔴, ✅) 📜.
- **Library Builds**: Supports static/dynamic libraries for integration 📚.

---

## 🛠️ Installation on g++ Compiler

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/baku-subway-simulator.git
   ```
2. **Navigate to Project Folder**:
   ```bash
   cd baku-subway-simulator
   ```
3. **Compile**:
   ```bash
   g++ -std=c++17 -pthread -o subway main.cpp TransitNetwork.cpp TrainOperator.cpp SimulationManager.cpp SystemMonitor.cpp
   # macOS (Apple Silicon): add -arch arm64
   ```
4. **Run (Preferred: Terminal)**:
   ```bash
   ./subway  # Linux/macOS
   subway.exe  # Windows
   ```
   **Note**: For best experience, run in CLion’s or Qt Creator’s Terminal tab to ensure emoji support and console clearing.
5. **P.S. Hear the train whistle? 🚆**

---

## 🛠️ Installation on CMake Compiler

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/baku-subway-simulator.git
   ```
2. **Navigate to Project Folder**:
   ```bash
   cd baku-subway-simulator
   ```
3. **Create Build Folder**:
   ```bash
   mkdir build && cd build
   ```
4. **Run CMake**:
   ```bash
   cmake ..
   cmake --build .
   ```
5. **Run (Preferred: CLion Terminal)**:
   - Open CLion, load the project, and open the Terminal tab (`Alt+F12`).
   - Run:
     ```bash
     ./subway  # Linux/macOS
     subway.exe  # Windows
     ```
   - **Native CLion Run**: Possible via `Shift+F10`, but **not recommended** due to limited emoji and clearing support in the output window.
6. **P.S. All aboard the Baku Metro! 🚉**

### Qt Creator Instructions
1. **Open Project**:
   - In Qt Creator, select `File > Open File or Project` > `Baku_subway_simulator.pro`.
2. **Configure**:
   - Ensure `QMAKE_CXXFLAGS += -utf-8` in `.pro`:
     ```pro
     QT -= gui
     CONFIG += c++17 console
     CONFIG -= app_bundle
     SOURCES += main.cpp TransitNetwork.cpp TrainOperator.cpp SimulationManager.cpp SystemMonitor.cpp
     QMAKE_CXXFLAGS += -utf-8
     ```
3. **Build**:
   - `Build > Build Project` or `Ctrl+B`.
4. **Run (Preferred: Terminal)**:
   - Open Qt Creator’s terminal or a native terminal.
   - Run:
     ```bash
     ./subway  # Linux/macOS
     subway.exe  # Windows
     ```
   - **Native Qt Creator Run**: **Not recommended** due to poor emoji rendering and lack of console clearing in the output window.

---

## Technical Details 🚄
The simulator uses:
- **Multithreading**: `std::thread` and `std::mutex` for concurrent train operations.
- **Randomization**: `std::random_device` and `std::mt19937` for passenger and fault simulation.
- **UTF-8 Emojis**: Supports emojis (🚆, 🔴, ✅), requiring UTF-8 terminal encoding.
- **Console Clearing**: Uses `clear_display()` with ANSI codes (`\033[2J\033[1;1H`) or `system("clear")`/`system("cls")`, best supported in CLion/Qt Creator Terminal.
- **Library Builds**:
  - **Static Library**: Produces `.a` (Linux/macOS) or `.lib` (Windows).
  - **Dynamic Library**: Produces `.so` (Linux), `.dylib` (macOS), or `.dll` (Windows).

### Example Library Configuration (`.pro`):
- Static:
  ```pro
  TEMPLATE = lib
  CONFIG += staticlib
  ```
- Dynamic:
  ```pro
  TEMPLATE = lib
  CONFIG += shared
  ```

---

## Functions Description 🚆

### TrainOperator
1. **`TrainOperator::secure_log(const std::string& message)`**  
   Logs thread-safe messages with train ID and emojis (e.g., 🔴 ✅).
2. **`TrainOperator::start_journey()`**  
   Simulates train journeys, handling shifts, passengers, and faults.
3. **`TrainOperator::return_to_hub(...)`**  
   Returns trains to hubs after shifts, locking platforms.
4. **`TrainOperator::estimate_travel_time(double distance)`**  
   Calculates travel time (60 seconds per km).
5. **`clear_display()`**  
   Clears console using ANSI codes or `system("clear")`/`system("cls")`.

### TransitNetwork
1. **`TransitNetwork::add_route(...)`**  
   Adds metro lines with stops, hubs, and platform locks.
2. **`TransitNetwork::distance_between(const std::string& stop1, const std::string& stop2)`**  
   Returns distance between stops (default: 1 km).
3. **`TransitNetwork::routes()`**  
   Returns map of routes (Red, Green, Purple, Light Green).

---

## 🤝 Contributing

We welcome contributions! To contribute:
1. Fork the repository 🍴.
2. Create a branch (`git checkout -b feature/YourFeature`) 🌱.
3. Commit changes (`git commit -m "Add YourFeature"`) 📝.
4. Push branch (`git push origin feature/YourFeature`) 🚀.
5. Open a pull request 🔄.

Follow coding standards and include tests for new features. Reference issue numbers in PR descriptions.

---

## 📝 License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT). 🎉

You are free to use, modify, and distribute this project, retaining the original license and copyright notice.

---

## 📞 Contact

For inquiries, reach out to Email and TG:


###Email:.
**fuadrashidov06@gmail.com**.
###TG:.
**Aw3some_0**.

---

## ♥️ Support Us on Patreon

Love the project? Support us with a donation! 🙌

[![Support on Patreon](https://img.shields.io/badge/Support_Patreon-FF424D?style=for-the-badge&logo=patreon&logoColor=white)](https://www.patreon.com/your-patreon)
