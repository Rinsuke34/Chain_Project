/* "水の魔法"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Spell_WaterMagic.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Character_Base.h"

// コンストラクタ
Card_Spell_WaterMagic::Card_Spell_WaterMagic() : Card_Spell_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "水の魔法";
	// スートリスト
	this->Suite_List.push_back(SUITE_ELEMENT);	// エレメント
	// ステータス
	this->Strength = 0;					// 攻撃力
	this->Diffence = 0;					// 防御力
	this->AttackRange = ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "WaterMagic";	// 画像の名前
	// 説明文
	this->ExplanationText = "せんとうのてきにXダメージをあたえる/nそのうしろのてきにははんぶんのダメージをあえたる/nそのさらにうしろのてきにはさらにはんぶんのダメージをあたえる/nXは１０+（１０×チェイン）";
}

// カード効果(特殊効果)
void Card_Spell_WaterMagic::Card_Effect_Extra_Process()
{
	/* 前から3体の敵を取得する */
	std::vector<std::shared_ptr<Character_Base>> TargetCharacter;	// 対象キャラクター

	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr && EnemyCharacter->GetHP_Now() > 0)
		{
			// 敵が存在するなら
			/* 配列に追加する */
			TargetCharacter.push_back(EnemyCharacter);
		}
	}

	/* 前から3体の敵にダメージを与える */
	for (int i = 0; i < TargetCharacter.size(); i++)
	{
		// ダメージ量 = 20×(1/2)^(対象の順番)
		int DamageAmount = 20 * pow(0.5, i);
		TargetCharacter[i]->Damage(DamageAmount);
	}
}
