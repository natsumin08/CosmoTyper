#include "GameManager.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "OverScene.h"
#include "ClearScene.h"

GameManager::GameManager()
{
	// --- 初期値の代入 ---
	distance = 0;
	stage = 1;
	score = 0;
	hisco = 10000;
	scene = SCENE_TITLE;
	timer = 0;
	enemyKillCount = 0;
	itemStockCount = 0;

	shakeTimer = 0;
	shakeX = 0;
	shakeY = 0;

	item.state = 0;
	for (int i = 0; i < EFFECT_MAX; i++)
		effect[i].state = 0;

	// 最初のシーンを生成
	currentScene = std::make_unique<TitleScene>();
}

// シーン切り替え関数
void GameManager::ChangeScene(int nextSceneID)
{
	scene = nextSceneID;

	switch (nextSceneID)
	{
	case SCENE_TITLE:
		currentScene = std::make_unique<TitleScene>();
		break;

	case SCENE_PLAY:
		currentScene = std::make_unique<PlayScene>();
		break;

	case SCENE_OVER:
		currentScene = std::make_unique<OverScene>();
		break;

	case SCENE_CLEAR:
		currentScene = std::make_unique<ClearScene>();
		break;
	}
}