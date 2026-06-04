#include "PlayScene.h"
#include "GameManager.h"
#include "Utilities.h"
#include "Constants.h"
#include "ResourceManager.h"

#include <algorithm>
#include <cstdlib>


// コンストラクタ：ステージ開始時の初期化
PlayScene::PlayScene()
{
	// 距離やタイマーのリセット
	GM.distance = STAGE_DISTANCE;
	GM.timer = 0;

	// アイテムとエフェクトの初期化
	GM.item.state = 0;
	for (int i = 0; i < EFFECT_MAX; i++)
		GM.effect[i].state = 0;

	// プレイヤーの生成
	GM.player = std::make_unique<Player>(WIDTH / 2, HEIGHT / 2);

	// 敵や弾をクリア
	GM.enemies.clear();
	GM.player_bullets.clear();
	GM.enemy_bullets.clear();

	// BGM再生
	//PlaySoundMem(GM.bgm, DX_PLAYTYPE_LOOP);
}

PlayScene::~PlayScene()
{
	// シーン破棄時の処理（BGMを止めるなど）
}

void PlayScene::Update()
{
	// 入力処理（ボム・タイピング）
	UpdateInput();
	
	// オブジェクトの更新
	UpdateObjects();
	
	// ゲームルールの処理
	SpawnEnemies();			// 出現
	HandleEnemyShooting();	// 射撃
	CheckCollisions();		// 当たり判定
	CleanupObjects();		// 消滅

	// アイテムの処理（ドロップ計算、出現）
	UpdateItems();

	// ゲーム進行状況のチェック（死亡、クリア）
	CheckGameState();

	
}

void PlayScene::Draw()
{
	// 背景の描画
	int spd = (GM.distance == 0) ? 0 : 1;
	scrollBG(spd);

	// 各オブジェクトの描画
	if (GM.player) GM.player->Draw();
	for (auto& obj : GM.enemies) obj->Draw();
	for (auto& obj : GM.player_bullets) obj->Draw();
	for (auto& obj : GM.enemy_bullets) obj->Draw();

	// アイテムとエフェクト
	moveItem();
	drawEffect();

	// UI描画
	StageMap();
	DrawParameter();
	DrawGameUI();
}

void PlayScene::SpawnEnemies()
{
	// --- 敵のスポーン ---
	if (GM.distance == STAGE_DISTANCE)
	{
		srand(GM.stage);	// ステージのパターンを決める
		PlaySoundMem(RM.GetSound("sound/bgm.mp3"), DX_PLAYTYPE_LOOP);
	}
	if (GM.distance > 0) GM.distance--;

	// ザコ1とザコ2
	if (300 < GM.distance && GM.distance % 60 == 0)
	{
		int x = 100 + rand() % (WIDTH - 200);
		int y = -50;
		int e = 1 + rand() % 2;
		if (e == ENE_ZAKO1)
			GM.enemies.push_back(std::make_unique<Enemy>(ENE_ZAKO1, x, y, 0.0, 3.0, 1 * GM.stage));
		if (e == ENE_ZAKO2)
		{
			int vx = 0;
			if (GM.player && GM.player->GetX() < x - 50) vx = -3;
			if (GM.player && GM.player->GetX() > x + 50) vx = 3;
			GM.enemies.push_back(std::make_unique<Enemy>(ENE_ZAKO2, x, -100, vx, 5.0, 3 * GM.stage));
		}
	}
	// ザコ3
	if (300 < GM.distance && GM.distance < 900 && GM.distance % 120 == 0)
	{
		int x = 100 + rand() % (WIDTH - 200);
		double vy = (40 + rand() % 20) / 10.0;	// 減速させるためにdouble
		GM.enemies.push_back(std::make_unique<Enemy>(ENE_ZAKO3, x, -100, 0.0, vy, 5 * GM.stage));
	}
	// ボス
	if (GM.distance == 1)
		GM.enemies.push_back(std::make_unique<Enemy>(ENE_BOSS, WIDTH / 2, -120, 0.0, 1.0, 200 * GM.stage));
}

void PlayScene::HandleEnemyShooting()
{
	// 入力を処理
	char typedChar = getTypedChar();
	if(typedChar == 0) return;

	// 現在ロックオン中の敵を探す
	Enemy* currentTarget = nullptr;
	for (auto& enemy : GM.enemies)
	{
		if (enemy->IsTarget())
		{
			currentTarget = enemy.get();
			break;
		}
	}

	// 状況に応じて分岐
	if (currentTarget)	// 敵への攻撃判定
		TryAttackExistingTarget(typedChar, currentTarget);
	else	// 新しい敵を探す
		TryFindNewTarget(typedChar);
}

