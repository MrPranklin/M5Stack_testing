cmake_minimum_required(VERSION 3.15)
project(m5_light)

set(CMAKE_CXX_STANDARD 14)

add_executable(m5_light LightControl.hpp LightControl.cpp m5lcd.cpp m5lcd.hpp mqtt.hpp mqtt.cpp mqtt_topics.h ota.hpp ota.cpp state_n.hpp m5_light.ino)
