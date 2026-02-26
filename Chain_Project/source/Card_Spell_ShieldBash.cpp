/* "シールドバッシュ"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Spell_ShieldBash.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Character_Base.h"
#include "Action_Effect.h"

// コンストラクタ
Card_Spell_ShieldBash::Card_Spell_ShieldBash() : Card_Spell_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity	= RARITY_COMMON;
	// カード名
	this->Name		= "シールドバッシュ";
	// スートリスト
	this->Suite_List.push_back(SUITE_SHIELD);	// 盾
	// ステータス
	this->Strength		= 0;					// 攻撃力
	this->Diffence		= 0;					// 防御力
	this->AttackRange	= ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName		= "ShieldBash";			// 画像の名前
	// 説明文
	this->ExplanationText = "いちばんまえのてき１たいにXダメージをあたえる/nXはじぶんのシールドりょう";
}

// カード効果(特殊効果)
void Card_Spell_ShieldBash::Card_Effect_Extra_Process()
{
	/* プレイヤーを取得できていないならプレイヤーを取得する */
	CheckHavePlayer();

	/* 与ダメージを算出 */
	// ダメージ量 = プレイヤーのシールド(現在値)
	int DamageAmount = this->pPlayer->GetShield_Now();

	/* 攻撃対象にダメージを与える */
	// 前衛から順に敵が存在するか確認
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr && EnemyCharacter->GetHP_Now() > 0)
		{
			// 敵が存在するなら
			/* 対象にダメージを与える */
			EnemyCharacter->Damage(DamageAmount);
			break;
		}
	}
}
