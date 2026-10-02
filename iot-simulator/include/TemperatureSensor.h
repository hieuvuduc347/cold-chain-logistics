#pragma once
#include "Sensor.h"
#include <random>

class TemperatureSensor : public Sensor {
    private:
        std::mt19937 gen;
        std::uniform_real_distribution <> dist;
    public:
        TemperatureSensor (std::string Sensor_ID, std::string Product_name);

        SensorData readData() override;
};