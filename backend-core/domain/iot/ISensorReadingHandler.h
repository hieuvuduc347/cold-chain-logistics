#pragma once
#include "TemperatureReading.h"

namespace Domain {
namespace IoT {

class ISensorReadingHandler {
public:
    virtual ~ISensorReadingHandler() = default;

    // Callback nhận dữ liệu đo nhiệt độ đã được chuẩn hóa từ Sensing Subsystem
    virtual void onReading(const TemperatureReading& reading) = 0;
};

} // namespace IoT
} // namespace Domain
