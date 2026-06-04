#pragma once

#include "Constants.h"

// --- GameObject 基底クラス ---
class GameObject {
protected:
	double x, y, vx, vy;
	double radius;	// 当たり判定の半径
	int image;

public:
	bool isDead;	// 死亡フラグ

	GameObject(double x, double y)
		: x(x), y(y), vx(0), vy(0), radius(10.0), image(-1), isDead(false) {
	}
	virtual ~GameObject() {}	// 継承するのでvirtual

	// 純粋仮想関数（子供のクラスは必ず実装）
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// 共通のゲッター
	int GetX() const { return (int)x; }
	int GetY() const { return (int)y; }
	double GetRadius() const { return radius; }

	// 敵クラス用の関数（Enemyクラスで上書き）
	virtual void Damage(int dmg) {}
	virtual int GetType() const { return -1; }
};
