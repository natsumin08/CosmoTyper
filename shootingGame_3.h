#pragma once

// 構造体の宣言
struct OBJECT // 自機や敵機用
{
    int x;     // x座標
    int y;     // y座標
    int vx;    // x軸方向の速さ
    int vy;    // y軸方向の速さ
    int state;   // 存在するか
    int pattern; // 敵機の動きのパターン
    int image;   // 画像
    int wid;     // 画像の幅（ピクセル数）
    int hei;     // 画像の高さ
    int shield;  // シールド（耐久力）
    int timer;   // タイマー
};

// 関数プロトタイプ宣言
// ここにプロトタイプ宣言を記述する
void initGame(void);
void scrollBG(int spd);