#include "../include/Sensor.h"
#include <utility>

Sensor::Sensor(std::string sensorId) : id(std::move(sensorId)) {}

const std::string& Sensor::getId() const {
    return id;
}