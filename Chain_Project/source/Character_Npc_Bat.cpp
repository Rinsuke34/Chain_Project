/* キャラクター(コウモリ)の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Npc_Bat.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Card_Base.h"
#include "Buff_Debuff.h"

// コンストラクタ
Character_Npc_Bat::Character_Npc_Bat() : Character_Base()
{
	/* 初期化 */
	this->iHP_Max	= 20;			// 体力(最大値)
	this->iHP_Now	= 20;			// 体力(現在値)
	this->Camp		= Character_Base::CAMP_ENEMY;	// 陣営:敵陣営
	this->SizeX		= 150;			// キャラクターの幅
	this->SizeY		= 150;			// キャラクターの高さ
	this->DropCoin	= 5;			// ドロップするコインの枚数
	SetUpImage("Character_Ilust/Npc/Bat");

	/* ドロップするカードの設定 */
	DropItemSet(Card_Base::RARITY_COMMON);
	DropItemSet(Card_Base::RARITY_COMMON);
}

// 行動
void Character_Npc_Bat::Action()
{
	/* ランダムに行動を行う */
	int RandAction = GetRand(1);
	switch (RandAction)
	{
		/* 全体に毒付与 */
		case 0:
		{
			/* 特殊行動を設定する */
			std::shared_ptr<Action_Effect_Extra> addEffect = std::make_shared<Action_Effect_Extra>();
			addEffect->EffectUser		= shared_from_this();	// 効果の使用者:自分自身
			addEffect->Priority			= 0;					// 優先順位：最遅
			addEffect->ExplanationText	= "どくこうげき";		// 説明文
			this->pDataList_Battle->AddEffect(addEffect);
			this->ActionEffectList.push_back(addEffect);
		}
		break;

		/* 攻撃 */
		case 1:
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
		break;
	}
}

// 特殊行動(エネミーの特殊行動はこの関数を継承して行う)
void Character_Npc_Bat::Action_Extra()
{
	/* 毒デバフを敵全員にかける */
	// 前衛から順に敵が存在するか確認
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		std::shared_ptr<Character_Base> Enemy = this->pDataList_Battle->GetFriendCharacter(i);
		if (Enemy != nullptr)
		{
			// 敵が存在するなら
			/* 攻撃対象が"状態異常：毒"を所持しているか確認 */
			std::shared_ptr<Character_Buff_Debuff_Base> Poison = Enemy->CheckGet_Buff_Debuff("Debuff_Poison");
			if (Poison != nullptr)
			{
				// 所持しているなら
				/* 残りターン数を+5する */
				Poison->Buff_Debuff_Time += 5;	// 残りターン数 + 5
			}
			else
			{
				// 所持していないなら
				/* "状態異常：毒"を付与する */
				std::shared_ptr<Character_Buff_Debuff_Base> Poison = std::make_shared<Character_Debuff_Poison>();
				Poison->Buff_Debuff_Time = 5;	// 残りターン数 =  5
				Enemy->Add_Buff_Debuff(Poison);
			}
		}
	}
}
