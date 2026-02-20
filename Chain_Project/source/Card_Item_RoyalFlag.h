/* "王国軍の軍旗"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Item_Base.h"

// "王国軍の軍旗"カードクラス
class Card_Item_RoyalFlag : public Card_Item_Base
{
	public:
		Card_Item_RoyalFlag();				// コンストラクタ
		virtual ~Card_Item_RoyalFlag() {};	// デストラクタ

		/* 関数 */
		virtual void BattleAction() override;		// 戦闘行動
};
