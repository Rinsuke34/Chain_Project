/* キャラクター(ロボット(セキュリティ))の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// 前方宣言
class DataList_Battle;

// キャラクター(ロボット(セキュリティ))のクラス
class Character_Npc_Robot_Security : public Character_Base
{
	public:
		Character_Npc_Robot_Security();				// コンストラクタ
		virtual ~Character_Npc_Robot_Security() {};	// デストラクタ

		/* 関数 */
		virtual void Action()		override;	// 行動
};
