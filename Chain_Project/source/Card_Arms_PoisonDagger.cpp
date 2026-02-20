/* "毒ナイフ"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_PoisonDagger.h"
// 関連クラス
#include "Character_Base.h"
#include "Buff_Debuff.h"

// コンストラクタ
Card_Arms_PoisonDagger::Card_Arms_PoisonDagger() : Card_Arms_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_RARE;
	// カード名
	this->Name = "毒ナイフ";
	// スートリスト
	this->Suite_List.push_back(SUITE_DAGGER);	// 短剣
	this->Suite_List.push_back(SUITE_POISON);	// 毒
	// ステータス
	this->Strength		= 1;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName	= "PoisonDagger";		// 画像の名前
	// 説明文
	this->ExplanationText = "/cysこうげきじ/ce/nこうげきたいしょうにどくをあたえる/n/cysこうかりょう/ce/nチェインすう+５";
	// 行動前後の効果を実行するかのフラグ
	this->Effect_Action_Before_Flg	= false;
	this->Effect_Action_After_Flg	= true;
}

// 効果発動:行動時
void Card_Arms_PoisonDagger::Effect_Action_After(std::shared_ptr<Character_Base> Target)
{
	// 引数
	// Target : 効果対象のキャラクター

	/* 攻撃対象が"状態異常：毒"を所持しているか確認 */
	std::shared_ptr<Character_Buff_Debuff_Base> Poison = Target->CheckGet_Buff_Debuff("Debuff_Poison");
	if (Poison != nullptr)
	{
		// 所持しているなら
		/* 残りターン数をチェイン数分加算する */
		Poison->Buff_Debuff_Time += MyChainCountGet_Buff() + 5;	// 残りターン数 += チェイン数 + 5
	}
	else
	{
		// 所持していないなら
		/* "状態異常：毒"を付与する */
		std::shared_ptr<Character_Debuff_Poison> New_Poison_Debuff = std::make_shared<Character_Debuff_Poison>();
		New_Poison_Debuff->Buff_Debuff_Time = MyChainCountGet_Buff() + 5;	// 残りターン数 = チェイン数 + 5
		Target->Add_Buff_Debuff(New_Poison_Debuff);
	}
}
