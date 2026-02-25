/* キャラクター(ゴーレム(石))の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Npc_Golem_Stone.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Card_Base.h"

// コンストラクタ
Character_Npc_Golem_Stone::Character_Npc_Golem_Stone() : Character_Base()
{
	/* 初期化 */
	this->iHP_Max	= 50;			// 体力(最大値)
	this->iHP_Now	= 50;			// 体力(現在値)
	this->Camp		= Character_Base::CAMP_ENEMY;	// 陣営:敵陣営
	this->SizeX		= 150;			// キャラクターの幅
	this->SizeY		= 150;			// キャラクターの高さ
	this->DropCoin	= 5;			// ドロップするコインの枚数
	SetUpImage("Character_Ilust/Npc/Golem_Stone");

	/* ドロップするカードの設定 */
	DropItemSet(Card_Base::RARITY_RARE);
	DropItemSet(Card_Base::RARITY_RARE);
}

// 行動
void Character_Npc_Golem_Stone::Action()
{
	/* ランダムに行動を行う */
	int RandAction = GetRand(1);
	switch (RandAction)
	{
		/* 防御 */
		case 0:
		{
			/* 自分自身の立ち位置を取得 */
			int MyPosition = -1;
			for (int i = DataList_Battle::POSITION_FRONT; i < DataList_Battle::POSITION_MAX; i++)
			{
				std::shared_ptr<Character_Base> pCheckCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
				if (pCheckCharacter != nullptr && pCheckCharacter->GetHP_Now() > 0)
				{
					if (pCheckCharacter.get() == this)
					{
						MyPosition = i;
						break;
					}
				}
			}

			/* 自分の位置が見つからない場合は行動しない */
			if (MyPosition == -1) { return; }

			/* 防御行動を設定する */
			std::shared_ptr<Action_Effect_Defence> defenceEffect = std::make_shared<Action_Effect_Defence>();
			defenceEffect->Target_Camp		= Character_Base::CAMP_ENEMY;	// 効果対象の陣営:敵
			defenceEffect->Target_Position	= MyPosition;					// 効果対象の立ち位置:自分自身
			defenceEffect->ShieldAmount		= 20;							// シールド量
			defenceEffect->EffectUser		= shared_from_this();			// 効果の使用者:自分自身
			defenceEffect->Priority			= 9999;							// 優先順位：最遅
			defenceEffect->EffectCard		= nullptr;						// 効果を使用するカード:無し
			this->pDataList_Battle->AddEffect(defenceEffect);
			this->ActionEffectList.push_back(defenceEffect);
		}
		break;

		/* 攻撃をおこなう */
		case 1:
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
						addEffect->DamageAmount		= 20;							// ダメージ量
						addEffect->EffectUser		= shared_from_this();			// 効果の使用者:自分自身
						addEffect->Priority			= 0;							// 優先順位：最遅
						addEffect->EffectCard		= nullptr;						// 効果を使用するカード:無し
						this->pDataList_Battle->AddEffect(addEffect);
						this->ActionEffectList.push_back(addEffect);
						break;
					}
				}
			}
		}
		break;
	}
}
