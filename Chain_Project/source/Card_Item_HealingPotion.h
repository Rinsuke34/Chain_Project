/* "回復のポーション"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Item_Base.h"

// "回復のポーション"カードクラス
class Card_Item_HealingPotion : public Card_Item_Base
{
	public:
		Card_Item_HealingPotion();				// コンストラクタ
		virtual ~Card_Item_HealingPotion() {};	// デストラクタ

		/* 関数 */
		virtual void Effect_StartAction() override;	// 効果発動:行動開始時
};
