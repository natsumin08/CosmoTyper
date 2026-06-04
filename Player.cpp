#include "Player.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "GameFuncs.h"
#include "Utilities.h"

// --- Playerクラスの実装 ---
Player::Player(double x, double y) : GameObject(x, y)
{
	image = RM.GetGraph("image/fighter.png");
	shield = PLAYER_SHIELD_MAX;
	GetGraphSize(image, &wid, &hei);
	radius = (wid + hei) / 4.0;	// 簡易的な当たり判定半径

	noDamageTimer = 0;
	weaponLv = 1;
	speed = 5;
}

Player::~Player() {}

void Player::Update()
{
	if (bombTimer > 0)
	{
		bombTimer--;
		return;
	}

	// 移動処理
	if (CheckHitKey(KEY_INPUT_UP))
	{
		y -= speed;
		if (y < 30) y = 30;
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		y += speed;
		if (y > HEIGHT - 30) y = HEIGHT - 30;
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		x -= speed;
		if (x < 30) x = 30;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		x += speed;
		if (x > WIDTH - 30) x = WIDTH - 30;
	}

	// 無敵時間
	if (noDamageTimer > 0) noDamageTimer--;
}

void Player::Draw()
{
	// 無敵の点滅
	if (noDamageTimer % 4 < 2)
		drawImage(image, (int)x, (int)y);
}

void Player::Damage()
{
	if (noDamageTimer > 0) return;	// 無敵中はダメージを受けない

	if (shield > 0) shield--;

	// 画面シェイク
	StartShake(15);

	noDamageTimer = FPS;	// 1秒間無敵

	if (shield <= 0)
		isDead = true;	// 死亡フラグを立てる
}

// --- アイテム取得用 ---
void Player::IncreaseSpeed(int amount)
{
	if (speed < PLAYER_SPEED_MAX)
		speed += amount;
}
void Player::RecoverShield(int amount)
{
	if (shield < PLAYER_SHIELD_MAX)
		shield += amount;
}
void Player::IncreaseWeaponLv(int amount)
{
	if (weaponLv < WEAPON_LV_MAX)
		weaponLv++;
}

void Player::UseBomb()
{
	if (bombCount <= 0 || bombTimer > 0) return;

	bombCount--;			// 残団を減らす
	bombTimer = FPS * 3;	// 3秒間のボム演出開始
	noDamageTimer = bombTimer;	// ボム演出中は無敵

	// 敵弾を全て消去
	GM.enemy_bullets.clear();

	// 画面上の敵を全て倒す
	for (auto& enemy : GM.enemies)
		enemy->isDead = true;
}
