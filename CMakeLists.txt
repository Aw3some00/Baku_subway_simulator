cmake_minimum_required(VERSION 3.15)
project(BakuSubwaySimulator CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
add_executable(subway
        main.cpp
        TransitNetwork.cpp
        TrainOperator.cpp
        SimulationManager.cpp
        SystemMonitor.cpp
)
find_package(Threads REQUIRED)
target_link_libraries(subway Threads::Threads)