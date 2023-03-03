#include "RTC.h"

RTC::RTC(RTCPinInfo _PIN) : PIN(_PIN) {}

void RTC::setup() {
  uint8_t extreg = 0x20; // 分が更新されるごとに割り込み
  uint8_t flgreg = 0x00; // フラグのリセット
  uint8_t conreg = 0x20; // 時間更新割り込み開始
  uint8_t init_flgreg;   // フラグレジスタの初期状態
  struct  tm init_time = {0, 0, 0, 1, 0, 100, 6, 0, 0}; // 初期時刻 2000/01/01/00:00:00(SUT)
  
  delay(100);
  pinMode(PIN.n_int, INPUT_PULLUP);
  Wire.begin((int8_t)PIN.sda, (int8_t)PIN.scl);

  /* バッテリーの状態確認 */
  getRegs(FLAG_REG, 1, &init_flgreg); // フラグレジスタの値を取得
  if (init_flgreg & 0x02) {           // VLF(Voltage Low Flag)が1なら
    setDateTime(&init_time);          // 初期時刻を保存
  }
  else {/* 何もしない */}

  /* 各レジスタに値を設定 */
  setRegs(EXTENSION_REG, 1, &extreg);
  setRegs(FLAG_REG,      1, &flgreg);
  setRegs(CONTROL_REG,   1, &conreg);
}

void RTC::setISR(void (*isr)()) const {
  attachInterrupt(digitalPinToInterrupt(PIN.n_int), isr, FALLING);
}

void RTC::setDateTime(struct tm *tm) {
  uint8_t data[7];
  data[0] = dec2bcd(tm->tm_sec % 60);
  data[1] = dec2bcd(tm->tm_min);
  data[2] = dec2bcd(tm->tm_hour);
  data[3] = 1 << (tm->tm_wday);
  data[4] = dec2bcd(tm->tm_mday);
  data[5] = dec2bcd(tm->tm_mon + 1);
  data[6] = dec2bcd(tm->tm_year - 100);
  setRegs(DATE_TIME_REG, 7, data);
}

void RTC::getDateTime(struct tm *tm) const {
  uint8_t data[7];
  getRegs(DATE_TIME_REG, 7, data);
  tm->tm_sec  = bcd2dec(data[0] & 0x7f);
  tm->tm_min  = bcd2dec(data[1] & 0x7f);
  tm->tm_hour = bcd2dec(data[2] & 0x3f);
  tm->tm_wday = [&]{uint8_t wday = 0; while(data[3]>>=1){++wday;} return wday;}();
  tm->tm_mday = bcd2dec(data[4] & 0x3f);
  tm->tm_mon  = bcd2dec(data[5] & 0x1f) - 1;
  tm->tm_year = bcd2dec(data[6]) + 100;
}

void RTC::setRegs(uint8_t addr, uint8_t sz, uint8_t *data) const {
  Wire.beginTransmission(RTC_ADRS);
  Wire.write(addr);
  Wire.write(data, sz);
  Wire.endTransmission();
}

void RTC::getRegs(uint8_t addr, uint8_t sz, uint8_t *data) const {
  Wire.beginTransmission(RTC_ADRS);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADRS, sz);
  for (uint8_t i = 0; i < sz; i++) {
    data[i] = Wire.read();
  }
}

uint8_t RTC::dec2bcd(uint8_t dec) const {
  return (((dec / 10) << 4) | (dec % 10));
}

uint8_t RTC::bcd2dec(uint8_t bcd) const {
  return ((bcd >> 4) * 10 + (bcd & 0x0f));
}
