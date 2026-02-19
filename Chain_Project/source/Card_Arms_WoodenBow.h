/* "木の弓"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "木の弓"カードクラス
class Card_Arms_WoodenBow : public Card_Arms_Base
{
	public:
		Card_Arms_WoodenBow();				// コンストラクタ
		virtual ~Card_Arms_WoodenBow() {};	// デストラクタ
};
