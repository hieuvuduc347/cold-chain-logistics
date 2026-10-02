#include "../include/Sensor.h"
#include <string>

Sensor::Sensor(std::string Sensor_ID, std::string Product_name) {
    this -> ID = Sensor_ID;
    this -> Product_name = Product_name;
}