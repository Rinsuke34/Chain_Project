/* "硝子の短剣"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_GrassDagger.h"

// コンストラクタ
Card_Arms_GrassDagger::Card_Arms_GrassDagger() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_RARE;
	// カード名
	this->Name	= "硝子の短剣";
	// スートリスト
	this->Suite_List.push_back(SUITE_DAGGER);	// 短剣
	this->Suite_List.push_back(SUITE_GRASS);	// ガラス
	// ステータス
	this->Strength		= 20;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName		= "GrassDagger";		// 画像の名前
	// 説明文
	this->ExplanationText = "";
}
