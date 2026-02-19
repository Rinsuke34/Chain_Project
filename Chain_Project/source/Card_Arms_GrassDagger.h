/* "硝子の短剣"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "硝子の短剣"カードクラス
class Card_Arms_GrassDagger : public Card_Arms_Base
{
	public:
		Card_Arms_GrassDagger();				// コンストラクタ
		virtual ~Card_Arms_GrassDagger() {};	// デストラクタ
};
