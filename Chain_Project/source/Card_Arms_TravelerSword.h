/* "旅人の剣"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "旅人の剣"カードクラス
class Card_Arms_TravelerSword : public Card_Arms_Base
{
	public:
		Card_Arms_TravelerSword();				// コンストラクタ
		virtual ~Card_Arms_TravelerSword() {};	// デストラクタ
};
