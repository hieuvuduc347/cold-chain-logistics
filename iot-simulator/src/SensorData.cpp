#include "../include/SensorData.h"
#include <sstream>
#include <chrono>
#include <iomanip>

std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm ltime{};
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&ltime, &currentTime);
#else
    localtime_r(&currentTime, &ltime);
#endif
    std::stringstream ss;
    ss << std::put_time(&ltime, "%Y-%m-%dT%H:%M:%S");
    return ss.str();
}

std::string SensorData::toRawString() const {
    std::stringstream ss;
    ss << sensorId << "," << std::fixed << std::setprecision(2) << temperature;
    return ss.str();
}

std::string SensorData::toRawStringWithTime() const {
    std::stringstream ss;
    ss << sensorId << "," << std::fixed << std::setprecision(2) << temperature << "," << timestamp;
    return ss.str();
}

std::string SensorData::toJson() const {
    std::stringstream ss;
    ss << "{\n"
       << "  \"Sensor_ID\": \"" << sensorId << "\",\n"
       << "  \"Sensor_Type\": \"Temperature\",\n"
       << "  \"Temperature\": " << std::fixed << std::setprecision(2) << temperature << ",\n"
       << "  \"Timestamp\": \"" << timestamp << "\"\n"
       << "}";
    return ss.str();
}