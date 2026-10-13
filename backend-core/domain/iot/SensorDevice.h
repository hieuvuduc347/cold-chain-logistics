#pragma once
#include <string>
#include <chrono>

namespace Domain {
namespace IoT {

enum class DeviceStatus {
    ONLINE,
    OFFLINE
};

class SensorDevice {
private:
    std::string sensorId;
    std::string containerId;
    DeviceStatus status;
    double lastTemperature;
    std::chrono::steady_clock::time_point lastHeartbeat;

public:
    static constexpr double MIN_VALID_TEMP = -50.0;
    static constexpr double MAX_VALID_TEMP = 60.0;

    explicit SensorDevice(std::string sensorId, std::string containerId = "");

    const std::string& getSensorId() const;
    const std::string& getContainerId() const;
    DeviceStatus getStatus() const;
    std::string getStatusString() const;
    double getLastTemperature() const;
    
    void setContainerId(const std::string& newContainerId);
    void setStatus(DeviceStatus newStatus);

    // Kiểm tra tính hợp lệ của giá trị nhiệt độ (loại bỏ dưới -50°C hoặc trên 60°C)
    bool isReadingValid(double temp) const;

    // Cập nhật giá trị đo: nếu hợp lệ thì lưu nhiệt độ, cập nhật heartbeat và chuyển ONLINE
    bool updateReading(double temp);

    // Kiểm tra mất kết nối: quá timeoutSeconds không có dữ liệu thì chuyển sang OFFLINE
    bool checkTimeout(int timeoutSeconds);
};

} // namespace IoT
} // namespace Domain
