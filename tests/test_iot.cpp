#include <iostream>
#include <cassert>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include <atomic>

#include "../backend-core/domain/iot/TemperatureReading.h"
#include "../backend-core/domain/iot/ISensorReadingHandler.h"
#include "../backend-core/domain/iot/SensorDevice.h"
#include "../iot-simulator/include/SensorGateway.h"
#include "../iot-simulator/include/SensorSimulator.h"

using namespace Domain::IoT;

// Mock Handler để ghi nhận reading từ Gateway
class TestMockHandler : public ISensorReadingHandler {
public:
    std::vector<TemperatureReading> receivedReadings;
    std::atomic<int> callCount{0};

    void onReading(const TemperatureReading& reading) override {
        receivedReadings.push_back(reading);
        callCount++;
    }

    void clear() {
        receivedReadings.clear();
        callCount = 0;
    }
};

void testTemperatureReadingValueObject() {
    std::cout << "[RUN] Test 1: TemperatureReading Value Object... ";
    TemperatureReading r1("SENSOR-01", 4.5, "2026-10-13T16:00:00");
    assert(r1.getSensorId() == "SENSOR-01");
    assert(r1.getCelsius() == 4.5);
    assert(r1.getTimestamp() == "2026-10-13T16:00:00");

    TemperatureReading r2("SENSOR-01", 4.5, "2026-10-13T16:00:00");
    assert(r1 == r2);

    TemperatureReading r3("SENSOR-02", 4.5, "2026-10-13T16:00:00");
    assert(r1 != r3);

    // Kiểm tra ném ngoại lệ khi sensorId rỗng
    bool caught = false;
    try {
        TemperatureReading rInvalid("", 5.0, "time");
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught && "Phai nem ngoai le khi sensorId rong");

    std::cout << "PASSED\n";
}

void testSensorDeviceValidation() {
    std::cout << "[RUN] Test 2: SensorDevice Entity & Validation [-50C, 60C]... ";
    SensorDevice dev("SENSOR-01", "CONTAINER-A");
    assert(dev.getStatus() == DeviceStatus::ONLINE);
    assert(dev.getContainerId() == "CONTAINER-A");

    // Giá trị hợp lệ
    assert(dev.isReadingValid(4.0) == true);
    assert(dev.isReadingValid(-50.0) == true);
    assert(dev.isReadingValid(60.0) == true);
    assert(dev.updateReading(4.5) == true);
    assert(dev.getLastTemperature() == 4.5);

    // Loại bỏ giá trị vô lý
    assert(dev.isReadingValid(-50.1) == false);
    assert(dev.isReadingValid(60.1) == false);
    assert(dev.isReadingValid(-999.0) == false);
    assert(dev.updateReading(100.0) == false);
    // Nhiệt độ cũ không bị thay đổi bởi giá trị vô lý
    assert(dev.getLastTemperature() == 4.5);

    std::cout << "PASSED\n";
}

void testSensorGatewayAcl() {
    std::cout << "[RUN] Test 3: SensorGateway Anti-Corruption Layer (Parsing & Filtering)... ";
    auto gateway = std::make_shared<SensorGateway>();
    auto mock = std::make_shared<TestMockHandler>();
    gateway->registerHandler(mock);

    // 1. Chuỗi thô hợp lệ "SENSOR-01,5.2"
    bool ok1 = gateway->processRawString("SENSOR-01,5.2");
    assert(ok1 == true);
    assert(mock->callCount == 1);
    assert(mock->receivedReadings[0].getSensorId() == "SENSOR-01");
    assert(mock->receivedReadings[0].getCelsius() == 5.2);

    // 2. Chuỗi thô có timestamp kèm theo: "SENSOR-02,-18.5,2026-10-13T16:30:00"
    bool ok2 = gateway->processRawString("SENSOR-02,-18.5,2026-10-13T16:30:00");
    assert(ok2 == true);
    assert(mock->callCount == 2);
    assert(mock->receivedReadings[1].getSensorId() == "SENSOR-02");
    assert(mock->receivedReadings[1].getCelsius() == -18.5);
    assert(mock->receivedReadings[1].getTimestamp() == "2026-10-13T16:30:00");

    // 3. Chuỗi sai định dạng
    bool okBad1 = gateway->processRawString("INVALID_STRING_WITHOUT_COMMA");
    assert(okBad1 == false);

    bool okBad2 = gateway->processRawString("SENSOR-03,NOT_A_NUMBER");
    assert(okBad2 == false);

    // 4. Giá trị vô lý (ngoài dải [-50, 60])
    bool okBad3 = gateway->processRawString("SENSOR-01,-75.0");
    assert(okBad3 == false);

    bool okBad4 = gateway->processRawString("SENSOR-01,88.5");
    assert(okBad4 == false);

    // Đảm bảo số lần gọi vào handler không bị tăng bởi các gói tin lỗi
    assert(mock->callCount == 2);

    std::cout << "PASSED\n";
}

