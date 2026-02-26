/* "妖刀"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_CursedKatana.h"
// 関連クラス
#include "Buff_Debuff.h"
#include "Character_Base.h"

// コンストラクタ
Card_Arms_CursedKatana::Card_Arms_CursedKatana() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity	= RARITY_RARE;
	// カード名
	this->Name		= "妖刀";
	// スートリスト
	this->Suite_List.push_back(SUITE_SWORD);	// 剣
	// ステータス
	this->Strength		= 20;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange = ATTACKRANGE_FRONT;		// 攻撃範囲
	// 画像の名前
	this->ImageName = "CursedKatana";			// 画像の名前
	// 説明文
	this->ExplanationText = "いちばんまえのてきをこうげきする/cysこうげきご/ce/nXかいふくする/nXはこれのこうげきりょく";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= false;
	this->Effect_Action_After_Flg	= true;
}

// 効果発動:行動時
void Card_Arms_CursedKatana::Effect_Action_After(std::shared_ptr<Character_Base> Target)
{
	/* 攻撃バフを取得 */
	std::shared_ptr<Character_Buff_Debuff_Base> Strength = this->pPlayer->CheckGet_Buff_Debuff("Buff_Strength");

	/* これの攻撃力を取得(バフ込み) */
	int Total_Strength = this->Strength + this->Strength_Buff + Strength->Buff_Debuff_Time;

	/* プレイヤーに回復を行う */
	this->pPlayer->Heal(Total_Strength);
}
