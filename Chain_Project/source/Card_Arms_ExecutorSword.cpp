/* "執行者の剣"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_ExecutorSword.h"
// 関連クラス
#include "DataList_Battle.h"

// コンストラクタ
Card_Arms_ExecutorSword::Card_Arms_ExecutorSword() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity	= RARITY_RARE;
	// カード名
	this->Name		= "執行者の剣";
	// スートリスト
	this->Suite_List.push_back(SUITE_SWORD);	// 剣
	this->Suite_List.push_back(SUITE_DEATH);	// 死者
	// ステータス
	this->Strength		= 0;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName		= "ExecutorSword";		// 画像の名前
	// 説明文
	this->ExplanationText = "/cysターンかいしじ/ce/nこうげきりょくをXにする\nXはすてられたカードのまいすう";
}

// 効果発動:ターン開始時
void Card_Arms_ExecutorSword::Effect_StartTurn()
{
	/* 捨てられたカード枚数を取得 */
	int Discarded_Card_Count = this->pDataList_Battle->GetTrashCardList().size();

	/* 攻撃力を捨てられたカード枚数にする */
	this->SetStrength_Buff(Discarded_Card_Count - this->Strength);	// 攻撃力バフを設定:捨てられたカード枚数 - 元の攻撃力
}
