#include "../include/TemperatureSensor.h"
#include <chrono>

TemperatureSensor::TemperatureSensor(std::string sensorId, double minRange, double maxRange)
    : Sensor(std::move(sensorId)),
      gen(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())),
      dist(minRange, maxRange) {}

SensorData TemperatureSensor::readData() {
    SensorData data;
    data.sensorId = this->id;
    data.temperature = dist(gen);
    data.timestamp = getCurrentTime();
    return data;
}

SensorData TemperatureSensor::readValue(double specificTemp) {
    SensorData data;
    data.sensorId = this->id;
    data.temperature = specificTemp;
    data.timestamp = getCurrentTime();
    return data;
}