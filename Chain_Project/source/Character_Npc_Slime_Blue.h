/* キャラクター(スライム(青))の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// 前方宣言
class DataList_Battle;

// キャラクター(スライム(青))のクラス
class Character_Slime_Blue : public Character_Base
{
	public:
		Character_Slime_Blue();				// コンストラクタ
		virtual ~Character_Slime_Blue() {};	// デストラクタ

		/* 関数 */
		virtual void Action() override;		// 行動
};
