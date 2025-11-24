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

		/* 定数 */
		static const int PLAYER_WIDE	= 150;	// プレイヤーの幅
		static const int PLAYER_HEIGHT	= 150;	// プレイヤーの高さ
};
