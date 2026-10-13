#include "../include/SensorGateway.h"
#include "../include/SensorData.h"
#include <sstream>
#include <iostream>
#include <vector>

SensorGateway::SensorGateway() = default;

void SensorGateway::registerHandler(std::shared_ptr<Domain::IoT::ISensorReadingHandler> handler) {
    std::lock_guard<std::mutex> lock(gatewayMutex);
    this->readingHandler = std::move(handler);
}

void SensorGateway::registerDevice(const std::string& sensorId, const std::string& containerId) {
    std::lock_guard<std::mutex> lock(gatewayMutex);
    devices.emplace(sensorId, Domain::IoT::SensorDevice(sensorId, containerId));
}

bool SensorGateway::processRawString(const std::string& rawPayload) {
    // 1. Tách chuỗi thô theo dấu phẩy: "SENSOR-01,5.2" hoặc "SENSOR-01,5.2,TIMESTAMP"
    std::stringstream ss(rawPayload);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, ',')) {
        tokens.push_back(item);
    }

    if (tokens.size() < 2) {
        std::cerr << "[SensorGateway-ACL] LOI: Dinh dang chuoi tho khong hop le: " << rawPayload << "\n";
        return false;
    }

    std::string sensorId = tokens[0];
    double tempVal = 0.0;
    try {
        tempVal = std::stod(tokens[1]);
    } catch (const std::exception& e) {
        std::cerr << "[SensorGateway-ACL] LOI: Khong the chuyen doi gia tri nhiet do: " << tokens[1] << "\n";
        return false;
    }

    std::string timestamp = (tokens.size() >= 3 && !tokens[2].empty()) ? tokens[2] : getCurrentTime();

    // Khóa mutex: đảm bảo mỗi lần chỉ một dữ liệu vào Shipment và bảo vệ danh sách thiết bị
    std::lock_guard<std::mutex> lock(gatewayMutex);

    // Tự động đăng ký thiết bị nếu chưa có
    if (devices.find(sensorId) == devices.end()) {
        devices.emplace(sensorId, Domain::IoT::SensorDevice(sensorId));
    }

    Domain::IoT::SensorDevice& device = devices.at(sensorId);

    // 2. Kiểm tra tính hợp lệ: Loại bỏ giá trị vô lý (dưới -50°C hoặc trên 60°C)
    if (!device.isReadingValid(tempVal)) {
        std::cerr << "[SensorGateway-ACL] LOAI BO: Nhiet do vo ly ngoai khoang [-50, 60] do C (" 
                  << tempVal << " C) tu " << sensorId << "\n";
        return false;
    }

    // 3. Cập nhật thiết bị (cập nhật nhiệt độ, heartbeat và chuyển trạng thái ONLINE)
    device.updateReading(tempVal);

    // 4. Ánh xạ thành Value Object TemperatureReading
    Domain::IoT::TemperatureReading reading(sensorId, tempVal, timestamp);

    // 5. Đẩy sang phân hệ Shipment của TV1
    if (readingHandler) {
        readingHandler->onReading(reading);
        return true;
    }

    return false;
}

void SensorGateway::checkDeviceTimeouts(int timeoutSeconds) {
    std::lock_guard<std::mutex> lock(gatewayMutex);
    for (auto& pair : devices) {
        if (pair.second.checkTimeout(timeoutSeconds)) {
            std::cout << "[SensorGateway] CANH BAO: Cam bien " << pair.first 
                      << " khong co du lieu qua " << timeoutSeconds 
                      << " giay! Trang thai chuyen sang: OFFLINE.\n";
        }
    }
}

Domain::IoT::DeviceStatus SensorGateway::getDeviceStatus(const std::string& sensorId) const {
    std::lock_guard<std::mutex> lock(gatewayMutex);
    auto it = devices.find(sensorId);
    if (it != devices.end()) {
        return it->second.getStatus();
    }
    return Domain::IoT::DeviceStatus::OFFLINE;
}

std::string SensorGateway::getDeviceStatusString(const std::string& sensorId) const {
    Domain::IoT::DeviceStatus st = getDeviceStatus(sensorId);
    return (st == Domain::IoT::DeviceStatus::ONLINE) ? "ONLINE" : "OFFLINE";
}
