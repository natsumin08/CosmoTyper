#pragma once

#define NOMINMAX

#include "DxLib.h"

// -- - 定義・enum-- -
const int WIDTH = 1200, HEIGHT = 720;	// ウィンドウの幅と高さのピクセル数
const int FPS = 60;						// フレームレート
const int IMG_ENEMY_MAX = 5;			// 敵の画像の枚数（種類）
const int STAGE_DISTANCE = FPS * 60;	// ステージの長さ
const int PLAYER_SHIELD_MAX = 8;		// 自機のシールドの最大値
const int EFFECT_MAX = 100;				// エフェクトの最大数
const int ITEM_TYPE = 3;				// アイテムの種類
const int WEAPON_LV_MAX = 10;			// 武器レベルの最大値
const int PLAYER_SPEED_MAX = 20;		// 自機の速さの最大値
const int ITEM_DROP_THRESHOLD = 5;		// アイテムドロップに必要な撃破数
const double BULLET_SPEED = 20.0;		// 弾の速さ

enum
{	// 敵機の種類
	ENE_BULLET,
	ENE_ZAKO1,
	ENE_ZAKO2,
	ENE_ZAKO3,
	ENE_BOSS
};

enum
{	// エフェクトの種類
	EFF_EXPLODE,
	EFF_RECOVER
};

enum
{	// シーンを分けるための列挙定数
	SCENE_TITLE,
	SCENE_PLAY,
	SCENE_OVER,
	SCENE_CLEAR
};

enum
{	// アイテムの種類
	ITEM_ENERGY,
	ITEM_SHIELD,
	ITEM_WEAPON
};

// --- 構造体 ---
struct OBJECT
{
	int x;			// ｘ座標
	int y;			// ｙ座標
	int vx;			// ｘ軸方向の速さ
	int vy;			// ｙ軸方向の速さ
	int state;		// 存在するか
	int pattern;	// 動きのパターン
	int image;		// 画像
	int wid;		// 画像の幅（ピクセル数）
	int hei;		// 画像の高さ
	int shield;		// シールド（耐久力）
	int timer;		// タイマー
};
