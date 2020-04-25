cmake_minimum_required(VERSION 3.15)
project(m5_temp)

set(CMAKE_CXX_STANDARD 14)

add_executable(m5_temp Cooler.hpp DHT22_C.cpp DHT22_C.hpp FanController.hpp FanController.cpp HeatControl.hpp HeatControl.cpp Heater.hpp MockHeater.hpp MockHeater.cpp HumSensor.hpp m5lcd.cpp m5lcd.hpp mqtt.hpp mqtt.cpp mqtt_topics.h ota.hpp ota.cpp state_n.hpp TempSensor.hpp m5_temp.ino)