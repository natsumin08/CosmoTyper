#include "ClearScene.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "Utilities.h"
#include "Constants.h"

ClearScene::ClearScene()
{
	m_timer = 0;
}

ClearScene::~ClearScene() {}

void ClearScene::Update()
{
	// 自機操作
	if (GM.player) GM.player->Update();

	// 演出
	if (m_timer < FPS * 3)
	{
		if (m_timer % 7 == 0)
			setEffect(WIDTH / 2, 120, EFF_EXPLODE);
	}
	else if (m_timer == FPS * 3)
		PlaySoundMem(RM.GetSound("sound/stageclear.mp3"), DX_PLAYTYPE_BACK);

	// 次のステージへ
	if (m_timer > FPS * 8)
	{
		GM.stage++;
		GM.ChangeScene(SCENE_PLAY);
	}
	
	m_timer++;
	drawEffect();
}

void ClearScene::Draw()
{
	// クリア画面は自機を表示し続ける
	if (GM.player) GM.player->Draw();
	drawEffect();

	if (m_timer > FPS * 3)
		drawTextC(WIDTH * 0.5, HEIGHT * 0.3, "STAGE CLEAR!", 0x00ffff, 80);
}