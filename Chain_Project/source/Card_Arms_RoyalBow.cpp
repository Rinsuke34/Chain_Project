/* "王国軍の弓"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_RoyalBow.h"

// コンストラクタ
Card_Arms_RoyalBow::Card_Arms_RoyalBow() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "王国軍の弓";
	// スートリスト
	this->Suite_List.push_back(SUITE_BOW);		// 弓
	this->Suite_List.push_back(SUITE_ROYAL);	// 王国
	// ステータス
	this->Strength		= 15;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_RANDOM;	// 攻撃範囲
	// 画像の名前
	this->ImageName	= "RoyalBow";		// 画像の名前
	// 説明文
	this->ExplanationText = "ランダムなあいてをこうげきする/n/cysこうどうまえ/ce/nこうげきりょくをXきょうか/nXはチェイン×２";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= true;
	this->Effect_Action_After_Flg	= false;
}

// 効果発動:行動直前
void Card_Arms_RoyalBow::Effect_Action_Before(std::shared_ptr<Character_Base> Target)
{
	// 引数
	// Target : 効果対象のキャラクター

	/* 攻撃力を加算 */
	this->Add_Strength_Buff(MyChainCountGet_Buff() * 2);	// 攻撃力バフ追加:チェイン数
}
