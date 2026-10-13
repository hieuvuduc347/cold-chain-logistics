#include "SensorDevice.h"
#include <utility>

namespace Domain {
namespace IoT {

SensorDevice::SensorDevice(std::string sensorId, std::string containerId)
    : sensorId(std::move(sensorId)),
      containerId(std::move(containerId)),
      status(DeviceStatus::ONLINE),
      lastTemperature(0.0),
      lastHeartbeat(std::chrono::steady_clock::now()) {}

const std::string& SensorDevice::getSensorId() const {
    return sensorId;
}

const std::string& SensorDevice::getContainerId() const {
    return containerId;
}

DeviceStatus SensorDevice::getStatus() const {
    return status;
}

std::string SensorDevice::getStatusString() const {
    return (status == DeviceStatus::ONLINE) ? "ONLINE" : "OFFLINE";
}

double SensorDevice::getLastTemperature() const {
    return lastTemperature;
}

void SensorDevice::setContainerId(const std::string& newContainerId) {
    this->containerId = newContainerId;
}

void SensorDevice::setStatus(DeviceStatus newStatus) {
    this->status = newStatus;
}

bool SensorDevice::isReadingValid(double temp) const {
    // Loại bỏ giá trị vô lý: dưới -50°C hoặc trên 60°C
    return (temp >= MIN_VALID_TEMP && temp <= MAX_VALID_TEMP);
}

bool SensorDevice::updateReading(double temp) {
    if (!isReadingValid(temp)) {
        return false;
    }
    this->lastTemperature = temp;
    this->lastHeartbeat = std::chrono::steady_clock::now();
    this->status = DeviceStatus::ONLINE;
    return true;
}

bool SensorDevice::checkTimeout(int timeoutSeconds) {
    auto now = std::chrono::steady_clock::now();
    auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastHeartbeat).count();
    if (elapsedMs >= static_cast<long long>(timeoutSeconds) * 1000) {
        if (this->status != DeviceStatus::OFFLINE) {
            this->status = DeviceStatus::OFFLINE;
            return true; // Trạng thái vừa thay đổi sang OFFLINE
        }
    }
    return false;
}

} // namespace IoT
} // namespace Domain
