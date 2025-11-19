/* キャラクター(テストNPC)の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// キャラクター(テストNPC)のクラス
class Character_Npc_Test : public Character_Base
{
	public:
		Character_Npc_Test();				// コンストラクタ
		virtual ~Character_Npc_Test() {};	// デストラクタ

		/* 関数 */
//		virtual void Draw_HPBar() override;	// 体力バー描画
};
