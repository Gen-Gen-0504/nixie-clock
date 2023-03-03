#include "NTP.h"
#include "RTC.h"
#include <WiFi.h>

const char* SSID = "YOUR_SSID";
const char* PASS = "YOUR_PASSWORD";
NTP ntp(SSID, PASS); // NTPクラスのインスタンスを生成

const uint8_t  PIN_SCL   = 32;
const uint8_t  PIN_SDA   = 33;
const uint8_t  PIN_N_INT = 27;
const RTCPinInfo rtcPinInfo {PIN_SCL, PIN_SDA, PIN_N_INT};
RTC rtc(rtcPinInfo); // RTCクラスのインスタンスを生成

bool tick_flag = false;                  // 時間更新割り込み発生フラグ
void setupTickFlag() {tick_flag = true;} // 時間更新割り込み時に実行する関数
struct tm tm;                            // 時刻保存用の構造体


void setup() {
  Serial.begin(115200);
  ntp.setup();
  rtc.setup();
  rtc.setISR(setupTickFlag);
  if(ntp.getIsConfigured()) { // Wi-Fi接続成功時
    ntp.getTime(&tm);
    rtc.setDateTime(&tm); // RTCに時刻を保存
    Serial.println(&tm, "%A, %B %d %Y %H:%M:%S");
    ntp.disconnect();
  }
  else { // Wi-Fi接続失敗時
    Serial.println("Could not connect to network.");
  }
}

void loop() {
  if(tick_flag) {
    rtc.getDateTime(&tm);
    Serial.println(&tm, "%A, %B %d %Y %H:%M:%S");
    tick_flag = false;
  }
}
