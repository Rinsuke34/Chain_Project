/* キャラクター(コウモリ)の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Npc_Bat.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Card_Base.h"

// コンストラクタ
Character_Bat::Character_Bat() : Character_Base()
{
	/* 初期化 */
	this->iHP_Max	= 10;			// 体力(最大値)
	this->iHP_Now	= 10;			// 体力(現在値)
	this->Camp		= Character_Base::CAMP_ENEMY;	// 陣営:敵陣営
	this->SizeX		= 150;			// キャラクターの幅
	this->SizeY		= 150;			// キャラクターの高さ
	this->DropCoin	= 5;			// ドロップするコインの枚数
	SetUpImage("Character_Ilust/Npc/Bat");

	/* ドロップするカードの設定 */
	DropItemSet(Card_Base::RARITY_COMMON);
}

// 行動
void Character_Bat::Action()
{
	/* プレイヤー側の最も前のキャラクターに対して攻撃を作成する */
	if (this->pDataList_Battle != nullptr)
	{
		// 最も前の仲間キャラクターを取得
		std::shared_ptr<Character_Base> pTargetCharacter = nullptr;
		for (int i = DataList_Battle::BATTLE_AREA_1; i < DataList_Battle::BATTLE_AREA_MAX; i++)
		{
			pTargetCharacter = this->pDataList_Battle->GetFriendCharacter(i);
			if (pTargetCharacter != nullptr)
			{
				// 攻撃行動を設定する
				std::shared_ptr<Action_Effect_Attack> addEffect = std::make_shared<Action_Effect_Attack>();
				addEffect->Target_Camp		= Character_Base::CAMP_FRIEND;	// 効果対象の陣営:味方
				addEffect->Target_Position	= i;							// 効果対象の立ち位置:確認した敵キャラクターの位置
				addEffect->DamageAmount		= 7;							// ダメージ量
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
