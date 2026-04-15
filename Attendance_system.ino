#include "BLEDevice.h"
#include <WiFi.h>

char ssid[] = "Your wifi ssid";
char pass[] = "Your wifi pw";

const char* studentUUIDs[] = {
    "12345678-1234-1234-1234-000000000001",
    "12345678-1234-1234-1234-000000000002",
    "12345678-1234-1234-1234-000000000003",
};
const char* studentNames[] = {"Student-01", "Student-02", "Student-03"};
const int studentCount = 3;
bool attendance[3] = {false, false, false};

WiFiServer server(80);
BLEAdvertData foundDevice;

void scanFunction(T_LE_CB_DATA* p_data) {
    foundDevice.parseScanInfo(p_data);

    uint8_t serviceCount = foundDevice.getServiceCount();
    if (serviceCount > 0) {
        BLEUUID* serviceList = foundDevice.getServiceList();
        for (uint8_t i = 0; i < serviceCount; i++) {
            String uuid = String(serviceList[i].str());
            Serial.println(uuid);
            for (int s = 0; s < studentCount; s++) {
                if (uuid.equalsIgnoreCase(studentUUIDs[s])) {
                    if (!attendance[s]) {
                        attendance[s] = true;
                        Serial.print(studentNames[s]);
                        Serial.println(" 출석!");
                    }
                }
            }
        }
    }
}

void setup() {
    Serial.begin(115200);

    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
        Serial.println("WiFi 연결 중...");
        delay(5000);
    }
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    server.setBlockingMode();
    server.begin();

    BLE.init();
    BLE.configScan()->setScanMode(GAP_SCAN_MODE_ACTIVE);
    BLE.configScan()->setScanInterval(500);
    BLE.configScan()->setScanWindow(250);
    BLE.configScan()->updateScanParams();
    BLE.setScanCallback(scanFunction);
    BLE.beginCentral(0);
    BLE.configScan()->startScan(0);

    Serial.println("출석 시스템 시작!");
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        String currentLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();
                        client.print(R"html(
<!DOCTYPE html>
<html>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1'>
<meta http-equiv='refresh' content='3'>
<title>출석 시스템</title>
<style>
  body{font-family:sans-serif;background:#111;color:white;padding:20px;}
  h2{color:#4CAF50;}
  .card{background:#222;border-radius:12px;padding:16px;margin:10px 0;display:flex;justify-content:space-between;}
  .present{color:#4CAF50;font-weight:bold;}
  .absent{color:#f44336;}
</style>
</head>
<body>
<h2>📋 출석 현황</h2>
)html");
                        for (int s = 0; s < studentCount; s++) {
                            client.print("<div class='card'><span>");
                            client.print(studentNames[s]);
                            client.print("</span><span class='");
                            client.print(attendance[s] ? "present'>✅ 출석" : "absent'>❌ 미출석");
                            client.print("</span></div>");
                        }
                        client.print("</body></html>");
                        client.println();
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }
            }
        }
        client.stop();
    }
}
