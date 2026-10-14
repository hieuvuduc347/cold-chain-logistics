# ❄️ Cold Chain Logistics - Hệ thống Quản lý & Điều phối Chuỗi Cung ứng Lạnh Tích hợp IoT

Dự án xây dựng hệ thống giám sát, quản lý và điều phối chuỗi cung ứng lạnh (Cold Chain Logistics) theo thời gian thực. Hệ thống kết hợp thiết bị mô phỏng cảm biến IoT hiệu năng cao (C++) với nền tảng xử lý trung tâm và giao diện điều khiển trực quan nhằm đảm bảo chất lượng hàng hóa nhạy cảm với nhiệt độ (dược phẩm, vắc-xin, thực phẩm tươi sống) trong suốt quá trình lưu kho và vận chuyển.

---

## 🏗️ Kiến trúc Hệ thống (4-Layer Architecture)

Hệ thống được thiết kế theo mô hình kiến trúc 4 lớp và tổ chức dưới dạng **Monorepo**:

1. **Lớp Cảm biến & Thu thập dữ liệu (Sensing Layer):** Giả lập các node cảm biến nhiệt độ, độ ẩm sử dụng C++ hướng đối tượng (OOP), hỗ trợ đa luồng và áp dụng *Observer Pattern* để phát hiện bất thường tức thời.
2. **Lớp Truyền tải & Cổng kết nối (Gateway / Network Layer):** Thu thập dữ liệu từ các cảm biến, đóng gói định dạng chuẩn **JSON** và truyền tải lên máy chủ qua giao thức mạng.
3. **Lớp Xử lý & Lưu trữ (Backend & Database Layer):** Tiếp nhận luồng dữ liệu thời gian thực, kiểm tra ngưỡng an toàn theo chuẩn **Domain-Driven Design (DDD)**, lưu trữ lịch sử và kích hoạt cảnh báo.
4. **Lớp Giao diện Ứng dụng (Frontend / Dashboard Layer):** Hiển thị biểu đồ nhiệt độ/độ ẩm trực tiếp, theo dõi vị trí lô hàng và quản lý cảnh báo cho điều phối viên.

---

## 📂 Cấu trúc Thư mục (Project Structure)

```text
cold-chain-logistics/
├── iot-simulator/               # Phân hệ giả lập thiết bị IoT & Cảm biến (C++)
│   ├── include/                 # Khai báo các lớp và cấu trúc dữ liệu (Header files)
│   │   ├── Sensor.h             # Lớp cơ sở trừu tượng cho các loại cảm biến
│   │   ├── SensorData.h         # Cấu trúc dữ liệu đo đạc & đóng gói bản tin
│   │   ├── TemperatureSensor.h  # Lớp cảm biến nhiệt độ
│   │   └── HumiditySensor.h     # Lớp cảm biến độ ẩm
│   └── src/                     # Mã nguồn cài đặt chi tiết (Source files)
│       ├── main.cpp             # Luồng thực thi chính & giả lập Gateway
│       ├── Sensor.cpp
│       ├── SensorData.cpp
│       ├── TemperatureSensor.cpp
│       └── HumiditySensor.cpp
├── backend/                     # Máy chủ xử lý nghiệp vụ & RESTful API / Realtime
├── frontend/                    # Giao diện Dashboard giám sát & điều phối
├── database/                    # Các kịch bản khởi tạo CSDL (SQL schemas & migrations)
├── docs/                        # Tài liệu thiết kế hệ thống, sơ đồ UML & báo cáo
└── README.md                    # Tài liệu hướng dẫn tổng quan dự án
```

---

## ✨ Tính năng Nổi bật

* **Mô phỏng Cảm biến IoT bằng C++ (`iot-simulator`):**
  * Áp dụng lập trình hướng đối tượng (Kế thừa, Đa hình) và quản lý bộ nhớ tối ưu bằng con trỏ.
  * Sinh dữ liệu nhiệt độ và độ ẩm theo thời gian thực, hỗ trợ xử lý đa luồng (multithreading).
  * Tự động đóng gói dữ liệu thành chuỗi JSON chuẩn hóa để gửi qua Gateway.
* **Giám sát & Cảnh báo Thời gian thực:**
  * Phát hiện ngay lập tức khi nhiệt độ hoặc độ ẩm vượt ngưỡng cho phép của từng mặt hàng cụ thể.
  * Áp dụng *Observer Pattern* để thông báo tự động đến các bộ phận liên quan khi xảy ra sự cố.
* **Quản lý Chuỗi cung ứng & Truy xuất Nguồn gốc:**
  * Lưu trữ toàn bộ lịch sử biến thiên môi trường của từng lô hàng.
  * Hỗ trợ tra cứu, đánh giá chất lượng bảo quản từ kho xuất phát đến điểm giao nhận.

---

## 🛠️ Công nghệ Sử dụng (Tech Stack)

| Phân hệ | Công nghệ / Ngôn ngữ |
| :--- | :--- |
| **IoT Simulator & Gateway** | C++ (C++17), OOP, Multithreading, JSON Serialization |
| **Backend** | Python (FastAPI / Django) hoặc C# (.NET Core) |
| **Database** | PostgreSQL / SQL Server / Supabase |
| **Frontend** | HTML5, CSS3, JavaScript (React / Vue.js) |
| **Quản lý mã nguồn** | Git & GitHub |

---

## 🚀 Hướng dẫn Cài đặt & Khởi chạy

### 1. Clone dự án về máy
```bash
git clone [https://github.com/hieuvuduc347/cold-chain-logistics.git](https://github.com/hieuvuduc347/cold-chain-logistics.git)
cd cold-chain-logistics
```

### 2. Chạy phân hệ mô phỏng IoT (`iot-simulator`)
Yêu cầu máy tính đã cài đặt trình biên dịch C++ (`g++` qua MinGW/MSYS2 trên Windows hoặc GCC/Clang trên Linux/macOS):

```bash
cd iot-simulator
g++ -Iinclude src/*.cpp -o simulator -pthread
./simulator
```
*(Trên Windows PowerShell/CMD, chạy lệnh `.\simulator.exe` sau khi biên dịch).*

### 3. Khởi tạo Cơ sở dữ liệu & Backend
* Chạy các script khởi tạo bảng trong thư mục `database/`.
* Di chuyển vào thư mục `backend/`, cài đặt các thư viện phụ thuộc và khởi chạy server theo hướng dẫn chi tiết bên trong phân hệ.

### 4. Khởi chạy Giao diện (`frontend`)
```bash
cd frontend
npm install
npm run dev
```

---

## 🤝 Quy trình Làm việc Nhóm (Git Workflow)

1. Không đẩy (push) trực tiếp mã nguồn lên nhánh `main`.
2. Tạo nhánh mới cho từng tính năng hoặc phân hệ:
   * `feature/iot-sensing` (Phân hệ C++ IoT)
   * `feature/backend-api` (Phân hệ Server)
   * `feature/frontend-dashboard` (Phân hệ Giao diện)
3. Thực hiện `commit` rõ ràng và tạo **Pull Request (PR)** để các thành viên kiểm duyệt trước khi gộp (merge) vào nhánh chính.

---

## 👥 Tác giả & Thành viên Dự án

* **Vũ Đức Hiếu** (@hieuvuduc347) - Phát triển phân hệ IoT Simulator (C++) & Kiến trúc hệ thống.
* Cùng các thành viên trong nhóm phát triển dự án.
