#include "arduino_secrets.h"
#include "config.h"       
#include "wifiConfig.h"
#include "OTAUpdate.h"
#include "thingProperties.h"

void setup() {
  Serial.begin(115200);
  delay(1500);

  // Cấu hình chân LED điều khiển bởi Cloud
  pinMode(CLOUD_LED_PIN, OUTPUT);
  digitalWrite(CLOUD_LED_PIN, LOW);

  // 1. Bắt đầu cấu hình WiFi (sẽ tự động kết nối hoặc mở AP)
  wifiConfigSetup();

  // 2. KIỂM TRA TRẠNG THÁI WIFI SAU KHI CẤU HÌNH
  // Chỉ khởi động Cloud và OTA nếu đã kết nối WiFi thành công
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ WiFi da san sang, khoi dong Cloud & OTA...");

    // Khởi tạo thuộc tính cho Arduino IoT Cloud
    initProperties();

    // Kết nối tới Arduino IoT Cloud
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);

    setDebugMessageLevel(2);
    ArduinoCloud.printDebugInfo();

    // Kiểm tra cập nhật OTA lần đầu ngay khi có mạng
    initialOtaCheck();

  } else {
    Serial.println("⚠️ Dang o che do AP. Bo qua khoi dong Cloud & OTA.");
    Serial.println("Vui long ket noi vao mang ESP32-XXXX va truy cap http://192.168.4.1");
  }
}


// =========================================================================
 /*nếu không muốn cập nhật định kỳ thì comment dòng otaLoop() lại
 ReadMe.adoc do web sinh ra kệ nó không cần quan tâm 
 arfuino_secrets.h chỉ quan tâm dòng đầu tiên
 
 */
// =========================================================================
void loop() {
  // Tác vụ này luôn chạy (để xử lý webserver ở chế độ AP)
  wifiLoop();

  // Chỉ chạy các tác vụ liên quan đến Internet nếu WiFi đã kết nối
  if (WiFi.status() == WL_CONNECTED) {
    ArduinoCloud.update();
   // otaLoop();
  }
}

// Hàm callback này được tự động gọi khi biến 'led1' trên Cloud thay đổi
void onLed1Change() {
  digitalWrite(CLOUD_LED_PIN, led1);
  Serial.print("Trang thai LED Cloud da thay doi: ");
  Serial.println(led1 ? "BAT" : "TAT");
}