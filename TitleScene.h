#pragma once

#include "IScene.h"

class TitleScene : public IScene {
public:
	TitleScene();
	~TitleScene();

	void Update() override;
	void Draw() override;
};