#pragma once
#include <string>

std::string getCurrentTime();

struct SensorData {
    std::string sensorId;
    double temperature;
    std::string timestamp;

    // Xuất chuỗi thô chuẩn vi điều khiển: "SENSOR-01,5.2" hoặc "SENSOR-01,5.2,TIMESTAMP"
    std::string toRawString() const;
    std::string toRawStringWithTime() const;

    // Đóng gói JSON thô (nếu cần log)
    std::string toJson() const;
};
