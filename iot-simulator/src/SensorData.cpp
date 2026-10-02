#include "../include/SensorData.h"
#include <sstream>
#include <chrono> // Thu vien de lay va xu li thoi gian
#include <iomanip>// Thu vien de dinh dang cach hien thi du lieu

std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t CurrentTime = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&CurrentTime), "%d-%m-%YT%H:%M:%S");

    return ss.str();
}

std::string SensorData::toJson() {
    std::stringstream ss;
    ss << "{\n"
       << "\"ID\"" << ": " << "\"" << Sensor_ID << "\", \n"
       << "\"Ten San Pham\"" << ": " << "\"" << Product_name << "\", \n"
       << "\"Loai San Pham\"" << ": " << "\"" << Sensor_type << "\", \n"
       << "\"Gia Tri\"" << ": " << std::fixed << std::setprecision(2) << value << ",\n" // lam tron sau dau , 2 chu so
       << "\"Thoi Gian\"" << ": " << "\"" << timestamp << "\"\n"
       << "}";

       return ss.str();
}