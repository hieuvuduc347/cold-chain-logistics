#pragma once
#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include <string>
#include <random>
#include "SensorGateway.h"

enum class SimulationMode {
    STABLE,  // Kịch bản 1: Ổn định trong dải an toàn (2°C - 5°C)
    DRIFT,   // Kịch bản 2: Hỏng thùng, nhiệt độ tăng dần đều qua từng chu kỳ
    SPIKE,   // Kịch bản 3: Đột biến tức thời (vọt lên 11°C - 13°C) rồi hồi phục
    NOISE    // Kịch bản 4: Nhiễu ngẫu nhiên dao động biên độ lớn
};

class SensorSimulator {
private:
    std::shared_ptr<SensorGateway> gateway;
    std::vector<std::string> sensorIds;
    std::atomic<bool> isRunning{false};
    std::thread workerThread;
    int intervalMs;
    SimulationMode currentMode;

    std::mt19937 rng;
    std::uniform_real_distribution<double> stableDist;
    std::uniform_real_distribution<double> noiseDist;

    void runLoop();

public:
    explicit SensorSimulator(std::shared_ptr<SensorGateway> gateway, int intervalMs = 1500);
    ~SensorSimulator();

    void addSensor(const std::string& sensorId);
    void setMode(SimulationMode mode);
    void setInterval(int ms);
    SimulationMode getMode() const;
    std::string getModeString() const;

    void start();
    void stop();
    bool running() const;
};
