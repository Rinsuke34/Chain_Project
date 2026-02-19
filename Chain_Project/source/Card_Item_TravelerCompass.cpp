/* "旅人のコンパス"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Item_TravelerCompass.h"
// 関連クラス	
#include "DataList_Battle.h"

// コンストラクタ
Card_Item_TravelerCompass::Card_Item_TravelerCompass() : Card_Item_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity	= RARITY_COMMON;
	// カード名
	this->Name		= "旅人のコンパス";
	// スートリスト
	this->Suite_List.push_back(SUITE_TRAVELER);	// 旅人
	// 画像の名前
	this->ImageName = "TravelerCompass";			// 画像の名前
	// 説明文
	this->ExplanationText = "";
}

// 戦闘行動
void Card_Item_TravelerCompass::BattleAction()
{

}
