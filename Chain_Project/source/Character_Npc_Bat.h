/* キャラクター(コウモリ)の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// 前方宣言
class DataList_Battle;

// キャラクター(コウモリ)のクラス
class Character_Bat : public Character_Base
{
	public:
		Character_Bat();				// コンストラクタ
		virtual ~Character_Bat() {};	// デストラクタ

		/* 関数 */
		virtual void Action() override;		// 行動
};
