/* "道具"カードのベースクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Base.h"

// "道具"カードのベースクラス
class Card_Item_Base : public Card_Base
{
	public:
		Card_Item_Base();				// コンストラクタ
		virtual ~Card_Item_Base() {};	// デストラクタ

		/* 関数 */

	protected:
		/* 関数 */
};
