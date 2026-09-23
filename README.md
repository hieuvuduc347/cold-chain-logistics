# cold-chain-logistics
Thiết kế phần mềm quản lý và giám sát luồng vận chuyển hàng hóa nhạy cảm với nhiệt độ (vắc-xin, thực phẩm tươi sống) từ kho đến điểm tiêu thụ.

/cold-chain-logistics
├── /iot-gateway        // Lớp 1 & 2: Script C++ giả lập sinh dữ liệu JSON
├── /backend-core       // Lớp 3: Chứa code DDD, Observer Pattern, Database
├── /frontend-dashboard // Lớp 4: Code UI, WebSockets nhận cảnh báo
└── README.md           // Chứa mô tả dự án và cách chạy code cho cả nhóm
