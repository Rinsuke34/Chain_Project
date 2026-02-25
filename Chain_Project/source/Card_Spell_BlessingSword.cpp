/* "剣の加護"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Spell_BlessingSword.h"
// 関連クラス
#include "DataList_Battle.h"

// コンストラクタ
Card_Spell_BlessingSword::Card_Spell_BlessingSword() : Card_Spell_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "剣の加護";
	// スートリスト
	this->Suite_List.push_back(SUITE_ELEMENT);	// エレメント
	// ステータス
	this->Strength = 0;					// 攻撃力
	this->Diffence = 0;					// 防御力
	this->AttackRange = ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "BlessingSword";	// 画像の名前
	// 説明文
	this->ExplanationText = "";
}

// カード効果(特殊効果)
void Card_Spell_BlessingSword::Card_Effect_Extra_Process()
{

}
