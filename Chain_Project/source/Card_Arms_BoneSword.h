/* "骨の剣"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "骨の剣"カードクラス
class Card_Arms_BoneSword : public Card_Arms_Base
{
	public:
		Card_Arms_BoneSword();				// コンストラクタ
		virtual ~Card_Arms_BoneSword() {};	// デストラクタ

		virtual void	Effect_StartTurn() override;	// 効果発動:ターン開始時
};
