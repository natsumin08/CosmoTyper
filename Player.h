#pragma once

#include "GameObject.h"

// --- Player クラス定義 ---
class Player : public GameObject {
private:
	int shield;
	int wid, hei;	// 画像サイズ
	int noDamageTimer;
	int weaponLv;
	int speed;

	int bombCount;	// ボム残弾数
	int bombTimer;	// ボム演出の継続時間

public:
	Player(double x, double y);
	~Player();

	void Update() override;
	void Draw() override;

	void Damage();
	void UseBomb();	// ボム発動を処理する

	// ゲッター
	int GetShield() const { return shield; }
	int GetWeaponLv() const { return weaponLv; }
	int GetSpeed() const { return speed; }
	int GetWid() const { return wid; }
	int GetHei() const { return hei; }
	bool IsNoDamage() const { return noDamageTimer > 0; }
	int GetBombCount() const { return bombCount; }
	int GetBombTimer() const { return bombTimer; }	// ボムタイマーの値を取得

	// アイテム取得用
	void IncreaseSpeed(int amount);
	void RecoverShield(int amount);
	void IncreaseWeaponLv(int amount);
};
