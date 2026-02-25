/* "大地の魔法"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Spell_EarthMagic.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Character_Base.h"

// コンストラクタ
Card_Spell_EarthMagic::Card_Spell_EarthMagic() : Card_Spell_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "大地の魔法";
	// スートリスト
	this->Suite_List.push_back(SUITE_ELEMENT);	// エレメント
	// ステータス
	this->Strength		= 0;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "EarthMagic";	// 画像の名前
	// 説明文
	this->ExplanationText = "てきぜんたいにXダメージ/nXは５+（１０×チェイン）";
}

// カード効果(特殊効果)
void Card_Spell_EarthMagic::Card_Effect_Extra_Process()
{
	/* すべての敵に攻撃を行う */
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr && EnemyCharacter->GetHP_Now() > 0)
		{
			// 敵が存在するなら
			// ダメージ量 = 5 + 10×チェイン数
			int DamageAmount = 5 + 10 * this->iNowChainCount;
			EnemyCharacter->Damage(DamageAmount);
		}
	}
}
