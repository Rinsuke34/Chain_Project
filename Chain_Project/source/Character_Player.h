/* キャラクター(プレイヤー)の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// キャラクター(プレイヤー)のクラス
class Character_Player : public Character_Base
{
	public:
		Character_Player();				// コンストラクタ
		virtual ~Character_Player() {};	// デストラクタ
};
