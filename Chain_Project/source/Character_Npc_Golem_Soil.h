/* キャラクター(ゴーレム(土))の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// 前方宣言
class DataList_Battle;

// キャラクター(ゴーレム(土))のクラス
class Character_Npc_Golem_Soil : public Character_Base
{
	public:
		Character_Npc_Golem_Soil();				// コンストラクタ
		virtual ~Character_Npc_Golem_Soil() {};	// デストラクタ

		/* 関数 */
		virtual void Action()		override;	// 行動
};
