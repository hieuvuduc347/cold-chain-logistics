#include "../include/TemperatureSensor.h"
#include <ctime>
TemperatureSensor::TemperatureSensor(std::string Sensor_ID, std::string Product_name)
    : Sensor(Sensor_ID, Product_name),
    gen(time(0)), dist(2.0, 12.0)
    {}

// Dinh nghia ham doc du lieu
SensorData TemperatureSensor::readData() {
    SensorData data;
    data.Sensor_ID = this -> ID;
    data.Product_name = this -> Product_name;
    data.Sensor_type = "Temperature";
    data.value = dist(gen);
    data.timestamp = getCurrentTime();

    return data;
}