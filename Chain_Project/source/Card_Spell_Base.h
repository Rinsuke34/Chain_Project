/* "魔法"カードのベースクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Base.h"

// "魔法"カードのベースクラス
class Card_Spell_Base : public Card_Base
{
	public:
		Card_Spell_Base();				// コンストラクタ
		virtual ~Card_Spell_Base() {};	// デストラクタ

		/* 関数 */


	protected:
		/* 変数 */
		/* 関数 */
};
