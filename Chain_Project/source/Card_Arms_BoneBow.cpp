/* "骨の弓"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_BoneBow.h"
// 関連クラス
#include "DataList_Battle.h"

// コンストラクタ
Card_Arms_BoneBow::Card_Arms_BoneBow() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "骨の弓";
	// スートリスト
	this->Suite_List.push_back(SUITE_BOW);		// 弓
	this->Suite_List.push_back(SUITE_DEATH);	// 死
	// ステータス
	this->Strength		= 15;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_RANDOM;	// 攻撃範囲
	// 画像の名前
	this->ImageName		= "BoneBow";		// 画像の名前
	// 説明文
	this->ExplanationText = "ランダムなあいてをこうげきする/n/cysターンかいしじ/ce/nこうげきりょくをXにする\nXはすてられたカードのまいすう+１５";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= false;
	this->Effect_Action_After_Flg	= false;
}

// 効果発動:ターン開始時
void Card_Arms_BoneBow::Effect_StartTurn()
{
	/* 捨てられたカード枚数を取得 */
	int Discarded_Card_Count = this->pDataList_Battle->GetTrashCardList().size();

	/* 攻撃力を所持しているコイン数にする */
	this->Strength = 15 + Discarded_Card_Count;

	/* 永続的に強化されているなら、ステータスを水色で描写するようにする */
	if (this->Strength > 15)
	{
		this->StatusDrawBaseColor[0] = GetColor(0, 255, 255);	// 水色
	}

	/* 画像の更新 */
	this->UpdateImage();
}
