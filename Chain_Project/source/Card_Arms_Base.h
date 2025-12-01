/* "武具"カードのベースクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Base.h"

// "武具"カードのベースクラス
class Card_Arms_Base : public Card_Base
{
	public:
		Card_Arms_Base();				// コンストラクタ
		virtual ~Card_Arms_Base() {};	// デストラクタ

		/* 定数 */
		static const int ARMS_CARD_IMAGE_WIDE		= 110;	// カード画像の幅
		static const int ARMS_CARD_IMAGE_HEIGHT		= 110;	// カード画像の高さ
		static const int ARMS_ILLUSTRATION_POS_Y	= 10;	// イラストのY位置
		static const int ARMS_ATTACK_X_POS			= -30;	// 攻撃力表示のX位置(中心から見て)
		static const int ARMS_DIFFENSE_X_POS		= 45;	// 防御力表示のX位置(中心から見て)
		static const int ARMS_STATUS_Y_POS			= 206;	// ステータス表示のY位置

	protected:
		/* 関数 */
		virtual void	DrawBackGround()	override;	// 背景を描写
		virtual void	DrawImage()			override;	// イラストを描写
		virtual void	DrawSuite()			override;	// スートを描写
		virtual void	DrawFrame()			override;	// フレームを描写
		virtual void	DrawName()			override;	// 名前を描写
};
