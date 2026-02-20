/* "シールドバッシュ"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Spell_Base.h"

// "シールドバッシュ"カードクラス
class Card_Spell_ShieldBash : public Card_Spell_Base
{
	public:
		Card_Spell_ShieldBash();				// コンストラクタ
		virtual ~Card_Spell_ShieldBash() {};	// デストラクタ

		/* 関数 */
		virtual void Card_Effect_Extra_Process() override;		// カード効果(特殊効果)
};
