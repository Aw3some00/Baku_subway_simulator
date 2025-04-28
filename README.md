# Baku Subway Simulator

**Make Sure That**:
- You have a C++17-compliant compiler (e.g., GCC, MSVC, Clang).
- Qt 5+ is installed for `qmake` builds (optional for CMake).
- CMake 3.15+ is installed for CLion.
- Your terminal supports UTF-8 for emoji display (`export LANG=en_US.UTF-8` on Linux/macOS).
- CLion or Qt Creator is installed for the best experience.

## 🚆 Overview
The **Baku Subway Simulator** is a C++ console application that simulates the Baku Metro system. It models train movements, passenger dynamics, and station interactions across four lines: Red, Green, Purple, and Light Green. Run it in CLion’s Terminal or Qt Creator’s terminal for full immersion with emoji-rich logs and console clearing.

### ✨ Features
- **Multithreaded Trains**: Concurrent train operations with mutex-protected platforms.
- **Passenger Simulation**: Randomized boarding/alighting based on station traffic.
- **Emoji Logs**: Visual feedback with 🚆, 🟢, ✅, and more.
- **Library Support**: Build as static or dynamic libraries for integration.

## 🛠️ Quick Start Guide

### Prerequisites
- **Compiler**: GCC, MSVC, or Clang (C++17).
- **Qt**: For `qmake` builds (optional).
- **CMake**: For CLion builds.
- **OS**: Windows, Linux, or macOS.

### Clone the Project
```bash
git clone https://github.com/your-repo/baku-subway-simulator.git
cd baku-subway-simulator
```

### Build and Run

#### Preferred Method: CLion Terminal
1. **Open in CLion**:
   - File > Open > Select `baku-subway-simulator` folder.
2. **Configure CMake** (if not using `qmake`):
   - Create `CMakeLists.txt`:
     ```cmake
     cmake_minimum_required(VERSION 3.15)
     project(BakuSubwaySimulator CXX)
     set(CMAKE_CXX_STANDARD 17)
     add_executable(subway
         main.cpp
         TransitNetwork.cpp
         TrainOperator.cpp
         SimulationManager.cpp
         SystemMonitor.cpp
     )
     find_package(Threads REQUIRED)
     target_link_libraries(subway Threads::Threads)
     ```
3. **Build**:
   - `Ctrl+F9` or `Build > Build Project`.
4. **Run in Terminal Tab**:
   - Open Terminal (`Alt+F12`).
   - Run:
     ```bash
     ./build/subway  # macOS/Linux
     build\subway.exe  # Windows
     ```
   - Input train counts (e.g., `2` for Red, `7` for Green, `3` for Purple, `1` for Light Green) and time (e.g., `22:30`).

#### Alternative: Qt Creator Terminal
1. **Open in Qt Creator**:
   - File > Open File or Project > Select `Baku_subway_simulator.pro`.
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
4. **Run in Terminal**:
   - Open Qt Creator’s terminal or native terminal.
   - Run:
     ```bash
     ./subway  # macOS/Linux
     subway.exe  # Windows
     ```

#### Any PC (Terminal)
1. **Install Dependencies**:
   - **Windows**: Install MinGW or MSVC (via Visual Studio Build Tools).
   - **Ubuntu**: `sudo apt install g++ qt5-qmake make`.
   - **macOS**: `brew install qt`.
2. **Build**:
   ```bash
   qmake Baku_subway_simulator.pro
   make
   ```
   Or manually:
   ```bash
   g++ -std=c++17 -pthread -o subway main.cpp TransitNetwork.cpp TrainOperator.cpp SimulationManager.cpp SystemMonitor.cpp
   # macOS (Apple Silicon): add -arch arm64
   ```
3. **Run**:
   ```bash
   ./subway  # macOS/Linux
   subway.exe  # Windows
   ```

### Library Builds
- **Static Library**:
  ```pro
  TEMPLATE = lib
  CONFIG += staticlib
  ```
  ```bash
  qmake && make
  ```
  Output: `libBaku_subway_simulator.a` (Linux/macOS) or `.lib` (Windows).

- **Dynamic Library**:
  ```pro
  TEMPLATE = lib
  CONFIG += shared
  ```
  ```bash
  qmake && make
  ```
  Output: `.so` (Linux), `.dylib` (macOS), or `.dll` (Windows).

## 📺 Example Output
```
Train 1: 🚆 Train 1 (Red) departing from Bakmil 🚉
Train 1: ⏰ Train 1 (Red) shift 1 started 🔴 ✅
Train 1: 👥 Train 1 (Red) 🔴: 10 alighted 🚶, 20 boarded 🧳, current: 30 passengers ✅
```

## ⚠️ Troubleshooting
- **Console Not Clearing**:
  - Use `clear_display()` with ANSI codes:
    ```cpp
    void clear_display() { std::cout << "\033[2J\033[1;1H" << std::flush; }
    ```
  - Run in CLion/Qt Creator Terminal tab for `system("clear")` support.
- **Emoji Issues**:
  - Set `LANG=en_US.UTF-8`:
    ```bash
    export LANG=en_US.UTF-8
    ```
  - Save files as UTF-8 in CLion/Qt Creator.
- **Build Errors**:
  - Ensure `QMAKE_CXXFLAGS += -utf-8` in `.pro`.
  - Verify source files match artifacts (e.g., `TrainOperator.cpp`).

## 🤝 Contributing
Fork, branch (`feature/YourFeature`), commit, and open a PR. See [CONTRIBUTING.md](#) for details.

## 📜 License
MIT License. See `LICENSE` for details.

## 💬 Contact
Open an issue on GitHub or email [your-email@example.com](mailto:your-email@example.com).
