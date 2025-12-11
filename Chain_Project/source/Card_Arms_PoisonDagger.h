/* "毒ナイフ"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "毒ナイフ"カードクラス
class Card_Arms_PoisonDagger : public Card_Arms_Base
{
	public:
		Card_Arms_PoisonDagger();				// コンストラクタ
		virtual ~Card_Arms_PoisonDagger() {};	// デストラクタ

		/* 関数 */
		virtual void Effect_Action_After(std::shared_ptr<Character_Base> Target) override;		// 効果発動:行動時
};
