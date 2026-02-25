/* キャラクター(スケルトン)の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// 前方宣言
class DataList_Battle;

// キャラクター(スケルトン)のクラス
class Character_Npc_Skeleton : public Character_Base
{
	public:
		Character_Npc_Skeleton();				// コンストラクタ
		virtual ~Character_Npc_Skeleton() {};	// デストラクタ

		/* 関数 */
		virtual void Action()		override;	// 行動
};
