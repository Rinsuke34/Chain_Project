/* "硝子の剣"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "硝子の剣"カードクラス
class Card_Arms_GrassSword : public Card_Arms_Base
{
	public:
		Card_Arms_GrassSword();				// コンストラクタ
		virtual ~Card_Arms_GrassSword() {};	// デストラクタ
};
