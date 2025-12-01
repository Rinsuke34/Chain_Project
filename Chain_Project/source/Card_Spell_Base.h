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

	protected:
		/* 関数 */
		virtual void	DrawBackGround()	override;	// 背景を描写
		virtual void	DrawImage()			override;	// イラストを描写
		virtual void	DrawSuite()			override;	// スートを描写
		virtual void	DrawFrame()			override;	// フレームを描写
		virtual void	DrawName()			override;	// 名前を描写
};
