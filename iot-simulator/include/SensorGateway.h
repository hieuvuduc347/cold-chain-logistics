#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>
#include "../../backend-core/domain/iot/ISensorReadingHandler.h"
#include "../../backend-core/domain/iot/SensorDevice.h"

class SensorGateway {
private:
    std::shared_ptr<Domain::IoT::ISensorReadingHandler> readingHandler;
    std::unordered_map<std::string, Domain::IoT::SensorDevice> devices;
    mutable std::mutex gatewayMutex;

public:
    SensorGateway();

    // Đăng ký handler nhận dữ liệu (từ phân hệ Shipment của TV1)
    void registerHandler(std::shared_ptr<Domain::IoT::ISensorReadingHandler> handler);

    // Đăng ký thiết bị cảm biến vào gateway
    void registerDevice(const std::string& sensorId, const std::string& containerId = "");

    // Anti-Corruption Layer: Nhận chuỗi thô dạng "SENSOR-01,5.2", parse và chuyển thành TemperatureReading
    bool processRawString(const std::string& rawPayload);

    // Kiểm tra cảm biến mất kết nối (quá timeoutSeconds không nhận được dữ liệu thì báo OFFLINE)
    void checkDeviceTimeouts(int timeoutSeconds);

    // Lấy thông tin trạng thái thiết bị
    Domain::IoT::DeviceStatus getDeviceStatus(const std::string& sensorId) const;
    std::string getDeviceStatusString(const std::string& sensorId) const;
};
