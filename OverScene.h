#pragma once

#include "IScene.h"

class OverScene : public IScene {
public:
	OverScene();
	~OverScene();

	void Update() override;
	void Draw() override;

private:
	int m_timer;
};