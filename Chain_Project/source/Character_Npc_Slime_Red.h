/* キャラクター(スライム(赤))の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// 前方宣言
class DataList_Battle;

// キャラクター(スライム(赤))のクラス
class Character_Npc_Slime_Red : public Character_Base
{
	public:
		Character_Npc_Slime_Red();				// コンストラクタ
		virtual ~Character_Npc_Slime_Red() {};	// デストラクタ

		/* 関数 */
		virtual void Action()		override;	// 行動
		virtual void Action_Extra()	override;	// 特殊行動(エネミーの特殊行動はこの関数を継承して行う)
};
