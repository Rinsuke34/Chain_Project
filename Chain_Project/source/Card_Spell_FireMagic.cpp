/* "火の魔法"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Spell_FireMagic.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Character_Base.h"

// コンストラクタ
Card_Spell_FireMagic::Card_Spell_FireMagic() : Card_Spell_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "火の魔法";
	// スートリスト
	this->Suite_List.push_back(SUITE_ELEMENT);	// エレメント
	// ステータス
	this->Strength		= 0;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "FireMagic";	// 画像の名前
	// 説明文
	this->ExplanationText = "たいりょくがもっともひくいてきにXだめーじ/nXは１５+（１０×チェイン）";
}

// カード効果(特殊効果)
void Card_Spell_FireMagic::Card_Effect_Extra_Process()
{
	/* 最も残りHPが低い敵を取得する */
	std::shared_ptr<Character_Base> pTargetCharacter = nullptr;	// 対象キャラクター
	int TargetHP = 9999;												// 対象の残りHP
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr)
		{
			// 敵が存在するなら
			if (EnemyCharacter->GetHP_Now() < TargetHP)
			{
				// 対象の残りHPよりも残りHPが低いなら
				pTargetCharacter = EnemyCharacter;	// 対象キャラクターを更新
				TargetHP = EnemyCharacter->GetHP_Now();	// 対象の残りHPを更新
			}
		}
	}

	/* 最もHPが少ない敵にダメージ */
	if (pTargetCharacter != nullptr)
	{
		// 対象キャラクターが存在するなら
		// ダメージ量 = 15 + 10×チェイン数
		int DamageAmount = 15 + 10 * this->iNowChainCount;
		pTargetCharacter->Damage(DamageAmount);
	}
}