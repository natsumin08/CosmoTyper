#include "TitleScene.h"
#include "DxLib.h"
#include "GameManager.h"
#include "Constants.h"
#include "Utilities.h"

TitleScene::TitleScene()
{
	// 初期化
}

TitleScene::~TitleScene() {}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		// ゲーム開始前にスコアとステージをリセット
		GM.score = 0;
		GM.stage = 1;
		GM.itemStockCount = 0;

		ClearInputCharBuf();

		GM.ChangeScene(SCENE_PLAY);
	}
}

void TitleScene::Draw()
{
	scrollBG(1);

	drawTextC(WIDTH * 0.5, HEIGHT * 0.3, "Cosmo Typer", 0xffffff, 80);
	drawTextC(WIDTH * 0.5, HEIGHT * 0.7, "Press SPACE to start.", 0xffffff, 30);
}