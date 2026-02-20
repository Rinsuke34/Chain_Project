/* "守護のポーション"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Item_GuardianPotion.h"
// 関連クラス	
#include "DataList_Battle.h"
#include "Character_Base.h"
#include "Action_Effect.h"
#include "Buff_Debuff.h"

// コンストラクタ
Card_Item_GuardianPotion::Card_Item_GuardianPotion() : Card_Item_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity	= RARITY_RARE;
	// カード名
	this->Name		= "守護の薬";
	// スートリスト
	this->Suite_List.push_back(SUITE_POTION);	// 薬(ポーション)
	// 画像の名前
	this->ImageName = "GuardianPotion";			// 画像の名前
	// 説明文
	this->ExplanationText = "じぶんにぼうぎょりょくアップじょうたいをふよ/n/cysこうかりょう/ce/nチェインすう";
}

// 戦闘行動
void Card_Item_GuardianPotion::BattleAction()
{
	/* プレイヤーを取得できていないならプレイヤーを取得する */
	CheckHavePlayer();

	/* プレイヤーに防御力強化を設定する */
	/* プレイヤー自身が"状態異常：防御力強化"を所持しているか確認 */
	std::shared_ptr<Character_Buff_Debuff_Base> GuardianBuff = this->pPlayer->CheckGet_Buff_Debuff("Buff_Diffence");
	if (GuardianBuff != nullptr)
	{
		// 所持しているなら
		/* 残りターン数をチェイン数分加算する */
		GuardianBuff->Buff_Debuff_Time += GetNowChainCount();	// 残りターン数 += チェイン数
	}
	else
	{
		// 所持していないなら
		/* "状態異常：防御力強化"を付与する */
		std::shared_ptr<Character_Buff_Debuff_Base> New_Guardian_Buff = std::make_shared<Character_Buff_Diffence>();
		New_Guardian_Buff->Buff_Debuff_Time	= GetNowChainCount();	// 残りターン数 = チェイン数
		this->pPlayer->Add_Buff_Debuff(New_Guardian_Buff);
	}
}
