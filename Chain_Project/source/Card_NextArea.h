/* "移動先エリア"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Base.h"

// "移動先エリア"カードのベースクラス
class Card_NextArea : public Card_Base
{
	public:
		Card_NextArea(int NextAreaType);	// コンストラクタ
		virtual ~Card_NextArea() {};		// デストラクタ

		/* 定数 */
		// 種類
		static const int TYPE_ENEMY	= 0;	// 敵
		static const int TYPE_SHOP	= 1;	// ショップ
		static const int TYPE_BOSS	= 2;	// ボス
		// 描写関係
		static const int SPELL_CARD_IMAGE_WIDE		= 110;	// カード画像の幅
		static const int SPELL_CARD_IMAGE_HEIGHT	= 110;	// カード画像の高さ

	protected:
		/* 関数 */
		virtual void	DrawBackGround()	override;	// 背景を描写
		virtual void	DrawImage()			override;	// イラストを描写
		virtual void	DrawFrame()			override;	// フレームを描写
		virtual void	DrawName()			override;	// 名前を描写

	private:
		/* 変数 */
		int	NextAreaType;		// 移動先エリアの種類
};
