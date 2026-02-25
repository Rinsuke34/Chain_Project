/* "風の魔法"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Spell_WindMagic.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Character_Base.h"

// コンストラクタ
Card_Spell_WindMagic::Card_Spell_WindMagic() : Card_Spell_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "風の魔法";
	// スートリスト
	this->Suite_List.push_back(SUITE_ELEMENT);	// エレメント
	// ステータス
	this->Strength = 0;					// 攻撃力
	this->Diffence = 0;					// 防御力
	this->AttackRange = ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "WindMagic";	// 画像の名前
	// 説明文
	this->ExplanationText = "ランダムなてきにXダメージ/nこのしょりをYかいおこなう/nXは１５+（１０×チェイン）/nYはチェイン+１";
}

// カード効果(特殊効果)
void Card_Spell_WindMagic::Card_Effect_Extra_Process()
{
	/* 生存している敵を配列に入れる */
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

	/* ランダムな敵にダメージを与える処理をチェイン数+1回繰り返す */
	int ProcessCount = this->iNowChainCount + 1;	// 処理回数 = チェイン数 + 1
	for (int i = 0; i < ProcessCount; i++)
	{
		if (TargetCharacter.size() == 0)
		{
			// 対象キャラクターが存在しないなら処理を終了する
			break;
		}
		/* ランダムな敵を選択する */
		int TargetIndex				= GetRand(TargetCharacter.size() - 1);	// 対象のインデックス
		auto TargetEnemyCharacter	= TargetCharacter[TargetIndex];	// 対象の敵キャラクター

		/* ダメージ量 = 15 + 10×チェイン数 */
		int DamageAmount = 15 + 10 * this->iNowChainCount;

		/* 対象の敵にダメージを与える */
		TargetEnemyCharacter->Damage(DamageAmount);

		/* 対象の敵が倒れたなら配列から削除する */
		if (TargetEnemyCharacter->GetHP_Now() <= 0)
		{
			TargetCharacter.erase(TargetCharacter.begin() + TargetIndex);
		}
	}
}
