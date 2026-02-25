/* キャラクター(ロボット(セキュリティ))の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Npc_Robot_Security.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Card_Base.h"

// コンストラクタ
Character_Npc_Robot_Security::Character_Npc_Robot_Security() : Character_Base()
{
	/* 初期化 */
	this->iHP_Max	= 15;			// 体力(最大値)
	this->iHP_Now	= 15;			// 体力(現在値)
	this->Camp		= Character_Base::CAMP_ENEMY;	// 陣営:敵陣営
	this->SizeX		= 300;			// キャラクターの幅
	this->SizeY		= 300;			// キャラクターの高さ
	this->DropCoin	= 5;			// ドロップするコインの枚数
	SetUpImage("Character_Ilust/Npc/Robot_Security");

	/* ドロップするカードの設定 */
	DropItemSet(Card_Base::RARITY_RARE);
	DropItemSet(Card_Base::RARITY_RARE);
}

// 行動
void Character_Npc_Robot_Security::Action()
{
	/* プレイヤー側の最も前のキャラクターに対して攻撃を作成する */
	if (this->pDataList_Battle != nullptr)
	{
		// 最も前の仲間キャラクターを取得
		std::shared_ptr<Character_Base> pTargetCharacter = nullptr;
		for (int i = DataList_Battle::POSITION_FRONT; i < DataList_Battle::POSITION_MAX; i++)
		{
			pTargetCharacter = this->pDataList_Battle->GetFriendCharacter(i);
			if (pTargetCharacter != nullptr && pTargetCharacter->GetHP_Now() > 0)
			{
				// ランダムなバトルエリアを対象とする
				int BattleAreaNo = GetRand(DataList_Battle::BATTLE_AREA_MAX - 1);

				// 攻撃行動を設定する
				std::shared_ptr<Action_Effect_Attack> addEffect = std::make_shared<Action_Effect_Attack>();
				addEffect->Target_Camp		= Character_Base::CAMP_FRIEND;	// 効果対象の陣営:味方
				addEffect->Target_Position	= i;							// 効果対象の立ち位置:確認した敵キャラクターの位置
				addEffect->DamageAmount		= 15;							// ダメージ量
				addEffect->EffectUser		= shared_from_this();			// 効果の使用者:自分自身
				addEffect->Priority			= 0;							// 優先順位：最遅
				addEffect->EffectCard		= nullptr;						// 効果を使用するカード:無し
				this->pDataList_Battle->AddEffect(addEffect);
				this->ActionEffectList.push_back(addEffect);
				break;
			}
		}
	}

	/* 自分自身のHPを回復 */
	std::shared_ptr<Action_Effect_Heal> addEffect = std::make_shared<Action_Effect_Heal>();
	addEffect->Target_Camp		= Character_Base::CAMP_ENEMY;	// 効果対象の陣営:敵
	addEffect->Target_Position	= -1;							// 効果対象の立ち位置:無し
	addEffect->HealAmount		= 10;							// 回復量
	addEffect->EffectUser		= shared_from_this();			// 効果の使用者:自分自身
	addEffect->Priority			= 0;							// 優先順位：最遅
	addEffect->EffectCard		= nullptr;						// 効果を使用するカード:無し
	this->pDataList_Battle->AddEffect(addEffect);
	this->ActionEffectList.push_back(addEffect);
}
