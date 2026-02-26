/* "王国軍の盾"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_RoyalShield.h"

// コンストラクタ
Card_Arms_RoyalShield::Card_Arms_RoyalShield() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "王国軍の盾";
	// スートリスト
	this->Suite_List.push_back(SUITE_SHIELD);	// 盾
	this->Suite_List.push_back(SUITE_ROYAL);	// 王国
	// ステータス
	this->Strength		= 0;					// 攻撃力
	this->Diffence		= 10;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "RoyalShield";		// 画像の名前
	// 説明文
	this->ExplanationText = "じぶんにシールドをふよする/n/cysこうどうまえ/ce/nぼうぎょりょくをXきょうか/nXはチェイン×２";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= true;
	this->Effect_Action_After_Flg	= false;
}

// 効果発動:行動直前
void Card_Arms_RoyalShield::Effect_Action_Before(std::shared_ptr<Character_Base> Target)
{
	// 引数
	// Target : 効果対象のキャラクター

	/* 防御力を加算 */
	this->Add_Diffence_Buff(MyChainCountGet_Buff() * 2);	// 防御力バフ追加:チェイン数 * 2
}
