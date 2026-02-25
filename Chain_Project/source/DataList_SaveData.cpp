/* データリスト"セーブデータ管理"クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataList_SaveData.h"
// 標準ライブラリ
#include <fstream>
#include <filesystem>
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
	const std::string filePath = "resource/SaveData/SaveData.json";

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
	const std::string filePath = "resource/SaveData/SaveData.json";

	// JSONオブジェクトを構築
	json j;

	// PublicStateの書き込み
	j["PublicState"] =
	{
		{"ClassNo", this->PlayerClassNo},
		{"HaveExp", this->HaveExp}
	};

	// ClassStateの書き込み
	j["ClassState"] = json::array();
	for (int i = 0; i < CLASS_MAX; i++)
	{
		j["ClassState"].push_back({
			{"ClassNo", std::to_string(i)},
			{"Level_Hp", std::to_string(this->Level_Hp[i])},
			{"Level_Ability", std::to_string(this->Level_Ability[i])},
			{"Level_Deck", std::to_string(this->Level_Deck[i])}
			});
	}

	// ディレクトリが存在しない場合は作成
	std::filesystem::path filePathObj(filePath);
	if (filePathObj.has_parent_path())
	{
		std::filesystem::create_directories(filePathObj.parent_path());
	}

	// ファイルに書き込み
	std::ofstream ofs(filePath, std::ios::out | std::ios::trunc);
	if (!ofs.is_open())
	{
		// ファイルを開けない場合はエラー処理
		return;
	}
	ofs << j.dump(4);  // インデント4でフォーマット
}
