/* "木の盾"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_WoodenShield.h"

// コンストラクタ
Card_Arms_WoodenShield::Card_Arms_WoodenShield() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "木の盾";
	// スートリスト
	this->Suite_List.push_back(SUITE_SHIELD);	// 盾
	// ステータス
	this->Strength		= 0;					// 攻撃力
	this->Diffence		= 10;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName		= "WoodenShield";		// 画像の名前
}
