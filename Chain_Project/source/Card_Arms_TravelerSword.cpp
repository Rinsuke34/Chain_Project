/* "旅人の剣"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_TravelerSword.h"

// コンストラクタ
Card_Arms_TravelerSword::Card_Arms_TravelerSword() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "旅人の剣";
	// スートリスト
	this->Suite_List.push_back(SUITE_SWORD);	// 剣
	this->Suite_List.push_back(SUITE_TRAVELER);	// 旅人
	// ステータス
	this->Strength		= 10;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "TravelerSword";		// 画像の名前
}

// 効果発動:行動直前
void Card_Arms_TravelerSword::Effect_Action_Before(std::shared_ptr<Character_Base> Target)
{
	// 引数
	// Target : 効果対象のキャラクター

	/* 攻撃力を加算 */
	this->Add_Strength_Buff(GetNowChainCount());	// 攻撃力バフ追加:チェイン数
}
