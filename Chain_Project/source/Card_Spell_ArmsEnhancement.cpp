/* "武具強化"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Spell_ArmsEnhancement.h"
// 関連クラス
#include "DataList_Battle.h"

// コンストラクタ
Card_Spell_ArmsEnhancement::Card_Spell_ArmsEnhancement() : Card_Spell_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "武具強化";
	// スートリスト
	this->Suite_List.push_back(SUITE_SWORD);	// 剣
	this->Suite_List.push_back(SUITE_SHIELD);	// 盾
	// ステータス
	this->Strength		= 0;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName		= "ArmsEnhancement";	// 画像の名前
}

// カード効果(特殊効果)
void Card_Spell_ArmsEnhancement::Card_Effect_Extra_Process()
{
	/* 手札と山札とバトルエリアの武具カードにチェイン数分の攻撃バフと防御バフを与える */
	// 山札の武具カードにバフを与える
	for (int i = 0; i < this->pDataList_Battle->GetDeckCardList().size(); i++)
	{
		auto DeckCard = this->pDataList_Battle->GetDeckCardList()[i];
		if (DeckCard != nullptr && DeckCard->GetCardType() == TYPE_ARMS)
		{
			DeckCard->Add_Strength_Buff(this->iNowChainCount);
			DeckCard->Add_Diffence_Buff(this->iNowChainCount);
		}
	}
	// 手札の武具カードにバフを与える
	for (int i = 0; i < this->pDataList_Battle->GetHandCardList().size(); i++)
	{
		auto HandCard = this->pDataList_Battle->GetHandCardList()[i];
		if (HandCard != nullptr && HandCard->GetCardType() == TYPE_ARMS)
		{
			HandCard->Add_Strength_Buff(this->iNowChainCount);
			HandCard->Add_Diffence_Buff(this->iNowChainCount);
		}
	}
	// バトルエリアの武具カードにバフを与える
	for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	{
		auto BattleAreaCard = this->pDataList_Battle->GetBattleAreaCardList(i);
		if (BattleAreaCard != nullptr && BattleAreaCard->GetCardType() == TYPE_ARMS)
		{
			BattleAreaCard->Add_Strength_Buff(this->iNowChainCount);
			BattleAreaCard->Add_Diffence_Buff(this->iNowChainCount);
		}
	}
}
