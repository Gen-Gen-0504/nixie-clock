#include "NTP.h"
#include "RTC.h"
#include "NixieClock.h"

const char*   SSID = "YOUR_SSID";     // SSID
const char*   PASS = "YOUR_PASSWORD"; // パスワード
const uint8_t PIN_SCL         = 32;   // I2C通信用のピン
const uint8_t PIN_SDA         = 33;   //
const uint8_t PIN_N_INT       = 27;   // 割り込みを検知するピン
const uint8_t PIN_ANODE_HR_L  = 23;   // 4つのニキシー管のアノードのピン
const uint8_t PIN_ANODE_HR_R  = 22;   //
const uint8_t PIN_ANODE_MIM_L = 21;   //
const uint8_t PIN_ANODE_MIM_R = 19;   //
const uint8_t PIN_CATHODE_A   = 18;   // カソードの制御ピン
const uint8_t PIN_CATHODE_B   = 16;   //
const uint8_t PIN_CATHODE_C   = 4;    //
const uint8_t PIN_CATHODE_D   = 17;   //

NTP ntp(SSID, PASS); // NTPクラスのインスタンスを生成

const RTCPinInfo rtcPinInfo {
  PIN_SCL,
  PIN_SDA,
  PIN_N_INT
};
RTC rtc(rtcPinInfo); // RTCクラスのインスタンスを生成

const dgt_t_umap<NixiePinInfo> dgt_pinf_umap = {
  {Digit::HR_L,  {PIN_ANODE_HR_L,  PIN_CATHODE_A, PIN_CATHODE_B, PIN_CATHODE_C, PIN_CATHODE_D}}, // 時の左側
  {Digit::HR_R,  {PIN_ANODE_HR_R,  PIN_CATHODE_A, PIN_CATHODE_B, PIN_CATHODE_C, PIN_CATHODE_D}}, // 時の右側
  {Digit::MIN_L, {PIN_ANODE_MIM_L, PIN_CATHODE_A, PIN_CATHODE_B, PIN_CATHODE_C, PIN_CATHODE_D}}, // 分の左側
  {Digit::MIN_R, {PIN_ANODE_MIM_R, PIN_CATHODE_A, PIN_CATHODE_B, PIN_CATHODE_C, PIN_CATHODE_D}}, // 分の左側
};
NixieClock nixie_clock(dgt_pinf_umap); // NixieClockクラスのインスタンスを生成

bool tick_flag = false;                  // 時間更新割り込み発生フラグ
void setupTickFlag() {tick_flag = true;} // 時間更新割り込み時に実行する関数
struct tm tm;                            // 時刻保存用の構造体


void setup() {
  ntp.setup();
  rtc.setup();
  rtc.setISR(setupTickFlag);
  if(ntp.getIsConfigured()) { // Wi-Fi接続成功時
    ntp.getTime(&tm);
    rtc.setDateTime(&tm); // RTCに時刻を保存
    ntp.disconnect();
  }
  else { // Wi-Fi接続失敗時
    // 何もしない
  }
  
  nixie_clock.setup();
  nixie_clock.shuffle(3);
  rtc.getDateTime(&tm);
  nixie_clock.setTime(&tm);
}

void loop() {
  nixie_clock.lightUp();

  if(tick_flag) { // 割り込み発生（「分」が更新）
    rtc.getDateTime(&tm);
    if(tm.tm_min == 0) { // 「時」が変わったらシャッフル
      nixie_clock.shuffle(1);
    }
    nixie_clock.setTime(&tm);
    tick_flag = false; // フラグを降ろす
  }
}
