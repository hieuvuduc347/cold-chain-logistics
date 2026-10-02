#pragma once
#include <string>
#include "SensorData.h"

class Sensor { // Ham cha
    protected:
        std::string ID;
        std::string Product_name;
    public:
        Sensor (std::string Sensor_ID, std::string Product_name);
        virtual ~Sensor() = default;

        virtual SensorData readData() = 0; // tao ham data de cac chuong trinh con tu dnh nghia
};