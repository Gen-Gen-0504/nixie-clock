#pragma once
#include <Arduino.h>

/* ピン情報をまとめる構造体 */
struct NixiePinInfo {
  uint8_t anode;
  uint8_t cathode_a;
  uint8_t cathode_b;
  uint8_t cathode_c;
  uint8_t cathode_d;
};

/* ニキシー管を表すクラス */
class Nixie {
  private:
    const NixiePinInfo PIN; // ピン情報
    uint8_t            num; // 表示する数字

  public:
    /* コンストラクタ */
    Nixie(NixiePinInfo _pin_info): PIN(_pin_info), num(0) {
      pinMode(PIN.anode,     OUTPUT);
      pinMode(PIN.cathode_a, OUTPUT);
      pinMode(PIN.cathode_b, OUTPUT);
      pinMode(PIN.cathode_c, OUTPUT);
      pinMode(PIN.cathode_d, OUTPUT);
      digitalWrite(PIN.anode,     HIGH);
      digitalWrite(PIN.cathode_a, HIGH);
      digitalWrite(PIN.cathode_b, HIGH);
      digitalWrite(PIN.cathode_c, HIGH);
      digitalWrite(PIN.cathode_d, HIGH);
    }

    /* 数字を設定 */
    void setNum(uint8_t _num) {
      num = _num;
    }

    /* 点灯 */
    void lightOn() const {
      digitalWrite(PIN.cathode_a, num & B0001);
      digitalWrite(PIN.cathode_b, num & B0010);
      digitalWrite(PIN.cathode_c, num & B0100);
      digitalWrite(PIN.cathode_d, num & B1000);
      digitalWrite(PIN.anode, LOW);
    }

    /* 消灯 */
    void lightOff() const {
      digitalWrite(PIN.anode, HIGH);
    }
};
