/* "木の盾"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "木の盾"カードクラス
class Card_Arms_WoodenShield : public Card_Arms_Base
{
	public:
		Card_Arms_WoodenShield();				// コンストラクタ
		virtual ~Card_Arms_WoodenShield() {};	// デストラクタ
};
