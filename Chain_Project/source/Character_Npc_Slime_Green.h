/* キャラクター(スライム(緑))の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// 前方宣言
class DataList_Battle;

// キャラクター(スライム(緑))のクラス
class Character_Npc_Slime_Green : public Character_Base
{
	public:
		Character_Npc_Slime_Green();				// コンストラクタ
		virtual ~Character_Npc_Slime_Green() {};	// デストラクタ

		/* 関数 */
		virtual void Action()		override;	// 行動
};
