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

	/* HP設定 */
	int Level_Hp	= SaveData->GetLevel_Hp(SaveData->GetPlayerClassNo());	// HPレベル
	this->iHP_Max	= 20 + Level_Hp * 5;		// 体力(最大値) = 基本値 + HPレベル * 5
	this->iHP_Now	= this->iHP_Max;			// 体力(現在値) = 体力(最大値)

	/* 画像設定 */
	switch (SaveData->GetPlayerClassNo())
	{
		case DataList_SaveData::CLASS_TRAVELER:		SetUpImage("Character_Ilust/Player/Player_Traveler");	break;	// 旅人
		case DataList_SaveData::CLASS_SWORDSMAN:	SetUpImage("Character_Ilust/Player/Player_Swordsman");	break;	// 剣士
		case DataList_SaveData::CLASS_WIZARD:		SetUpImage("Character_Ilust/Player/Player_Wizard");		break;	// 魔法使い
		case DataList_SaveData::CLASS_ROGUE:		SetUpImage("Character_Ilust/Player/Player_Rogue");		break;	// 盗賊
		case DataList_SaveData::CLASS_MARCHANT:		SetUpImage("Character_Ilust/Player/Player_Marchant");	break;	// 商人
		default:									SetUpImage("Character_Ilust/Player/Player_Traveler");	break;	// 旅人(デフォルト)
	}
}
