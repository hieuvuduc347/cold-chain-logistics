#pragma once
#include "Sensor.h"
#include <random>

class TemperatureSensor : public Sensor {
private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist;

public:
    explicit TemperatureSensor(std::string sensorId, double minRange = 2.0, double maxRange = 6.0);

    SensorData readData() override;
    SensorData readValue(double specificTemp);
};