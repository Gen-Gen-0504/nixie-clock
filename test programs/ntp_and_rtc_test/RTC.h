#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <time.h>

/* ピン情報をまとめる構造体 */
struct RTCPinInfo {
  uint8_t scl;
  uint8_t sda;
  uint8_t n_int;
};

class RTC {
  private:
    const RTCPinInfo PIN;
    constexpr static uint8_t RTC_ADRS      = 0x32; // RX8900のアドレス
    constexpr static uint8_t DATE_TIME_REG = 0x00; // 時間が格納されている領域の先頭アドレス
    constexpr static uint8_t EXTENSION_REG = 0x0D; //-------------------------------// 
    constexpr static uint8_t FLAG_REG      = 0x0E; // 各種設定用の3つのレジスタのアドレス //
    constexpr static uint8_t CONTROL_REG   = 0x0F; //-------------------------------//

  public:
    RTC(RTCPinInfo rtcPinInfo);
    void setup();
    void setISR(void (*isr)()) const;      // 時刻更新割り込み時に実行する関数の設定
    void setDateTime(struct tm *tm);       // 時間の保存
    void getDateTime(struct tm *tm) const; // 時間の取得

  private:
    void    setRegs(uint8_t addr, uint8_t sz, uint8_t *data) const; // レジスタに値を設定
    void    getRegs(uint8_t addr, uint8_t sz, uint8_t *data) const; // レジスタの値を取得
    uint8_t dec2bcd(uint8_t dec) const; // 10進数→BCD
    uint8_t bcd2dec(uint8_t bcd) const; // BCD→10進数
};
