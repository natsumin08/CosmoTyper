#include "Constants.h"
#include "Utilities.h"
#include "GameManager.h"
#include "ResourceManager.h"

#include <cmath>
#include <algorithm>
#include <cstring>
#include <cctype>

#define GM GameManager::GetInstance()

void drawImage(int img, int x, int y)
{
	int w, h;
	GetGraphSize(img, &w, &h);
	DrawGraph(x - w / 2 + GM.shakeX, y - h / 2 + GM.shakeY, img, TRUE);
}

void drawText(int x, int y, const char* txt, int val, int col, int siz)
{
	SetFontSize(siz);
	DrawFormatString(x + 1, y + 1, 0x000000, txt, val);
	DrawFormatString(x, y, col, txt, val);
}

void drawTextC(int x, int y, const char* txt, int col, int siz)
{
	SetFontSize(siz);
	int strWidth = GetDrawStringWidth(txt, strlen(txt));
	x -= strWidth / 2;
	y -= siz / 2;
	DrawString(x + 1, y + 1, txt, 0x000000);
	DrawString(x, y, txt, col);
}

void StartShake(int duration)
{
	GM.shakeTimer = duration;
}


// 2点間のベクトルを正規化し、速度を乗算して弾の速度ベクトルを計算
void calculateBulletVelocity(double targetX, double targetY, double startX, double startY, double& out_vx, double& out_vy)
{
	double dx = targetX - startX;
	double dy = targetY - startY;
	double dist = std::sqrt(dx * dx + dy * dy);

	if (dist > 0)
	{
		// ベクトルを正規化し、乗算
		out_vx = (dx / dist) * BULLET_SPEED;
		out_vy = (dy / dist) * BULLET_SPEED;
	}
	else
	{
		out_vx = 0.0;
		out_vy = -BULLET_SPEED;	// ターゲットが自機と同じ座標の場合、上向き発射
	}
}

// --- 当たり判定ヘルパー関数 ---
bool CheckCollision(GameObject* a, GameObject* b)
{
	if (!a || !b) return false;
	double dx = a->GetX() - b->GetX();
	double dy = a->GetY() - b->GetY();
	double dist = a->GetRadius() + b->GetRadius();
	return (dx * dx + dy * dy) < (dist * dist);
}
// Player vs Itemの当たり判定
bool CheckCollision(GameObject* a, int x, int y, int w, int h)
{
	if (!a) return false;
	// 簡易的な矩形 vs 円
	double dx = a->GetX() - std::max(x - w / 2, std::min(a->GetX(), x + w / 2));
	double dy = a->GetY() - std::max(y - h / 2, std::min(a->GetY(), y + h / 2));
	return (dx * dx + dy * dy) < (a->GetRadius() * a->GetRadius());
}


// キーボードで打たれた有効な文字を一つ取得
char getTypedChar()
{
	// 入力された文字のアスキーコードを取得
	int input = GetInputChar(FALSE);
	if (input > 0 && input < 256)
	{
		if (28 <= input && input <= 31)
			ClearInputCharBuf();

		char c = (char)input;

		if (std::isalpha(c))
			return (char)std::toupper(c);
	}
	return 0;
}

void scrollBG(int spd)
{
	int hGalaxy = RM.GetGraph("image/bg0.png");
	int hFloor = RM.GetGraph("image/bg1.png");
	int hWallL = RM.GetGraph("image/bg2.png");
	int hWallR = RM.GetGraph("image/bg3.png");

	static int galaxyY, floorY, wallY;

	galaxyY = (galaxyY + spd) % HEIGHT;
	DrawGraph(0, galaxyY - HEIGHT, hGalaxy, FALSE);
	DrawGraph(0, galaxyY, hGalaxy, FALSE);

	floorY = (floorY + spd * 2) % 120;
	for (int i = -1; i < 6; i++)
		DrawGraph(240, floorY + i * 120, hFloor, TRUE);

	wallY = (wallY + spd * 4) % 240;
	DrawGraph(0, wallY - 240, hWallL, TRUE);
	DrawGraph(WIDTH - 300, wallY - 240, hWallR, TRUE);
}

