#pragma once
#include <string>
#include <stdexcept>
#include <utility>

namespace Domain {
namespace IoT {

class TemperatureReading {
private:
    std::string sensorId;
    double celsius;
    std::string timestamp;

public:
    TemperatureReading(std::string sensorId, double celsius, std::string timestamp)
        : sensorId(std::move(sensorId)), celsius(celsius), timestamp(std::move(timestamp)) {
        if (this->sensorId.empty()) {
            throw std::invalid_argument("SensorId khong duoc de trong.");
        }
    }

    const std::string& getSensorId() const { return sensorId; }
    double getCelsius() const { return celsius; }
    const std::string& getTimestamp() const { return timestamp; }

    bool operator==(const TemperatureReading& other) const {
        return sensorId == other.sensorId &&
               celsius == other.celsius &&
               timestamp == other.timestamp;
    }

    bool operator!=(const TemperatureReading& other) const {
        return !(*this == other);
    }
};

} // namespace IoT
} // namespace Domain
