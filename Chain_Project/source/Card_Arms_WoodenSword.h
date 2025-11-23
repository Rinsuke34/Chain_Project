/* "木の剣"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "木の剣"カードクラス
class Card_Arms_WoodenSword : public Card_Arms_Base
{
	public:
		Card_Arms_WoodenSword();				// コンストラクタ
		virtual ~Card_Arms_WoodenSword() {};	// デストラクタ
};
