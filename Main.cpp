#include "GameManager.h"
#include "GameFuncs.h"
#include "Utilities.h"

// --- WinMain 関数 ---
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdline, int nCmdShow)
{
	SetWindowText("Cosmo Typer");
	SetGraphMode(WIDTH, HEIGHT, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) return -1;
	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);

	GM.distance = STAGE_DISTANCE;

	while (1)	// メインループ
	{
		if (ProcessMessage() == -1) break;

		ClearDrawScreen();	// 画面をクリアする

		// --- シェイク計算処理 ---
		if (GM.shakeTimer > 0)
		{
			GM.shakeTimer--;
			GM.shakeX = rand() % 21 - 10;
			GM.shakeY = rand() % 21 - 10;
		}
		else
		{
			GM.shakeX = 0;
			GM.shakeY = 0;
		}

		GM.timer++;		// 共通タイマー

	
		// --- シーンの更新と描画 ---
		if (GM.currentScene)
		{
			GM.currentScene->Update();
			GM.currentScene->Draw();
		}

		// --- 画面反映 ---
		ScreenFlip();
		WaitTimer(1000 / FPS);
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	GM.player.reset();
	GM.enemies.clear();
	GM.player_bullets.clear();
	GM.enemy_bullets.clear();
	DxLib_End();
	return 0;
}