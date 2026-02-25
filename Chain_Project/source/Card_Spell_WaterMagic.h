/* "水の魔法"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Spell_Base.h"

// "水の魔法"カードクラス
class Card_Spell_WaterMagic : public Card_Spell_Base
{
	public:
		Card_Spell_WaterMagic();				// コンストラクタ
		virtual ~Card_Spell_WaterMagic() {};	// デストラクタ

		/* 関数 */
		virtual void Card_Effect_Extra_Process() override;		// カード効果(特殊効果)
};