void PlayScene::CheckCollisions()
{
	// 自機がいないか無敵
	if (!GM.player || GM.player->IsNoDamage()) return;

	// 敵 vs 自機
	for (auto& enemy : GM.enemies)
	{
		if (enemy->isDead) continue;

		// 敵が画面下に到達した場合の処理
		if (enemy->GetY() > HEIGHT - 10)
		{
			GM.player->Damage();
			enemy->isDead = true;
			StartShake(30);
			continue;
		}

		if (CheckCollision(GM.player.get(), enemy.get()))
		{
			GM.player->Damage();
			enemy->Damage(1);
		}
	}

	// 敵の弾 vs 自機
	for (auto& bullet : GM.enemy_bullets)
	{
		if (bullet->isDead) continue;
		if (CheckCollision(GM.player.get(), bullet.get()))
		{
			GM.player->Damage();
			bullet->isDead = true;
		}
	}
}

void PlayScene::CleanupObjects()
{
	auto remover = [](const auto& obj) { return obj->isDead; };

	GM.enemies.erase(std::remove_if(GM.enemies.begin(), GM.enemies.end(), remover), GM.enemies.end());
	GM.player_bullets.erase(std::remove_if(GM.player_bullets.begin(), GM.player_bullets.end(), remover), GM.player_bullets.end());
	GM.enemy_bullets.erase(std::remove_if(GM.enemy_bullets.begin(), GM.enemy_bullets.end(), remover), GM.enemy_bullets.end());
}

void PlayScene::CheckGameState()
{
	std::string bgm_path = "sound/bgm.mp3";

	// プレイヤーが死んだら
	if (GM.player && GM.player->isDead)
	{
		StopSoundMem(RM.GetSound(bgm_path));
		GM.player.reset();	// プレイヤーを削除
		GM.ChangeScene(SCENE_OVER);
	}

	// ボスを倒したか？
	if (GM.distance == 0)
	{
		bool bossAlive = false;
		for (auto& obj : GM.enemies)
		{
			if (obj->GetType() == ENE_BOSS)
			{
				bossAlive = true;
				break;
			}
		}
		if (!bossAlive)
		{
			StopSoundMem(RM.GetSound(bgm_path));
			GM.ChangeScene(SCENE_CLEAR);
		}
	}
}

void PlayScene::HandleTypingInput()
{
	char typedChar = getTypedChar();
	std::string bullet_path = "image/bullet.png";

	if (typedChar != 0)
	{
		char buffer[2] = { typedChar, '\0' };
		//DrawString(WIDTH / 2, HEIGHT - 100, buffer, GetColor(0, 255, 255));
	}
	if (typedChar == 0) return;	// 有効なキー入力が無い場合は終了

	Enemy* currentTarget = nullptr;

	// 現在のターゲットを探す
	for (auto& enemy : GM.enemies)
	{
		if (enemy->IsTarget())
		{
			currentTarget = enemy.get();
			break;
		}
	}

	// ターゲットがいる場合：攻撃判定 or ミス判定
	if (currentTarget)
	{
		// 攻撃成功
		if (currentTarget->TypeChar(typedChar))
		{
			double vx, vy;
			calculateBulletVelocity(
				currentTarget->GetX(), currentTarget->GetY(),
				GM.player->GetX(), GM.player->GetY(),
				vx, vy
			);

			GM.player_bullets.push_back(std::make_unique<Bullet>(
				GM.player->GetX(), GM.player->GetY(), vx, vy, RM.GetGraph(bullet_path)
			));

			ClearInputCharBuf();
			return;
		}

		// ミスタッチ
		// 入力中の文字があり、それが間違っていた場合、リセット
		if (currentTarget->GetWordIndex() > 0)
		{
			// ターゲットをリセット（入力中の単語をリセット）
			currentTarget->ResetWordIndex();
			currentTarget->SetTarget(false);
			ClearInputCharBuf();
			return;
		}
	}

	// ターゲットがいない場合：新規ロックオン
	// 単語の最初の文字が一致する敵を探す
	for (auto& enemy : GM.enemies)
	{
		if (enemy->GetWordIndex() == 0)	// まだ攻撃されていない敵
		{
			char firstChar = toupper(enemy->GetWord()[0]);
			char inputChar = toupper(typedChar);

			if (firstChar == inputChar)
			{
				// ロックオン成功
				enemy->SetTarget(true);
				// 成功した文字を処理
				enemy->TypeChar(typedChar);

				// 攻撃が成立したので、弾も撃つ
				double vx, vy;
				calculateBulletVelocity(
					enemy->GetX(), enemy->GetY(),
					GM.player->GetX(), GM.player->GetY(),
					vx, vy
				);

				GM.player_bullets.push_back(std::make_unique<Bullet>(
					GM.player->GetX(), GM.player->GetY(), vx, vy, RM.GetGraph(bullet_path)
				));

				ClearInputCharBuf();
				return;
			}
		}
	}
	ClearInputCharBuf();
}

void PlayScene::DrawGameUI()
{
	// スコア、ハイスコア、ステージ数の表示
	drawText(10, 10, "SCORE %07d", GM.score, 0xffffff, 30);
	drawText(WIDTH - 220, 10, "HI-SC %07d", GM.hisco, 0xffffff, 30);
	drawText(WIDTH - 145, HEIGHT - 40, "STAGE %02d", GM.stage, 0xffffff, 30);
}

