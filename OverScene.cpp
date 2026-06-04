#include "OverScene.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "Utilities.h"
#include "Constants.h"

OverScene::OverScene()
{
	m_timer = 0;
}

OverScene::~OverScene() {}

void OverScene::Update()
{
	if (m_timer < FPS * 3)
	{
		// 一定間隔で爆発エフェクト
		if (m_timer % 7 == 0)
			setEffect(WIDTH / 2, HEIGHT / 2, EFF_EXPLODE);
	}
	// 3秒たったらジングル再生
	else if (m_timer == FPS * 3)
		PlaySoundMem(RM.GetSound("sound/gameover.mp3"), DX_PLAYTYPE_BACK);

	// 10秒待つか、ボタンを押したらタイトルへ
	if (m_timer > FPS * 10 || (m_timer > FPS * 2 && CheckHitKey(KEY_INPUT_SPACE)))
		GM.ChangeScene(SCENE_TITLE);

	m_timer++;

	drawEffect();
}

void OverScene::Draw()
{
	drawEffect();

	if (m_timer > FPS * 3)
	{
		drawTextC(WIDTH * 0.5, HEIGHT * 0.3, "GAME OVER", 0xff0000, 80);
		drawTextC(WIDTH * 0.5, HEIGHT * 0.7, "Press SPACE to Title", 0xffffff, 30);
	}
}