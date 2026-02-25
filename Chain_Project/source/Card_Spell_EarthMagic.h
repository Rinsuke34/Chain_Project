/* "大地の魔法"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Spell_Base.h"

// "大地の魔法"カードクラス
class Card_Spell_EarthMagic : public Card_Spell_Base
{
	public:
		Card_Spell_EarthMagic();				// コンストラクタ
		virtual ~Card_Spell_EarthMagic() {};	// デストラクタ

		/* 関数 */
		virtual void Card_Effect_Extra_Process() override;		// カード効果(特殊効果)
};
