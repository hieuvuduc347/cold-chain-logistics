#include "../include/HumiditySensor.h"
#include <ctime> 
// ham tao do am ngau nhie
HumiditySensor::HumiditySensor(std::string Sensor_ID, std::string Product_Name)
    : Sensor(ID, Product_Name),
    gen(time(0)), dist(50.0, 80.0)
    {}

SensorData HumiditySensor::readData() {
    SensorData data;
    data.Sensor_ID = this -> ID;
    data.Product_name = this -> Product_name;
    data.Sensor_type = "Humidity";
    data.value = dist(gen);
    data.timestamp = getCurrentTime();

    return data;
}