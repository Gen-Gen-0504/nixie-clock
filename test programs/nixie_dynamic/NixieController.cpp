#include "NixieController.h"

/* コンストラクタ */
NixieController::NixieController(const dgt_t_umap<NixiePinInfo> _dgt_pinf_umap) :
  dgt_nix_umap{
    {Digit::HR_L,  Nixie(_dgt_pinf_umap.at(Digit::HR_L))},
    {Digit::HR_R,  Nixie(_dgt_pinf_umap.at(Digit::HR_R))},
    {Digit::MIN_L, Nixie(_dgt_pinf_umap.at(Digit::MIN_L))},
    {Digit::MIN_R, Nixie(_dgt_pinf_umap.at(Digit::MIN_R))},
  }
{}

/* 初期設定 */
void NixieController::setup() {
  dgt_nix_iter = dgt_nix_umap.begin(); // 最初の桁
  dgt_nix_iter->second.lightOn();      // 点灯
  switched_time = micros();            // 点灯時刻を記録
  on_flag = true;                      // フラグを立てる
}

/* すべての管に数字をセットする */
void NixieController::setNums(const dgt_t_umap<uint8_t> dgt_num_umap) {
  for (auto [dgt, num] : dgt_num_umap) {
    dgt_nix_umap.at(dgt).setNum(num);
  }
}

/* すべての管を点灯させる */
void NixieController::lightup() {
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
