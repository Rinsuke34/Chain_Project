/* "骨のナイフ"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "骨のナイフ"カードクラス
class Card_Arms_BoneDagger : public Card_Arms_Base
{
	public:
		Card_Arms_BoneDagger();				// コンストラクタ
		virtual ~Card_Arms_BoneDagger() {};	// デストラクタ

		virtual void	Effect_StartTurn() override;	// 効果発動:ターン開始時
};
