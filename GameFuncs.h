#pragma once

#include "Constants.h"
#include "GameObject.h"

// --- シーン更新・描画関数 ---
void scrollBG(int spd);
void moveItem(void);
void drawEffect(void);

// --- ユーティリティ関数（各クラスやSceneMgr.cpp内で使われる）---
void setEffect(int x, int y, int ptn);
void setItem(void);
