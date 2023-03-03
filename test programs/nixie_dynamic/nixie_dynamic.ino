#include "NixieController.h"

/* ニキシー管のピン情報 */
const uint8_t PIN_ANODE_HR_L  = 23; // それぞれの管のアノードのピン
const uint8_t PIN_ANODE_HR_R  = 22; //
const uint8_t PIN_ANODE_MIM_L = 21; //
const uint8_t PIN_ANODE_MIM_R = 19; //
const uint8_t PIN_CATHODE_A   = 18; // カソードの制御ピン
const uint8_t PIN_CATHODE_B   = 16; //
const uint8_t PIN_CATHODE_C   = 4;  //
const uint8_t PIN_CATHODE_D   = 17; //

/* すべての管のピン情報をまとめたmap */
const dgt_t_umap<NixiePinInfo> dgt_pinf_umap = {
  {Digit::HR_L,  {PIN_ANODE_HR_L,  PIN_CATHODE_A, PIN_CATHODE_B, PIN_CATHODE_C, PIN_CATHODE_D}}, // 時の左側
  {Digit::HR_R,  {PIN_ANODE_HR_R,  PIN_CATHODE_A, PIN_CATHODE_B, PIN_CATHODE_C, PIN_CATHODE_D}}, // 時の右側
  {Digit::MIN_L, {PIN_ANODE_MIM_L, PIN_CATHODE_A, PIN_CATHODE_B, PIN_CATHODE_C, PIN_CATHODE_D}}, // 分の左側
  {Digit::MIN_R, {PIN_ANODE_MIM_R, PIN_CATHODE_A, PIN_CATHODE_B, PIN_CATHODE_C, PIN_CATHODE_D}}, // 分の左側
};
NixieController nixie_controller(dgt_pinf_umap); // コントローラ生成

dgt_t_umap<uint8_t> dgt_num_umap = { // 表示する数字のunordered_map
  {Digit::HR_L,  0},
  {Digit::HR_R,  1},
  {Digit::MIN_L, 2},
  {Digit::MIN_R, 3}
};
uint32_t       prev_time;
const uint32_t DURATION = 1000;


void setup() {
  nixie_controller.setup();
  nixie_controller.setNums(dgt_num_umap); // 数字を設定
  prev_time = millis();
}

void loop() {
  nixie_controller.lightup(); // 光らせる
  
  if (millis() - prev_time > DURATION) { // 一定時間が経過していれば
    for(auto [dgt, num] : dgt_num_umap){
      dgt_num_umap.at(dgt) = (num + 1) % 10; // 数字を更新
    }
    nixie_controller.setNums(dgt_num_umap); // 数字を設定
    prev_time = millis();
  }
}
