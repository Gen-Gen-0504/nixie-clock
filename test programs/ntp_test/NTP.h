#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

class NTP {
  private:
    const char*    SSID;
    const char*    PASS;
    const char*    NTP_SERVER_1    = "ntp.nict.jp";
    const char*    NTP_SERVER_2    = "time.google.com";
    const char*    NTP_SERVER_3    = "ntp.jst.mfeed.ad.jp";
    const uint32_t GMT_OFFSET      = 3600*9;
    const uint8_t  DATLGIHT_OFFSET = 0;
    const uint8_t  CONN_TRY_TIMES  = 10;
    const uint8_t  CONN_TRY_INTVL  = 500;
    bool           is_configured   = false;

  public:
    NTP(const char* _SSID, const char* _PASS) : SSID(_SSID), PASS(_PASS) {};

    void setup() {
      WiFi.mode(WIFI_STA);
      WiFi.begin(SSID, PASS);
      /* 接続試行 */
      for(uint8_t i = 0; i < CONN_TRY_TIMES; ++i) {
        if(WiFi.status() == WL_CONNECTED) {
          /* 時刻を設定 */
          configTime(GMT_OFFSET, DATLGIHT_OFFSET, NTP_SERVER_1, NTP_SERVER_2, NTP_SERVER_3);
          is_configured = true;
          return;
        }
        delay(CONN_TRY_INTVL);
      }
    }

    void getTime(struct tm *tm) {
      /* 秒数が更新された瞬間の時刻を格納 */
      getLocalTime(tm);
      uint8_t previous_sec = tm->tm_sec;
      do {
        getLocalTime(tm);
      }while(tm->tm_sec == previous_sec); // 秒が変わるまで待つ
    }

    void disconnect() const {
      WiFi.disconnect();
    }

    bool getIsConfigured() const {return is_configured;}
};
