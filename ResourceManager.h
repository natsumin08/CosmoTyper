#pragma once

#include <map>
#include <string>
#include <vector>

#include "DxLib.h"

class ResourceManager {
public:
	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	// コピー禁止
	ResourceManager(const ResourceManager&) = delete;
	void operator = (const ResourceManager&) = delete;

	// --- メイン機能 ---

	// 画像ハンドルを取得
	int GetGraph(const std::string& fileName);

	// 音源ハンドルを取得
	int GetSound(const std::string& fileName);
	
	// フォントハンドル
	int GetFont(const std::string& fontName, int size, int thick, int type);

	// 全リソースの削除
	void ClearAll();

private:
	ResourceManager() {}
	~ResourceManager() { ClearAll(); }

	// std::map <ファイル名、ハンドル番号>
	std::map<std::string, int> m_graphs;
	std::map<std::string, int> m_sounds;
	std::map<std::string, int> m_fonts;
};

#define RM ResourceManager::GetInstance()