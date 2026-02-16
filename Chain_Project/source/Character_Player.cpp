/* キャラクター(プレイヤー)の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Player.h"
// 関連クラス
#include "DataList_SaveData.h"

// コンストラクタ
Character_Player::Character_Player() : Character_Base()
{
	/* 初期化 */
	this->iHP_Max	= 20;							// 体力(最大値)
	this->iHP_Now	= this->iHP_Max;				// 体力(現在値)
	this->Camp		= Character_Base::CAMP_FRIEND;	// 陣営:仲間陣営
	this->SizeX		= PLAYER_WIDE;					// キャラクターの幅
	this->SizeY		= PLAYER_HEIGHT;				// キャラクターの高さ
	SetUpImage("Character_Ilust/Player/Player_Traveler");

	/* プレイヤーのセットアップ */
	PlayerSetup();
}

// プレイヤーのセットアップ
void Character_Player::PlayerSetup()
{
	/* データリスト"セーブデータ"を取得 */
	std::shared_ptr<DataList_SaveData> SaveData = std::dynamic_pointer_cast<DataList_SaveData>(gpDataListServer->GetDataList("DataList_SaveData"));

	/* 画像、HPの設定 */
	int Level_Hp = SaveData->GetLevel_Hp(SaveData->GetPlayerClassNo());	// HPレベル
	int Hp_IncreaseAmount = 0;											// レベルごとのHP上昇量
	switch (SaveData->GetPlayerClassNo())
	{
		// 旅人
		case DataList_SaveData::CLASS_TRAVELER:
			SetUpImage("Character_Ilust/Player/Player_Traveler");
			Hp_IncreaseAmount = 3;
			break;

		// 剣士
		case DataList_SaveData::CLASS_SWORDSMAN:
			SetUpImage("Character_Ilust/Player/Player_Swordsman");
			Hp_IncreaseAmount = 3;
			break;

		// 魔法使い
		case DataList_SaveData::CLASS_WIZARD:
			SetUpImage("Character_Ilust/Player/Player_Wizard");
			Hp_IncreaseAmount = 1;
			break;

		// 盗賊
		case DataList_SaveData::CLASS_ROGUE:
			SetUpImage("Character_Ilust/Player/Player_Rogue");
			Hp_IncreaseAmount = 2;
			break;

		// 商人
		case DataList_SaveData::CLASS_MARCHANT:
			SetUpImage("Character_Ilust/Player/Player_Marchant");
			Hp_IncreaseAmount = 2;
			break;

		// 旅人(デフォルト)
		default:
			SetUpImage("Character_Ilust/Player/Player_Traveler");
			Hp_IncreaseAmount = 3;
			break;
	}

	this->iHP_Max = 20 + (Level_Hp * Hp_IncreaseAmount);
	this->iHP_Now = this->iHP_Max;
}
