#include "NTP.h"

const char* SSID = "YOUR_SSID";
const char* PASS = "YOUR_PASSWORD";
NTP ntp(SSID, PASS); // NTPクラスのインスタンスを生成
struct tm tm;        // 時刻保存用の構造体


void setup() {
  Serial.begin(115200);
  ntp.setup();
  if(ntp.getIsConfigured()) { // Wi-Fi接続成功時
    ntp.getTime(&tm);
    Serial.println(&tm, "%A, %B %d %Y %H:%M:%S");
    ntp.disconnect();
  }
  else { // 失敗時
    Serial.println("Could not connect to network.");
  }
}

void loop() {
  /* none */
}
