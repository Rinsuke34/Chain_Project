/* キャラクター(使い魔)の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// 前方宣言
class DataList_Battle;

// キャラクター(使い魔)のクラス
class Character_Npc_Servant : public Character_Base
{
	public:
		Character_Npc_Servant();				// コンストラクタ
		virtual ~Character_Npc_Servant() {};	// デストラクタ

		/* 関数 */
		virtual void Action()		override;	// 行動
};