void PlayScene::StageMap(void)
{
	int mx = WIDTH - 30, my = 60;
	int wi = 20, he = HEIGHT - 120;
	int pos = 0;	// 距離が0で停止するため安全な値に
	if (STAGE_DISTANCE > 0)	// ゼロ除算を避ける
		pos = (HEIGHT - 140) * GM.distance / STAGE_DISTANCE;

	SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
	DrawBox(mx, my, mx + wi, my + he, 0xffffff, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(mx - 1, my - 1, mx + wi + 1, my + he + 1, 0xffffff, FALSE);
	DrawBox(mx, my + pos, mx + wi, my + pos + 20, 0x0080ff, TRUE);
}

void PlayScene::DrawParameter(void)
{
	if (!GM.player) return;	// プレイヤーがいない時は描画しない

	int x = 10, y = HEIGHT - 30;
	DrawBox(x, y, x + PLAYER_SHIELD_MAX * 30, y + 20, 0x000000, TRUE);

	int shield = GM.player->GetShield();
	for (int i = 0; i < shield; i++)
	{
		int r = 128 * (PLAYER_SHIELD_MAX - i) / PLAYER_SHIELD_MAX;
		int g = 255 * i / PLAYER_SHIELD_MAX;
		int b = 160 + 96 * i / PLAYER_SHIELD_MAX;
		DrawBox(x + 2 + i * 30, y + 2, x + 28 + i * 30, y + 18, GetColor(r, g, b), TRUE);
	}
	drawText(x, y - 25, "SHIELD Lv %02d", shield, 0xffffff, 20);
	drawText(x, y - 50, "WEAPON Lv %02d", GM.player->GetWeaponLv(), 0xffffff, 20);
	drawText(x, y - 75, "SPEED %02d", GM.player->GetSpeed(), 0xffffff, 20);
}

// 入力関連をまとめる
void PlayScene::UpdateInput()
{
	// ボム発動チェック
	if (GM.player && CheckHitKey(KEY_INPUT_B) == 1)
	{
		if (GM.player->GetBombCount() > 0 && GM.player->GetBombTimer() <= 0)
			GM.player->UseBomb();
	}

	// タイピング入力処理
	HandleTypingInput();

}

// オブジェクトの移動処理をまとめる
void PlayScene::UpdateObjects()
{
	// 自機の更新
	if (GM.player) GM.player->Update();

	// 敵・弾の更新
	for (auto& obj : GM.enemies) obj->Update();
	for (auto& obj : GM.player_bullets) obj->Update();
	for (auto& obj : GM.enemy_bullets) obj->Update();
}

// アイテム関連をまとめる
void PlayScene::UpdateItems()
{
	// --- 撃破数からのストック加算 ---
	if (GM.enemyKillCount >= ITEM_DROP_THRESHOLD)
	{
		// 閾値の何倍を倒したかを計算し、ストックに加える
		GM.itemStockCount += (GM.enemyKillCount / ITEM_DROP_THRESHOLD);

		// カウンターは残った分だけを残す
		GM.enemyKillCount %= ITEM_DROP_THRESHOLD;
	}

	// --- ストックの消費とアイテムの出現 ---
	if (GM.item.state == 0 && GM.itemStockCount > 0)
	{
		setItem();			// アイテムを出現
		GM.itemStockCount--;	// ストックを1つ消費
	}
}

// 共通処理：弾の生成と発射
void PlayScene::SpawnPlayerBullet(Enemy* target)
{
	double vx, vy;
	calculateBulletVelocity(
		target->GetX(), target->GetY(),
		GM.player->GetX(), GM.player->GetY(),
		vx, vy
	);

	GM.player_bullets.push_back(std::make_unique<Bullet>(
		GM.player->GetX(), GM.player->GetY(), vx, vy, RM.GetGraph("image/bullet.png")
	));
}

// 既存ターゲットへの攻撃処理
void PlayScene::TryAttackExistingTarget(char input, Enemy* target)
{
	// 正解
	if (target->TypeChar(input))
	{
		SpawnPlayerBullet(target);
		return;
	}

	// ミスタイプ
	if (target->GetWordIndex() > 0)
	{
		// ターゲット解除＆入力リセット
		target->ResetWordIndex();
		target->SetTarget(false);
	}
}

// 新規ターゲットの索敵処理
void PlayScene::TryFindNewTarget(char input)
{
	for (auto& enemy : GM.enemies)
	{
		// まだ一文字も撃たれていない敵を探す
		if (enemy->GetWordIndex() == 0)
		{
			// string[0]と入力文字を比較（大文字小文字無視：toupper）
			char firstChar = toupper(enemy->GetWord()[0]);
			char inputChar = toupper(input);

			if (firstChar == inputChar)
			{
				// ロックオン
				enemy->SetTarget(true);
				enemy->TypeChar(input);

				// 攻撃成功なので弾も撃つ
				SpawnPlayerBullet(enemy.get());
				return;	// 1回につき1体のみロックオン
			}
		}
	}
}