/* "力のポーション"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Item_PowerPotion.h"
// 関連クラス	
#include "DataList_Battle.h"
#include "Character_Base.h"
#include "Action_Effect.h"

// コンストラクタ
Card_Item_PowerPotion::Card_Item_PowerPotion() : Card_Item_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity	= RARITY_COMMON;
	// カード名
	this->Name		= "力の薬";
	// スートリスト
	this->Suite_List.push_back(SUITE_POTION);	// 薬(ポーション)
	// 画像の名前
	this->ImageName = "PowerPotion";			// 画像の名前
	// 説明文
	this->ExplanationText = "みかたぜんいんにこうげききょうか/n/cysこうかりょう/ce/n５+チェインすう×５";
}

// 戦闘行動
void Card_Item_PowerPotion::BattleAction()
{

}
