#pragma once
#include <string>

std::string getCurrentTime();

struct SensorData {
    std::string Sensor_ID;
    std::string Product_name;
    std::string Sensor_type;
    double value;
    std::string timestamp;

    // Dong goi JSON
    std::string toJson();
};
