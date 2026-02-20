/* "王国軍の剣"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "王国軍の剣"カードクラス
class Card_Arms_RoyalSword : public Card_Arms_Base
{
	public:
		Card_Arms_RoyalSword();				// コンストラクタ
		virtual ~Card_Arms_RoyalSword() {};	// デストラクタ

		/* 関数 */
		virtual void Effect_Action_Before(std::shared_ptr<Character_Base> Target) override;		// 効果発動:行動直前
};
