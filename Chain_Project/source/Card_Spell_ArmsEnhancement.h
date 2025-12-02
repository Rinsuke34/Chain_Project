/* "武具強化"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Spell_Base.h"

// "武具強化"カードクラス
class Card_Spell_ArmsEnhancement : public Card_Spell_Base
{
	public:
		Card_Spell_ArmsEnhancement();				// コンストラクタ
		virtual ~Card_Spell_ArmsEnhancement() {};	// デストラクタ

		/* 関数 */
		virtual void BattleAction() override;		// 戦闘行動
};
