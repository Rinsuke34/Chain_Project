/* "守護のポーション"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Item_Base.h"

// "守護のポーション"カードクラス
class Card_Item_GuardianPotion : public Card_Item_Base
{
	public:
		Card_Item_GuardianPotion();				// コンストラクタ
		virtual ~Card_Item_GuardianPotion() {};	// デストラクタ

		/* 関数 */
		virtual void Card_Effect_Extra_Process() override;		// カード効果(特殊効果)
};
