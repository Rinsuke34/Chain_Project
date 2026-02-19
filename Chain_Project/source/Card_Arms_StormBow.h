/* "嵐の弓"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "嵐の弓"カードクラス
class Card_Arms_StormBow : public Card_Arms_Base
{
	public:
		Card_Arms_StormBow();				// コンストラクタ
		virtual ~Card_Arms_StormBow() {};	// デストラクタ

		virtual void	BattleAction()	override;	// 戦闘行動
};
