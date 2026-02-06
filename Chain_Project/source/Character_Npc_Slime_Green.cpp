/* キャラクター(スライム(緑))の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Npc_Slime_Green.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Card_Include.h"

// コンストラクタ
Character_Slime_Green::Character_Slime_Green() : Character_Base()
{
	/* 初期化 */
	this->iHP_Max	= 15;			// 体力(最大値)
	this->iHP_Now	= 15;			// 体力(現在値)
	this->Camp		= Character_Base::CAMP_ENEMY;	// 陣営:敵陣営
	this->SizeX		= 150;			// キャラクターの幅
	this->SizeY		= 150;			// キャラクターの高さ
	this->DropCoin	= 2;			// ドロップするコインの枚数
	SetUpImage("Character_Ilust/Npc/Slime_Green");

	/* ドロップするカード一覧の設定 */
	std::vector<std::shared_ptr<Card_Base>> dropCardList;
	if ((rand() % 100) < 80) { dropCardList.push_back(std::make_shared<Card_Arms_TravelerShield>()); }	// 80%の確率で"木の剣"をドロップ
	SetDropCardList(dropCardList);
}

// 行動
void Character_Slime_Green::Action()
{
	/* プレイヤー側の最も前のキャラクターに対して攻撃を作成する */
	if (this->pDataList_Battle != nullptr)
	{
		// 最も前の仲間キャラクターを取得
		std::shared_ptr<Character_Base> pTargetCharacter = nullptr;
		for (int i = DataList_Battle::POSITION_FRONT; i < DataList_Battle::POSITION_MAX; i++)
		{
			pTargetCharacter = this->pDataList_Battle->GetFriendCharacter(i);
			if (pTargetCharacter != nullptr)
			{
				// ランダムなバトルエリアを対象とする
				int BattleAreaNo = GetRand(DataList_Battle::BATTLE_AREA_MAX - 1);

				// 攻撃行動を設定する
				std::shared_ptr<Action_Effect_Attack> addEffect = std::make_shared<Action_Effect_Attack>();
				addEffect->Target_Camp		= Character_Base::CAMP_FRIEND;	// 効果対象の陣営:味方
				addEffect->Target_Position	= i;							// 効果対象の立ち位置:確認した敵キャラクターの位置
				addEffect->DamageAmount		= 5;							// ダメージ量
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