void setEffect(int x, int y, int ptn)
{
	static int eff_num;
	GM.effect[eff_num].x = x;
	GM.effect[eff_num].y = y;
	GM.effect[eff_num].state = 1;
	GM.effect[eff_num].pattern = ptn;
	GM.effect[eff_num].timer = 0;

	eff_num = (eff_num + 1) % EFFECT_MAX;

	if (ptn == EFF_EXPLODE)
		PlaySoundMem(RM.GetSound("sound/explosion.mp3"), DX_PLAYTYPE_BACK);
}

void drawEffect(void)
{
	int ix;
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (GM.effect[i].state == 0) continue;
		switch (GM.effect[i].pattern)
		{
		case EFF_EXPLODE:
			ix = GM.effect[i].timer * 128;
			DrawRectGraph(GM.effect[i].x - 64, GM.effect[i].y - 64, ix, 0, 128, 128, RM.GetGraph("image/explosion.png"), TRUE, FALSE);
			GM.effect[i].timer++;
			if (GM.effect[i].timer == 7)
				GM.effect[i].state = 0;
			break;

		case EFF_RECOVER:
			if (!GM.player)
			{
				GM.effect[i].state = 0;
				continue;
			}
			if (GM.effect[i].timer < 30)
				SetDrawBlendMode(DX_BLENDMODE_ADD, GM.effect[i].timer * 8);
			else
				SetDrawBlendMode(DX_BLENDMODE_ADD, (60 - GM.effect[i].timer) * 8);

			int px = GM.player->GetX();
			int py = GM.player->GetY();
			int pw = GM.player->GetWid();
			int ph = GM.player->GetHei();
			for (int j = 3; j < 8; j++)
				DrawCircle(px, py, (pw + ph) / j, 0x2040c0, TRUE);

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			GM.effect[i].timer++;
			if (GM.effect[i].timer == 60)
				GM.effect[i].state = 0;
			break;
		}
	}
}

void setItem(void)
{
	memset(&GM.item, 0, sizeof(struct OBJECT));

	GM.item.x = (WIDTH / 4) * (1 + rand() % 3);
	GM.item.y = -16;
	GM.item.vx = 15;
	GM.item.vy = 1;
	GM.item.state = 1;
	GM.item.timer = 0;
	GM.item.pattern = rand() % ITEM_TYPE;
}

void moveItem(void)
{
	if (GM.item.state == 0) return;
	GM.item.x += GM.item.vx;
	GM.item.y += GM.item.vy;
	if (GM.item.timer % 60 < 30)
		GM.item.vx -= 1;
	else
		GM.item.vx += 1;
	if (GM.item.y > HEIGHT + 16)
		GM.item.state = 0;
	GM.item.timer++;
	DrawRectGraph(GM.item.x - 20, GM.item.y - 16, GM.item.pattern * 40, 0, 40, 32, RM.GetGraph("image/item.png"), TRUE, FALSE);

	if (GM.scene == SCENE_OVER || !GM.player) return;

	if (CheckCollision(GM.player.get(), GM.item.x, GM.item.y, 40, 32))
	{
		GM.item.state = 0;
		if (GM.item.pattern == ITEM_ENERGY)
			GM.player->IncreaseSpeed(3);
		if (GM.item.pattern == ITEM_SHIELD)
		{
			GM.player->RecoverShield(1);
			setEffect(GM.player->GetX(), GM.player->GetY(), EFF_RECOVER);
		}
		if (GM.item.pattern == ITEM_WEAPON)
			GM.player->IncreaseWeaponLv(1);
		PlaySoundMem(RM.GetGraph("image/item.png"), DX_PLAYTYPE_BACK);
	}
}
