#pragma once

// インターフェースクラス（純粋仮想関数のみを持つ）
class IScene {
public:
	// 仮想デストラクタ
	virtual ~IScene() {}

	// 全てのシーンが必ず持たなければならない機能
	virtual void Update() = 0;	// 更新処理
	virtual void Draw() = 0;	// 描画処理
};