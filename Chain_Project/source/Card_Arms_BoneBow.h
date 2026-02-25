/* "骨の弓"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "骨の弓"カードクラス
class Card_Arms_BoneBow : public Card_Arms_Base
{
	public:
		Card_Arms_BoneBow();				// コンストラクタ
		virtual ~Card_Arms_BoneBow() {};	// デストラクタ

		virtual void	Effect_StartTurn() override;	// 効果発動:ターン開始時
};
