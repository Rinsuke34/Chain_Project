/* "毒のポーション"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Item_PoisonPotion.h"
// 関連クラス	
#include "DataList_Battle.h"
#include "Character_Base.h"
#include "Action_Effect.h"
#include "Buff_Debuff.h"

// コンストラクタ
Card_Item_PoisonPotion::Card_Item_PoisonPotion() : Card_Item_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "毒薬";
	// スートリスト
	this->Suite_List.push_back(SUITE_POTION);	// 薬(ポーション)
	this->Suite_List.push_back(SUITE_POISON);	// 毒
	// 画像の名前
	this->ImageName = "PoisonPotion";			// 画像の名前
}

// カード効果(特殊効果)
void Card_Item_PoisonPotion::Card_Effect_Extra_Process()
{
	/* 敵全体に毒の状態異常を付与する */
	// 前衛から順に敵が存在するか確認
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr)
		{
			// 敵が存在するなら
			/* 攻撃対象が"状態異常：毒"を所持しているか確認 */
			std::vector<std::shared_ptr<Character_Buff_Debuff_Base>> Poison = EnemyCharacter->CheckGet_Buff_Debuff("Debuff_Poison");
			if (Poison.size() > 0)
			{
				// 所持しているなら
				/* 残りターン数をチェイン数分加算する */
				for (auto& debuff : Poison)
				{
					debuff->Buff_Debuff_Time += GetNowChainCount();	// 残りターン数 += チェイン数
				}
			}
			else
			{
				// 所持していないなら
				/* "状態異常：毒"を付与する */
				std::shared_ptr<Character_Debuff_Poison> New_Poison_Debuff = std::make_shared<Character_Debuff_Poison>();
				New_Poison_Debuff->Buff_Debuff_Time = GetNowChainCount() + 1;	// 残りターン数 = チェイン数 + 1
				EnemyCharacter->Add_Buff_Debuff(New_Poison_Debuff);
			}
		}
	}
}