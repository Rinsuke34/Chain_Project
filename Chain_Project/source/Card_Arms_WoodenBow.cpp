/* "木の弓"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_WoodenBow.h"

// コンストラクタ
Card_Arms_WoodenBow::Card_Arms_WoodenBow() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "木の弓";
	// スートリスト
	this->Suite_List.push_back(SUITE_BOW);	// 弓
	// ステータス
	this->Strength		= 8;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_RANDOM;	// 攻撃範囲:ランダム
	// 画像の名前
	this->ImageName		= "WoodenBow";			// 画像の名前
	// 説明文
	this->ExplanationText = "ランダムなあいてをこうげきする";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= false;
	this->Effect_Action_After_Flg	= false;
}