void testMutexThreadSafety() {
    std::cout << "[RUN] Test 4: Mutex Thread Safety with Concurrent Ingestion... ";
    auto gateway = std::make_shared<SensorGateway>();
    auto mock = std::make_shared<TestMockHandler>();
    gateway->registerHandler(mock);

    const int NUM_THREADS = 4;
    const int READINGS_PER_THREAD = 50;
    std::vector<std::thread> threads;

    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back([gateway, t]() {
            for (int i = 0; i < READINGS_PER_THREAD; ++i) {
                std::string payload = "SENSOR-T" + std::to_string(t) + "," + std::to_string(3.0 + i * 0.1);
                gateway->processRawString(payload);
            }
        });
    }

    for (auto& th : threads) {
        if (th.joinable()) th.join();
    }

    // Đảm bảo tất cả readings đều vào an toàn không bị race condition
    assert(mock->callCount == NUM_THREADS * READINGS_PER_THREAD);
    std::cout << "PASSED (" << mock->callCount << " readings safely processed)\n";
}

void testOfflineTimeoutDetection() {
    std::cout << "[RUN] Test 5: Heartbeat Timeout Detection (Feature [N])... ";
    auto gateway = std::make_shared<SensorGateway>();
    auto mock = std::make_shared<TestMockHandler>();
    gateway->registerHandler(mock);

    // Gửi dữ liệu để sensor ONLINE
    gateway->processRawString("SENSOR-OFFLINE-TEST,4.0");
    assert(gateway->getDeviceStatus("SENSOR-OFFLINE-TEST") == DeviceStatus::ONLINE);

    // Chờ 2 giây và kiểm tra timeout với ngưỡng 1 giây
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    gateway->checkDeviceTimeouts(1); // Ngưỡng 1 giây

    assert(gateway->getDeviceStatus("SENSOR-OFFLINE-TEST") == DeviceStatus::OFFLINE);
    std::cout << "PASSED (Transitioned to OFFLINE)\n";
}

void testSensorSimulatorLifecycle() {
    std::cout << "[RUN] Test 6: SensorSimulator Lifecycle & 4 Modes... ";
    auto gateway = std::make_shared<SensorGateway>();
    auto mock = std::make_shared<TestMockHandler>();
    gateway->registerHandler(mock);

    SensorSimulator sim(gateway, 100); // 100ms interval for fast test
    sim.addSensor("SIM-01");

    // Test STABLE
    sim.setMode(SimulationMode::STABLE);
    sim.start();
    assert(sim.running() == true);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    // Test DRIFT
    sim.setMode(SimulationMode::DRIFT);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    // Test SPIKE
    sim.setMode(SimulationMode::SPIKE);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    // Test NOISE
    sim.setMode(SimulationMode::NOISE);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    sim.stop();
    assert(sim.running() == false);
    assert(mock->callCount > 0);

    std::cout << "PASSED (" << mock->callCount << " readings collected across 4 modes)\n";
}

int main() {
    std::cout << "===============================================\n";
    std::cout << "   TEST SUITE: TV2 SENSING SUBSYSTEM (DDD)     \n";
    std::cout << "===============================================\n";

    testTemperatureReadingValueObject();
    testSensorDeviceValidation();
    testSensorGatewayAcl();
    testMutexThreadSafety();
    testOfflineTimeoutDetection();
    testSensorSimulatorLifecycle();

    std::cout << "===============================================\n";
    std::cout << "   ALL 6 TESTS PASSED SUCCESSFULLY! (100%)    \n";
    std::cout << "===============================================\n";
    return 0;
}
