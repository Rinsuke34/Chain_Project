/* データリスト"セーブデータ管理"クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataList_SaveData.h"
// 標準ライブラリ
#include <fstream>
#include <nlohmann/json.hpp>

// コンストラクタ
DataList_SaveData::DataList_SaveData() : DataList_Base("DataList_SaveData")
{
	/* 初期化 */
	this->HaveExp					= 0;	// 所持経験値
	this->PlayerClassNo				= 0;	// プレイヤークラス番号
	for (int i = 0; i < CLASS_MAX; i++)
	{
		this->Level_Hp[i]		= 0;	// クラスごとのHPレベル
		this->Level_Ability[i]	= 0;	// クラスごとのアビリティレベル
		this->Level_Deck[i]		= 0;	// クラスごとのデッキレベル
	}

	/* セーブデータ読み込み */
	SaveData_Load();
}

// デストラクタ
DataList_SaveData :: ~DataList_SaveData()
{
	/* セーブデータ保存 */
	SaveData_Save();
}

// セーブデータ読み込み
void DataList_SaveData::SaveData_Load()
{
	using json = nlohmann::json;

	// ファイルパス
	const std::string filePath = "resource/SetupData/SaveData.json";

	std::ifstream ifs(filePath);
	if (!ifs.is_open()) {
		// ファイルが存在しない場合は何もしない
		return;
	}

	json j;
	ifs >> j;

	// PublicStateの読み込み
	if (j.contains("PublicState")) {
		const auto& pub = j["PublicState"];
		if (pub.contains("ClassNo")) {
			this->PlayerClassNo = pub["ClassNo"].get<int>();
		}
		if (pub.contains("HaveExp")) {
			this->HaveExp = pub["HaveExp"].get<int>();
		}
	}

	// ClassStateの読み込み
	if (j.contains("ClassState") && j["ClassState"].is_array()) {
		for (const auto& cls : j["ClassState"]) {
			if (!cls.contains("ClassNo")) continue;
			int classNo = std::stoi(cls["ClassNo"].get<std::string>());
			if (classNo < 0 || classNo >= CLASS_MAX) continue;

			if (cls.contains("Level_Hp")) {
				this->Level_Hp[classNo] = std::stoi(cls["Level_Hp"].get<std::string>());
			}
			if (cls.contains("Level_Ability")) {
				this->Level_Ability[classNo] = std::stoi(cls["Level_Ability"].get<std::string>());
			}
			if (cls.contains("Level_Deck")) {
				this->Level_Deck[classNo] = std::stoi(cls["Level_Deck"].get<std::string>());
			}
		}
	}
}

// セーブデータ保存
void DataList_SaveData::SaveData_Save()
{
	using json = nlohmann::json;

	// ファイルパス
	const std::string filePath = "resource/SetupData/SaveData.json";

	std::ifstream ifs(filePath);
	if (!ifs.is_open()) {
		// ファイルが存在しない場合は何もしない
		return;
	}

	json j;
	ifs >> j;

	// PublicStateの読み込み
	if (j.contains("PublicState")) {
		const auto& pub = j["PublicState"];
		if (pub.contains("ClassNo")) {
			this->PlayerClassNo = pub["ClassNo"].get<int>();
		}
		if (pub.contains("HaveExp")) {
			this->HaveExp = pub["HaveExp"].get<int>();
		}
	}

	// ClassStateの読み込み
	if (j.contains("ClassState") && j["ClassState"].is_array()) {
		for (const auto& cls : j["ClassState"]) {
			if (!cls.contains("ClassNo")) continue;
			int classNo = std::stoi(cls["ClassNo"].get<std::string>());
			if (classNo < 0 || classNo >= CLASS_MAX) continue;

			if (cls.contains("Level_Hp")) {
				this->Level_Hp[classNo] = std::stoi(cls["Level_Hp"].get<std::string>());
			}
			if (cls.contains("Level_Ability")) {
				this->Level_Ability[classNo] = std::stoi(cls["Level_Ability"].get<std::string>());
			}
			if (cls.contains("Level_Deck")) {
				this->Level_Deck[classNo] = std::stoi(cls["Level_Deck"].get<std::string>());
			}
		}
	}
}
