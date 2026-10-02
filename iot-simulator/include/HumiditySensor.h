#pragma once
#include "Sensor.h"
#include <random>

class HumiditySensor : public Sensor {
    private:
        std::mt19937 gen;
        std::uniform_real_distribution <> dist;
    public:
        HumiditySensor(std::string ID, std::string Product_Name);

        SensorData readData() override;
};
