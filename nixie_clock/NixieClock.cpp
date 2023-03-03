#include <random>
#include "NixieClock.h"

/* コンストラクタ */
NixieClock::NixieClock(const dgt_t_umap<NixiePinInfo> _dgt_pinf_umap) :
  dgt_nix_umap{
    {Digit::HR_L,  Nixie(_dgt_pinf_umap.at(Digit::HR_L))},
    {Digit::HR_R,  Nixie(_dgt_pinf_umap.at(Digit::HR_R))},
    {Digit::MIN_L, Nixie(_dgt_pinf_umap.at(Digit::MIN_L))},
    {Digit::MIN_R, Nixie(_dgt_pinf_umap.at(Digit::MIN_R))},
  }
{}

/* 初期設定 */
void NixieClock::setup() {
  dgt_nix_iter = dgt_nix_umap.begin(); // 最初の桁
  dgt_nix_iter->second.lightOn();      // 点灯
  switched_time = micros();            // 点灯時刻を記録
  on_flag = true;                      // フラグを立てる
}

/* すべての管に時刻をセット */
void NixieClock::setTime(const struct tm *tm) {
  dgt_nix_umap.at(Digit::HR_L ).setNum(tm->tm_hour/10);
  dgt_nix_umap.at(Digit::HR_R ).setNum(tm->tm_hour%10);
  dgt_nix_umap.at(Digit::MIN_L).setNum(tm->tm_min/10);
  dgt_nix_umap.at(Digit::MIN_R).setNum(tm->tm_min%10);
}

/* すべての管を点灯 */
void NixieClock::lightUp() {
  if (on_flag) { // オン時
    if (micros() - switched_time > ON_TIME_MICRO) { // 点灯時間終了
      dgt_nix_iter->second.lightOff(); // 消灯
      on_flag = false;                 // フラグを降ろす
      switched_time = micros();        // 切り替え時刻更新
    }
  }
  else { // オフ時
    if (micros() - switched_time > OFF_TIME_MICRO) { // 消灯時間終了
      if(++dgt_nix_iter == dgt_nix_umap.end()){
        dgt_nix_iter = dgt_nix_umap.begin();
      }
      dgt_nix_iter->second.lightOn(); // 点灯
      on_flag = true;                 // フラグを立てる
      switched_time = micros();       // 切り替え時刻更新
    }
  }
}

/* シャッフル点灯 */
void NixieClock::shuffle(uint8_t n = 1) {
  const uint8_t  NUM_OF_NUMBERS     = 10;
  const uint32_t SHUFFLE_SPAN_MILLI = 30;
  uint32_t previous;
  static dgt_t_umap<std::array<uint8_t, NUM_OF_NUMBERS>> dgt_ary_umap = { 
    {Digit::HR_L,  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
    {Digit::HR_R,  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
    {Digit::MIN_L, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
    {Digit::MIN_R, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}}
  }; // ランダム表示させる数列のunordered_map

  std::random_device rd; // 乱数生成エンジン
  std::mt19937 mt(rd()); // 疑似乱数生成エンジン
  for(uint8_t _n = 0; _n < n; ++_n) {
    for(auto& [dgt, ary] : dgt_ary_umap){
      std::shuffle(ary.begin(), ary.end(), mt); // 数列をシャッフル
    }
    for(uint8_t i = 0; i < NUM_OF_NUMBERS; ++i) {
      for(auto& [dgt, ary] : dgt_ary_umap){
        dgt_nix_umap.at(dgt).setNum(ary[i]);
      }
      previous = millis();
      while(millis() - previous < SHUFFLE_SPAN_MILLI) {
        lightUp();
      }
    }
  }
}
