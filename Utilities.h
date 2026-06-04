#pragma once

#include "DxLib.h"
#include "GameObject.h"


// 描画・演出
void drawImage(int img, int x, int y);
void drawText(int x, int y, const char* txt, int val, int col, int siz);
void drawTextC(int x, int y, const char* txt, int col, int siz);
void StartShake(int duration);

// 計算・判定
void calculateBulletVelocity(double targetX, double targetY, double startX, double startY, double& out_vx, double& out_vy);
bool CheckCollision(GameObject* a, GameObject* b);
bool CheckCollision(GameObject* a, struct OBJECT* b);
bool CheckCollision(GameObject* a, int x, int y, int w, int h);	// Player vs Item

// 入力
char getTypedChar();

// 背景・演出
void scrollBG(int spd);
void setEffect(int x, int y, int ptn);
void drawEffect(void);

// アイテム
void setItem(void);
void moveItem(void);
