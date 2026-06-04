#pragma once

#include "GameObject.h"

#include <string>

// --- Enemy クラス定義 ---
class Enemy : public GameObject {
private:
	int hp;
	int type;
	int timer;
	int wid, hei;

	bool wantsToShoot;	// 射撃フラグ

	// タイピングゲーム用データ
	std::string m_word;		// 敵に紐づけられた単語	
	int m_wordIndex;		// 次に打つべき文字のインデックス
	bool m_isTarget;		// 現在、プレイヤーが狙っている敵か

	void AILogic();	// AI処理

public:
	Enemy(int type, double x, double y, double vx, double vy, int hp);
	~Enemy();

	const std::string& GetWord() const { return m_word; }
	int GetWordIndex() const { return m_wordIndex; }
	void SetTarget(bool isTarget) { m_isTarget = isTarget; }	// ターゲット設定
	bool IsTarget() const { return m_isTarget; }

	// 外部から入力処理を行う
	bool TypeChar(char c);

	// 単語入力のリセット関数
	void ResetWordIndex() { m_wordIndex = 0; }

	void Update() override;
	void Draw() override;
	void Damage(int dmg) override;

	int GetType() const override { return type; }

	// Scene_Playが弾を生成するために使用
	bool CheckWantsToShoot();
};
