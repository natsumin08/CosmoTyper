#include "ResourceManager.h"

// 画像ハンドル取得
int ResourceManager::GetGraph(const std::string& fileName)
{
	// すでに読み込み済みかチェック（キーが存在すれば1を返す）
	if (m_graphs.count(fileName) > 0)
		return m_graphs[fileName];

	// まだないなら読み込む
	int handle = LoadGraph(fileName.c_str());

	if (handle == -1)
		return -1;	// 読み込み失敗

	// マップに登録して返す
	m_graphs[fileName] = handle;
	return handle;
}

// 音源ハンドル取得
int ResourceManager::GetSound(const std::string& fileName)
{
	// すでに読み込み済みかチェック（キーが存在すれば1を返す）
	if (m_sounds.count(fileName) > 0)
		return m_sounds[fileName];

	// まだないなら読み込む
	int handle = LoadSoundMem(fileName.c_str());

	if (handle == -1)
		return -1;	// 読み込み失敗

	// マップに登録して返す
	m_sounds[fileName] = handle;
	return handle;
}

// フォント取得の実装
int ResourceManager::GetFont(const std::string& fontName, int size, int thick, int type)
{
	std::string key = fontName + "_" + std::to_string(size) + "_" +
		std::to_string(thick) + "_" + std::to_string(type);

	if (m_fonts.count(key) > 0)
		return m_fonts[key];

	int handle = CreateFontToHandle(fontName.c_str(), size, thick, type);

	if (handle == -1) return -1;

	m_fonts[key] = handle;
	return handle;
}

// 全削除
void ResourceManager::ClearAll()
{
	for (auto& pair : m_graphs)
		DeleteGraph(pair.second);
	m_graphs.clear();

	for (auto& pair : m_sounds)
		DeleteSoundMem(pair.second);
	m_sounds.clear();

	for (auto& pair : m_fonts)
		DeleteFontToHandle(pair.second);
	m_fonts.clear();
}