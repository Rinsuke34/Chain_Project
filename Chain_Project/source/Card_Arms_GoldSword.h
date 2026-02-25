/* "金の剣"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "金の剣"カードクラス
class Card_Arms_GoldSword : public Card_Arms_Base
{
	public:
		Card_Arms_GoldSword();				// コンストラクタ
		virtual ~Card_Arms_GoldSword() {};	// デストラクタ

		virtual void	Effect_StartTurn() override;	// 効果発動:ターン開始時
};
