#include "../include/SensorSimulator.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>

SensorSimulator::SensorSimulator(std::shared_ptr<SensorGateway> gateway, int intervalMs)
    : gateway(std::move(gateway)),
      intervalMs(intervalMs),
      currentMode(SimulationMode::STABLE),
      rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())),
      stableDist(2.5, 4.8),
      noiseDist(0.5, 9.5) {}

SensorSimulator::~SensorSimulator() {
    stop();
}

void SensorSimulator::addSensor(const std::string& sensorId) {
    sensorIds.push_back(sensorId);
}

void SensorSimulator::setMode(SimulationMode mode) {
    this->currentMode = mode;
}

void SensorSimulator::setInterval(int ms) {
    this->intervalMs = ms;
}

SimulationMode SensorSimulator::getMode() const {
    return currentMode;
}

std::string SensorSimulator::getModeString() const {
    switch (currentMode) {
        case SimulationMode::STABLE: return "STABLE (On dinh 2.5C - 4.8C)";
        case SimulationMode::DRIFT:  return "DRIFT (That thoat nhiet tang dan)";
        case SimulationMode::SPIKE:  return "SPIKE (Dot bien tuc thoi)";
        case SimulationMode::NOISE:  return "NOISE (Nhieu ngau nhien)";
        default: return "UNKNOWN";
    }
}

bool SensorSimulator::running() const {
    return isRunning.load();
}

void SensorSimulator::start() {
    if (isRunning.load()) return;
    isRunning.store(true);
    workerThread = std::thread(&SensorSimulator::runLoop, this);
}

void SensorSimulator::stop() {
    if (!isRunning.load()) return;
    isRunning.store(false);
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void SensorSimulator::runLoop() {
    int tick = 0;

    // Ngẫu nhiên chọn 1 trong 2 trường hợp cho kịch bản DRIFT:
    // true: Tăng nhanh và VƯỢT ngưỡng an toàn (> 8.0°C)
    // false: Tăng chậm nhưng KHÔNG vượt ngưỡng an toàn (<= 8.0°C)
    std::uniform_int_distribution<int> coinFlip(0, 1);
    bool driftWillExceed = (coinFlip(rng) == 1);

    std::uniform_real_distribution<double> startExceed(3.6, 4.4);
    std::uniform_real_distribution<double> stepExceed(1.15, 1.45);

    std::uniform_real_distribution<double> startSafe(2.2, 3.2);
    std::uniform_real_distribution<double> stepSafe(0.55, 0.85);

    double currentDriftTemp = driftWillExceed ? startExceed(rng) : startSafe(rng);

    std::uniform_real_distribution<double> spikeDist(10.5, 13.2);

    while (isRunning.load()) {
        tick++;

        for (const auto& sensorId : sensorIds) {
            double temp = 0.0;

            switch (currentMode) {
                case SimulationMode::STABLE: {
                    temp = stableDist(rng);
                    break;
                }
                case SimulationMode::DRIFT: {
                    if (tick > 1) {
                        double step = driftWillExceed ? stepExceed(rng) : stepSafe(rng);
                        currentDriftTemp += step;
                    }
                    temp = currentDriftTemp;
                    break;
                }
                case SimulationMode::SPIKE: {
                    // Cứ mỗi 4 chu kỳ thì xuất hiện một đỉnh nhiệt đột biến
                    if (tick % 4 == 0) {
                        temp = spikeDist(rng); // Vượt ngưỡng nguy hiểm ngẫu nhiên
                    } else {
                        temp = stableDist(rng); // Trở về bình thường
                    }
                    break;
                }
                case SimulationMode::NOISE: {
                    temp = noiseDist(rng);
                    break;
                }
            }

            // Định dạng chuỗi thô theo đúng yêu cầu: "SENSOR-01,5.2"
            std::stringstream ss;
            ss << sensorId << "," << std::fixed << std::setprecision(2) << temp;
            std::string rawStr = ss.str();

            // Đẩy vào Gateway (ACL)
            if (gateway) {
                gateway->processRawString(rawStr);
            }
        }

        // Tạm dừng theo interval cấu hình được (chia nhỏ 50ms để phản hồi stop tức thì)
        int elapsed = 0;
        while (elapsed < intervalMs && isRunning.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            elapsed += 50;
        }
    }
}
