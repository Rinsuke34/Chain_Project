/* "王国軍の剣"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_RoyalSword.h"

// コンストラクタ
Card_Arms_RoyalSword::Card_Arms_RoyalSword() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "王国軍の剣";
	// スートリスト
	this->Suite_List.push_back(SUITE_SWORD);	// 剣
	this->Suite_List.push_back(SUITE_ROYAL);	// 王国
	// ステータス
	this->Strength	= 10;					// 攻撃力
	this->Diffence	= 0;					// 防御力
	this->AttackRange = ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "RoyalSword";			// 画像の名前
	// 説明文
	this->ExplanationText = "いちばんまえのてきをこうげきする/n/cysこうどうまえ/ce/nこうげきりょくをXきょうか/nXはチェイン×２";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= true;
	this->Effect_Action_After_Flg	= false;
}

// 効果発動:行動直前
void Card_Arms_RoyalSword::Effect_Action_Before(std::shared_ptr<Character_Base> Target)
{
	// 引数
	// Target : 効果対象のキャラクター

	/* 攻撃力を加算 */
	this->Add_Strength_Buff(MyChainCountGet_Buff() * 2);	// 攻撃力バフ追加:チェイン数 * 2
}
