#pragma once

#include "IScene.h"

class ClearScene : public IScene {
public:
	ClearScene();
	~ClearScene();

	void Update() override;
	void Draw() override;

private:
	int m_timer;
};