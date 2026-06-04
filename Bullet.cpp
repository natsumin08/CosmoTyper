#include "Bullet.h"
#include "GameManager.h"
#include "GameFuncs.h"
#include "ResourceManager.h"

// --- Bullet クラスの実装 ---
Bullet::Bullet(double x, double y, double vx, double vy, int imgHandle)
	: GameObject(x, y)
{
	this->vx = vx;
	this->vy = vy;

	image = imgHandle;

	int w, h;
	GetGraphSize(image, &w, &h);
	radius = (w + h) / 4.0;
}

void Bullet::Update()
{
	x += vx;
	y += vy;

	// 画面外に出たら死亡フラグ
	if (x < -100 || x > WIDTH + 100 || y < -100 || y > HEIGHT + 100)
		isDead = true;
}

void Bullet::Draw()
{
	//DrawGraph((int)x - wid / 2, (int)y - hei / 2, image, TRUE);
	// 以下の表記を使う場合、extern宣言が必要
	//drawImage(image, (int)x, (int)y);
}
