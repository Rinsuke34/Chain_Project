/* "クラス"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Base.h"

// "クラス"カードのベースクラス
class Card_Class : public Card_Base
{
	public:
		Card_Class(int ClassNo);	// コンストラクタ
		virtual ~Card_Class() {};	// デストラクタ

		/* ゲッター */
		int GetClassNo() { return this->ClassNo; }	// クラスの番号を取得

		/* 定数 */
		// 描写関係
		static const int SPELL_CARD_IMAGE_WIDE		= 110;	// カード画像の幅
		static const int SPELL_CARD_IMAGE_HEIGHT	= 110;	// カード画像の高さ

	protected:
		/* 関数 */
		virtual void	DrawBackGround()	override;	// 背景を描写
		virtual void	DrawImage()			override;	// イラストを描写
		virtual void	DrawFrame()			override;	// フレームを描写
		virtual void	DrawName()			override;	// 名前を描写

		/* 変数 */
		int	ClassNo;			// クラスの番号
};
