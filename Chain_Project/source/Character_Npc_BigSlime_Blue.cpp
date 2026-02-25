/* キャラクター(スライム(巨大/緑))の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Npc_BigSlime_Green.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Card_Include.h"
#include "Character_Npc_Slime_Blue.h"
#include "Character_Npc_Slime_Green.h"
#include "Character_Npc_Slime_Red.h"

// コンストラクタ
Character_Boss_BigSlime_Green::Character_Boss_BigSlime_Green() : Character_Base()
{
	/* 初期化 */
	this->iHP_Max				= 50;							// 体力(最大値)
	this->iHP_Now				= 50;							// 体力(現在値)
	this->Camp					= Character_Base::CAMP_ENEMY;	// 陣営:敵陣営
	this->SizeX					= 300;							// キャラクターの幅
	this->SizeY					= 300;							// キャラクターの高さ
	this->DropCoin				= 30;							// ドロップするコインの枚数
	this->ActionCount			= 0;							// 行動カウント(行動内容の決定に使用)
	this->HPBarPosCorrectionY	= 130;							// HPバーの描写座標の補正値
	SetUpImage("Character_Ilust/Npc/Slime_Green");

	/* ドロップするカードの設定 */
	DropItemSet(Card_Base::RARITY_COMMON);
	DropItemSet(Card_Base::RARITY_RARE);
	DropItemSet(Card_Base::RARITY_EPIC);
}

// 行動
void Character_Boss_BigSlime_Green::Action()
{
	switch (this->ActionCount)
	{
		/* 前衛と中衛にスライムを召喚 */
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
void Character_Boss_BigSlime_Green::Action_Extra()
{

	/* 前衛にキャラクターが存在するか確認 */
	if (this->pDataList_Battle->GetEnemyCharacter(DataList_Battle::POSITION_FRONT) == nullptr)
	{
		// 存在しない場合
		std::shared_ptr<Character_Base> pSummonCharacter = nullptr;
		switch (GetRand(2))
		{
			case 0:
				/* 前衛に青スライムを召喚 */
				pSummonCharacter = std::make_shared<Character_Npc_Slime_Blue>();
				break;

			case 1:
				/* 前衛に緑スライムを召喚 */
				pSummonCharacter = std::make_shared<Character_Npc_Slime_Green>();
				break;

			case 2:
				/* 前衛に赤スライムを召喚 */
				pSummonCharacter = std::make_shared<Character_Npc_Slime_Red>();
				break;
		}

		// ドロップアイテムを無しに設定
		pSummonCharacter->SetDropCoin(0);
		pSummonCharacter->SetDropCardList(std::vector<std::shared_ptr<Card_Base>>());
		this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_FRONT, pSummonCharacter);
	}

	/* 中衛にキャラクターが存在するか確認 */
	if (this->pDataList_Battle->GetEnemyCharacter(DataList_Battle::POSITION_MIDDLE) == nullptr)
	{
		// 存在しない場合
		std::shared_ptr<Character_Base> pSummonCharacter = nullptr;
		switch (GetRand(2))
		{
			case 0:
				/* 前衛に青スライムを召喚 */
				pSummonCharacter = std::make_shared<Character_Npc_Slime_Blue>();
				break;

			case 1:
				/* 前衛に緑スライムを召喚 */
				pSummonCharacter = std::make_shared<Character_Npc_Slime_Green>();
				break;

			case 2:
				/* 前衛に赤スライムを召喚 */
				pSummonCharacter = std::make_shared<Character_Npc_Slime_Red>();
				break;
		}

		// ドロップアイテムを無しに設定
		pSummonCharacter->SetDropCoin(0);
		pSummonCharacter->SetDropCardList(std::vector<std::shared_ptr<Card_Base>>());
		this->pDataList_Battle->SetEnemyCharacter(DataList_Battle::POSITION_MIDDLE, pSummonCharacter);
	}
}
