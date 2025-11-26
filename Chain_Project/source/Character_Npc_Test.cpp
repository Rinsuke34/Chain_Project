/* キャラクター(テストNPC)の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Npc_Test.h"
// 関連クラス
#include "DataList_Battle.h"

// コンストラクタ
Character_Npc_Test::Character_Npc_Test()
{
	/* 初期化 */
	this->iHP_Max = 100;			// 体力(最大値)
	this->iHP_Now = 100;			// 体力(現在値)
	SetUpImage("Character_Ilust/Monster/Test");

	/* データリスト取得 */
	// バトル用データリスト
	this->pDataList_Battle = std::dynamic_pointer_cast<DataList_Battle>(gpDataListServer->GetDataList("DataList_Battle"));
}

// 行動
void Character_Npc_Test::Action()
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
				std::shared_ptr<Card_Effect_Attack> addEffect	= std::make_shared<Card_Effect_Attack>();
				addEffect->Target_Camp		= Character_Base::CAMP_FRIEND;	// 効果対象の陣営:味方
				addEffect->Target_Position	= i;							// 効果対象の立ち位置:確認した敵キャラクターの位置
				addEffect->DamageAmount			= 5;							// ダメージ量
				this->pDataList_Battle->AddEffect(addEffect, DataList_Battle::BATTLE_AREA_1);
				break;
			}
		}
	}
}
