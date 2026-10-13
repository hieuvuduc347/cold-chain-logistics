#pragma once
#include <string>
#include "SensorData.h"

class Sensor {
protected:
    std::string id;

public:
    explicit Sensor(std::string sensorId);
    virtual ~Sensor() = default;

    const std::string& getId() const;
    virtual SensorData readData() = 0;
};