/* データリスト"バトル用データ管理"クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataList_Battle.h"


// テスト用
#include "Card_Arms_Base.h"
#include "Card_Spell_Base.h"
#include "Card_Item_Base.h"
#include "Character_Player.h"
#include "Character_Npc_Test.h"
#include "Card_Arms_WoodenSword.h"
#include "Card_Arms_WoodenShield.h"

// コンストラクタ
DataList_Battle::DataList_Battle() : DataList_Base("DataList_Battle")
{
	/* テスト用 */
	// 手札にカードを追加
	this->DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	this->DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	this->DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	this->DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	this->DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	this->DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	this->DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	this->DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	this->DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	this->DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());

	// 画像更新
	for (auto& Deck : DeckCardList)
	{
		Deck->UpdateImage();
	}

	/* テスト用 */
	// キャラクターを追加
	this->Friend_CharacterList[POSITION_FRONT]	= nullptr;
	this->Friend_CharacterList[POSITION_MIDDLE]	= std::make_shared<Character_Player>();
	this->Friend_CharacterList[POSITION_BACK]	= nullptr;

	this->Enemy_CharacterList[POSITION_FRONT]	= std::make_shared<Character_Npc_Test>();
	this->Enemy_CharacterList[POSITION_MIDDLE]	= std::make_shared<Character_Npc_Test>();
	this->Enemy_CharacterList[POSITION_BACK]	= std::make_shared<Character_Npc_Test>();
}

// デッキにカードを追加
void DataList_Battle::AddDeckCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->DeckCardList.push_back(pCard);
}

// 手札にカードを追加
void DataList_Battle::AddHandCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->HandCardList.push_back(pCard);
}

// 捨て札にカードを追加
void DataList_Battle::AddTrashCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->TrashCardList.push_back(pCard);
}

// デッキからカードを削除
void DataList_Battle::RemoveDeckCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->DeckCardList.erase(std::remove(DeckCardList.begin(), DeckCardList.end(), pCard),DeckCardList.end());
}

// 手札からカードを削除
void DataList_Battle::RemoveHandCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->HandCardList.erase(std::remove(HandCardList.begin(), HandCardList.end(), pCard), HandCardList.end());
}

// 捨て札からカードを削除
void DataList_Battle::RemoveTrashCard(const std::shared_ptr<Card_Base>& pCard)
{
	// 引数
	// pCard <- 追加するカードのポインタ
	this->TrashCardList.erase(std::remove(TrashCardList.begin(), TrashCardList.end(), pCard), TrashCardList.end());
}

// バトルエリアからカードを削除
void DataList_Battle::RemoveBattleAreaCard(int AreaNo)
{
	// 引数
	// AreaNo <- バトルエリア番号
	this->BattleAreaCardList[AreaNo] = nullptr;
}

// 与効果を追加
void DataList_Battle::AddEffect(const Card_Effect_Base& effect)
{
	// 引数
	// effect <- 追加する与効果

	this->EffectList.push_back(effect);
}
