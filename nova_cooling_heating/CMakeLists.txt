cmake_minimum_required(VERSION 3.15)
project(nopva_cooling_heating)

set(CMAKE_CXX_STANDARD 14)

add_executable(nova_cooling_heating nova_cooling_heating.ino Cooler.hpp FanController.cpp FanController.hpp HeatControl.cpp ota.cpp ota.hpp HeatControl.hpp MockHeater.cpp MockHeater.hpp mqtt.hpp mqtt.cpp RelayHeater.hpp RelayHeater.cpp)