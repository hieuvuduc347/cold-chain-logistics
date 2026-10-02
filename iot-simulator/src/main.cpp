#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "../include/HumiditySensor.h"
#include "../include/TemperatureSensor.h"


int main() {
    // Nhap so luong thung hang
    std::cout << "\n---HE THONG QUAN LY CHUOI CUNG UNG LANH---\n";
    std::cout << "Nhap so luong thung hang can giam sat: ";
    int n;
    std::cin >> n;
    std::cin.ignore();

    // Tao mang dong chua du lieu
    std::vector<HumiditySensor*> HumSensors;
    std::vector<TemperatureSensor*> TemSensors;


    // Nhap du lieu cho tung thung hang
    for (int i=0; i<n; i++) {
        std::string input_id;
        std::string input_name;

        std::cout << "---Thung hang thu " << i+1 << "---\n";

        std::cout << "Ma thung hang: ";
        getline(std::cin, input_id);

        std::cout << "Ten san pham: ";
        getline(std::cin, input_name);

        // Them du lieu vao mang
        HumSensors.push_back(new HumiditySensor(input_id, input_name));
        TemSensors.push_back(new TemperatureSensor(input_id, input_name));
    }
 
    // Doc cam bien va ghi du lieuz
    std::cout << "\n===BAT DAU DOC CAM BIEN CHO" << n << " THUNG HANG===\n";
    while (true) {
        for (int i=0; i<n; i++) {
            SensorData temData = TemSensors[i] -> readData();
            SensorData humData = HumSensors[i] -> readData();

            // in du lieu
            std::cout << temData.toJson() << "\n";
            std::cout << humData.toJson() << "\n";
        }
        
        std::cout << "------------------------------------------";
        // Cu 10s doc cam bien 1 lan
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}