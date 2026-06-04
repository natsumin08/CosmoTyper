#pragma once

#include "GameObject.h"

// --- Bullet ƒNƒ‰ƒX’è‹` ---
class Bullet : public GameObject {
public:
	Bullet(double x, double y, double vx, double vy, int imgHandle);

	void Update() override;
	void Draw() override;
};
