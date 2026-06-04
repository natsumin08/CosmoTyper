#pragma once

#include <vector>
#include <memory>

#include "DxLib.h"
#include "Constants.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "IScene.h"

class GameManager {
public:
	//	シングルトンアクセス用
	static GameManager& GetInstance()
	{
		static GameManager instance;
		return instance;
	}

	// コピー禁止の呪文
	GameManager(const GameManager&) = delete;
	void operator = (const GameManager&) = delete;

	// 現在のシーンを管理するポインタ
	std::unique_ptr<IScene> currentScene;

	// シーンを変更する関数
	void ChangeScene(int nextSceneID);

public:
	// --- ゲームステート変数 ---
	int distance;
	int stage;
	int score;
	int hisco;
	int scene;
	int timer;
	int enemyKillCount;
	int itemStockCount;

	// --- シェイク演出用 ---
	int shakeTimer;
	int shakeX, shakeY;
	
	// --- オブジェクト管理 ---
	std::unique_ptr<Player> player;
	std::vector<std::unique_ptr<Enemy>> enemies;
	std::vector<std::unique_ptr<Bullet>> player_bullets;
	std::vector<std::unique_ptr<Bullet>> enemy_bullets;

	// --- その他 ---
	struct OBJECT item;
	struct OBJECT effect[EFFECT_MAX];

private:
	// コンストラクタをprivateにして、勝手にnewできないようにする
	GameManager();
};

#define GM GameManager::GetInstance()