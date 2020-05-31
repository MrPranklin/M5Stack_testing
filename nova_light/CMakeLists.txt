cmake_minimum_required(VERSION 3.15)
project(nova_light)

set(CMAKE_CXX_STANDARD 14)

add_executable(nova_light nova_light.ino ArtificialSource.hpp LightControl.cpp ota.cpp ota.hpp LightControl.hpp  mqtt.hpp mqtt.cpp MqttTopics.h LedController.cpp LedController.hpp ServoBlindController.hpp ServoBlindController.cpp)