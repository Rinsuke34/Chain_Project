/* "妖刀"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "妖刀"カードクラス
class Card_Arms_CursedKatana : public Card_Arms_Base
{
	public:
		Card_Arms_CursedKatana();				// コンストラクタ
		virtual ~Card_Arms_CursedKatana() {};	// デストラクタ

		/* 関数 */
		virtual void Effect_Action_After(std::shared_ptr<Character_Base> Target) override;		// 効果発動:行動時
};
