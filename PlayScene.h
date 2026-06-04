#pragma once

#include "IScene.h"

class Enemy;

class PlayScene : public IScene {
public:
	PlayScene();
	~PlayScene();

	void Update() override;
	void Draw() override;

private:
	// --- ゲームロジック用メンバ関数 ---
	void SpawnEnemies();
	void HandleEnemyShooting();
	void CheckCollisions();
	void CleanupObjects();
	void CheckGameState();
	void HandleTypingInput();

	// --- 描画用メンバ関数 ---
	void DrawGameUI();
	void StageMap();
	void DrawParameter();

	void UpdateInput();		// 入力関連まとめ
	void UpdateObjects();	// 移動処理まとめ
	void UpdateItems();		// アイテム処理まとめ

	// --- タイピング処理分解用
	void SpawnPlayerBullet(Enemy* target);						// 弾発射
	void TryAttackExistingTarget(char input, Enemy* target);	// 既存敵への攻撃
	void TryFindNewTarget(char input);							// 新規敵の索敵
};