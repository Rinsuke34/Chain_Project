/* キャラクター(スライム(巨大/緑))の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Npc_BigSlime_Green.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Card_Include.h"
#include "Character_Npc_Slime_Blue.h"
#include "Character_Npc_Slime_Green.h"

// コンストラクタ
Character_BigSlime_Green::Character_BigSlime_Green() : Character_Base()
{
	/* 初期化 */
	this->iHP_Max				= 150;							// 体力(最大値)
	this->iHP_Now				= 150;							// 体力(現在値)
	this->Camp					= Character_Base::CAMP_ENEMY;	// 陣営:敵陣営
	this->SizeX					= 300;							// キャラクターの幅
	this->SizeY					= 300;							// キャラクターの高さ
	this->DropCoin				= 30;							// ドロップするコインの枚数
	this->ActionCount			= 0;							// 行動カウント(行動内容の決定に使用)
	this->HPBarPosCorrectionY	= 130;							// HPバーの描写座標の補正値
	SetUpImage("Character_Ilust/Npc/Slime_Green");

	/* ドロップするカード一覧の設定 */
	std::vector<std::shared_ptr<Card_Base>> dropCardList;
	dropCardList.push_back(std::make_shared<Card_Item_PoisonPotion>());	// 100%の確率で"回復薬"をドロップ
	SetDropCardList(dropCardList);
}

// 行動
void Character_BigSlime_Green::Action()
{
	switch (this->ActionCount)
	{
		/* 前衛に青スライム、中衛に緑スライムを召喚 */
		case 0:
			{
				/* 特殊行動を設定する */
				std::shared_ptr<Action_Effect_Extra> addEffect = std::make_shared<Action_Effect_Extra>();
				addEffect->EffectUser		= shared_from_this();	// 効果の使用者:自分自身
				addEffect->Priority			= 0;					// 優先順位：最遅
				addEffect->ExplanationText	= "ぶんれつ";			// 説明文
				this->pDataList_Battle->AddEffect(addEffect);
				this->ActionEffectList.push_back(addEffect);
			}
			break;

		/* 全体回復 */
		case 1:
			{
				/* 回復行動を設定する */
				std::shared_ptr<Action_Effect_Heal> addEffect = std::make_shared<Action_Effect_Heal>();
				addEffect->Target_Camp	= Character_Base::CAMP_ENEMY;	// 効果対象の陣営:敵
				addEffect->AllRange		= true;							// 全体回復
				addEffect->HealAmount	= 15;							// 回復量
				addEffect->EffectUser	= shared_from_this();			// 効果の使用者:自分自身
				addEffect->Priority		= 0;							// 優先順位：最遅
				addEffect->EffectCard	= nullptr;						// 効果を使用するカード:無し
				this->pDataList_Battle->AddEffect(addEffect);
				this->ActionEffectList.push_back(addEffect);
			}
			break;

		/* 攻撃 */
		case 2:
			{
				// 最も前の仲間キャラクターを取得
				std::shared_ptr<Character_Base> pTargetCharacter = nullptr;
				for (int i = DataList_Battle::POSITION_FRONT; i < DataList_Battle::POSITION_MAX; i++)
				{
					pTargetCharacter = this->pDataList_Battle->GetFriendCharacter(i);
					if (pTargetCharacter != nullptr)
					{
						// 攻撃行動を設定する
						std::shared_ptr<Action_Effect_Attack> addEffect = std::make_shared<Action_Effect_Attack>();
						addEffect->Target_Camp		= Character_Base::CAMP_FRIEND;	// 効果対象の陣営:味方
						addEffect->Target_Position	= i;							// 効果対象の立ち位置:確認した敵キャラクターの位置
						addEffect->DamageAmount		= 10;							// ダメージ量
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
	
	/* 行動カウントを更新 */
	this->ActionCount++;
	if (this->ActionCount >= 3)
	{
		this->ActionCount = 0;
	}
}

// 特殊行動(エネミーの特殊行動はこの関数を継承して行う)
void Character_BigSlime_Green::Action_Extra()
{
	/* 前衛にキャラクターが存在するか確認 */
	if (this->pDataList_Battle->GetEnemyCharacter(DataList_Battle::POSITION_FRONT) == nullptr)
	{
		// 存在しない場合
		/* 前衛に青スライムを召喚 */
		std::shared_ptr<Character_Slime_Blue> pBlueSlime = std::make_shared<Character_Slime_Blue>();
		pBlueSlime->SetDropCoin(0);
		this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT, pBlueSlime);
	}

	/* 中衛にキャラクターが存在するか確認 */
	if (this->pDataList_Battle->GetEnemyCharacter(DataList_Battle::POSITION_MIDDLE) == nullptr)
	{
		// 存在しない場合
		/* 中衛に緑スライムを召喚 */
		std::shared_ptr<Character_Slime_Green> pGreenSlime = std::make_shared<Character_Slime_Green>();
		pGreenSlime->SetDropCoin(0);
		this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE, pGreenSlime);
	}
}
