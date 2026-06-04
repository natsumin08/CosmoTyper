#include "Enemy.h"
#include "GameManager.h"
#include "GameFuncs.h"
#include "Utilities.h"
#include "ResourceManager.h"

// --- 単語辞書を定義 ---
const std::vector<std::string> WORDS_LIST =
{
	"SHOOT",
	"GAME",
	"CODE",
	"TYPE",
	"WARP",
	"ENEMY",
	"BOSS",
	"FIRE",
	"SPEED"
};

// --- Enemy クラスの実装 ---
Enemy::Enemy(int type, double x, double y, double vx, double vy, int hp)
	: GameObject(x, y), type(type), hp(hp)
{
	this->vx = vx;
	this->vy = vy;
	this->timer = 0;
	this->wantsToShoot = false;

	// ファイル名を動的に作る
	std::string path = "image/enemy";
	path += std::to_string(type);
	path += ".png";

	image = RM.GetGraph(path);

	// タイピング用データの初期化と割り当て
	m_wordIndex = 0;
	m_isTarget = false;

	// WORD_LISTからランダムに単語を選択して割り当て
	int index = rand() % WORDS_LIST.size();
	m_word = WORDS_LIST[index];

	GetGraphSize(image, &wid, &hei);
	radius = (wid + hei) / 4.0;
}

Enemy::~Enemy() {}

void Enemy::Update()
{
	// AIロジックを呼び出す
	AILogic();

	// 移動
	x += vx;
	y += vy;
	timer++;

	// 画面外に出たら死亡フラグ
	if (x < -200 || x > WIDTH + 200 || y < -200 || y > HEIGHT + 200)
		isDead = true;
}

void Enemy::Draw()
{
	// 敵機本体の描画
	drawImage(image, (int)x, (int)y);

	// 単語の描画
	if (!m_word.empty())
	{
		int fontHandle = RM.GetFont("メイリオ", 22, 5, DX_FONTTYPE_EDGE);
		int startX = (int)x - GetDrawStringWidthToHandle(m_word.c_str(), m_word.length(), fontHandle) / 2;
		int startY = (int)y - 40;	// 敵スプライトの上部に表示

		// 敵の単語描画はシェイクの影響を受けない
		int shakeX_backup = GM.shakeX;
		int shakeY_backup = GM.shakeY;
		GM.shakeX = 0;
		GM.shakeY = 0;

		for (int i = 0; i < m_word.length(); i++)
		{
			char charBuffer[2] = { m_word[i], '\0' };
			unsigned int color;

			if (i < m_wordIndex)	// 入力済み：緑
				color = GetColor(0, 255, 0);
			else if (i == m_wordIndex)	// 次に打つべき文字：赤
				color = GetColor(255, 50, 50);
			else	// 未入力：白
				color = GetColor(255, 255, 255);

			// 文字を描画
			DrawStringToHandle(startX, startY, charBuffer, color, fontHandle);

			// 次の文字の開始X座標を更新
			startX += GetDrawStringWidthToHandle(charBuffer, 1, fontHandle);
		}

		// シェイクの値を元に戻す
		GM.shakeX = shakeX_backup;
		GM.shakeY = shakeY_backup;
	}
}

void Enemy::Damage(int dmg)
{
	if (type == ENE_BULLET) return;	// 敵の弾はダメージを受けない

	hp -= dmg;
	GM.score += 100;	// スコア加算
	if (GM.score > GM.hisco) GM.hisco = GM.score;

	// 死亡時の処理
	if (hp <= 0)
	{
		isDead = true;
		setEffect((int)x, (int)y, EFF_EXPLODE);	// 爆発エフェクト

		GM.enemyKillCount++;	// 爆破数をカウント
	}
}

bool Enemy::CheckWantsToShoot()
{
	if (wantsToShoot)
	{
		wantsToShoot = false;	// フラグリセット
		return true;
	}
	return false;
}

bool Enemy::TypeChar(char c)
{
	if (m_wordIndex < m_word.length())
	{
		// 入力文字を大文字に変換して比較
		char targetChar = toupper(m_word[m_wordIndex]);
		char inputChar = toupper(c);

		if (targetChar == inputChar)
		{
			// 正しい文字が打たれた
			m_wordIndex++;

			// 単語を全て打ち終わったかチェック
			if (m_wordIndex == m_word.length())
			{
				// 単語完成、敵を破壊
				isDead = true;
			}
			return true;
		}
	}
	return false;
}

void Enemy::AILogic()
{
	switch (type)
	{
	case ENE_ZAKO1:
		// （vx = 0, vy = 3）直進するだけ
		break;

	case ENE_ZAKO2:
		// Playerに向かうvxは生成時にMain.cppで設定
		break;

	case ENE_ZAKO3:
		if (vy > 1)	// 減速
			vy *= 0.9;
		else if (vy > 0)	// 停止＆射撃
		{
			vy = 0;
			wantsToShoot = true;	// 弾を撃ちたい
		}
		else if (timer > FPS * 2)	// 2秒後に飛び去る
		{
			vx = 8;
			vy = -4;
		}
		break;

	case ENE_BOSS:
		if (y > HEIGHT - 120 && vy > 0) vy = -2;
		if (y < 120 && vy < 0)	// 画面上端
		{
			vy = 2;
			wantsToShoot = true;	// 弾を撃ちたい
		}
		break;
	}
}
