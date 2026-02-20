/* "嵐の弓"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_StormBow.h"

// コンストラクタ
Card_Arms_StormBow::Card_Arms_StormBow() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_EPIC;
	// カード名
	this->Name = "旅人の弓";
	// スートリスト
	this->Suite_List.push_back(SUITE_BOW);		// 弓
	this->Suite_List.push_back(SUITE_ELEMENT);	// エレメント
	// ステータス
	this->Strength		= 15;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_RANDOM;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "StormBow";		// 画像の名前
	// 説明文
	this->ExplanationText	= "Xかいこうげきする/nXはチェイン+１";

	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= false;
	this->Effect_Action_After_Flg	= false;
}

// 戦闘行動
void Card_Arms_StormBow::BattleAction()
{
	/* 親クラスの戦闘行動処理をチェイン回数+1回分行う */
	for (int i = 0; i < MyChainCountGet_Buff() + 1; i++)
	{
		Card_Base::BattleAction();
	}
}
