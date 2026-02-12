/* データリスト"ゲームリソース管理"クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataList_GameResource.h"
// カードクラス
#include "Card_Include.h"
// プレイヤーキャラクタークラス
#include "Character_Player.h"

// コンストラクタ
DataList_GameResource::DataList_GameResource() : DataList_Base("DataList_GameResource")
{
	/* 初期化 */
	this->GameState				= STATE_STAGE;	// ゲームの状態
	this->StageEndFlg			= true;			// ステージ終了フラグ
	this->WoldMapActiveFlg		= false;		// ワールドマップが有効であるかのフラグ
	this->NowMapNode			= nullptr;		// 現在地点のノード
	this->NextStageSelectFlg	= false;		// 次のステージの選択が完了したかのフラグ
	this->GameEndFlg			= false;		// ゲーム終了フラグ
	this->HaveCoin				= 0;			// 所持コイン
	this->DropItemCheckFlg		= false;		// ドロップアイテム確認シーンを有効化
	this->NowStageNo			= 1;			// 現在のステージ番号

	/* プレイヤーキャラクターの生成 */
	this->PlayerCharacter = std::make_shared<Character_Player>();

	/* テスト用のカード生成処理 */
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
	DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	DeckCardList.push_back(std::make_shared<Card_Arms_TravelerSword>());
	DeckCardList.push_back(std::make_shared<Card_Arms_TravelerShield>());
	DeckCardList.push_back(std::make_shared<Card_Item_HealingPotion>());
	DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
	DeckCardList.push_back(std::make_shared<Card_Spell_ArmsEnhancement>());
}

// コインを追加
void DataList_GameResource::AddCoin(int coin)
{
	this->HaveCoin += coin;
}

// ドロップカードを追加
void DataList_GameResource::AddDropCard(std::shared_ptr<Card_Base> card)
{
	this->DropCardList.push_back(card);
}

// ドロップカードリストをクリア
void DataList_GameResource::ClearDropCardList()
{
	this->DropCardList.clear();
}
