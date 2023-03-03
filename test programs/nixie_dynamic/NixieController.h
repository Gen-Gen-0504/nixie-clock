#pragma once
#include <Arduino.h>
#include <unordered_map>
#include "Nixie.h"

/* 時計の桁を表す列挙型 */
enum class Digit : uint8_t {
  HR_L, HR_R, MIN_L, MIN_R
};

/* Digitの各要素と、任意の型のデータを紐づける辞書型のエイリアステンプレート */
template<typename T>
using dgt_t_umap = std::unordered_map<Digit, T>;

/* 4つのニキシー管を制御するクラス */
class NixieController {
  private:
    constexpr static uint32_t  ON_TIME_MICRO = 4000; // オン時間
    constexpr static uint32_t OFF_TIME_MICRO =  380; // オフ時間
    uint32_t switched_time; // 点灯消灯の切り替え時刻
    bool     on_flag;       // 点灯フラグ
    dgt_t_umap<Nixie>                 dgt_nix_umap; // 4桁分のニキシー管を扱うunordered_map
    dgt_t_umap<Nixie>::const_iterator dgt_nix_iter; // 現在扱っている桁

  public:
    NixieController(const dgt_t_umap<NixiePinInfo> _dgt_pinf_umap); // コンストラクタ
    void setup();                                                   // 初期設定
    void setNums(const dgt_t_umap<uint8_t> dgt_num_umap);           // すべての管に数字をセット
    void lightup();                                                 // すべての管を点灯
};
