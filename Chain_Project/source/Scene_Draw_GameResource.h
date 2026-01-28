/* シーン"リソース描写"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"
// 関連クラス
#include "Action_Effect.h"

/* 前方宣言 */
class DataList_GameResource;

// シーン"リソース描写"
class Scene_Draw_GameResource : public Scene_Base
{
	public:
		Scene_Draw_GameResource();					// コンストラクタ
		virtual ~Scene_Draw_GameResource() {};		// デストラクタ

		/* 関数 */
		virtual void	Update()	override;		// 更新
		virtual void	Draw()		override;		// 描画

		/* 定数 */
		static const int COIN_POS_X_PLUS	= 46;		// コイン表示のX位置(画面左上からのプラス側の値)
		static const int COIN_POS_Y_PLUS	= 48;		// コイン表示のY位置(画面左上からのプラス側の値)
		static const int COIN_POS_X_MINUS	= 14;		// コイン表示のX位置(画面左上からのマイナス側の値)
		static const int COIN_POS_Y_MINUS	= 14;		// コイン表示のX位置(画面左上からのマイナス側の値)
		static const int CARD_POS_X_PLUS	= 46 + 148;	// カード表示のX位置(画面左上からのプラス側の値)
		static const int CARD_POS_Y_PLUS	= 48;		// カード表示のY位置(画面左上からのプラス側の値)
		static const int CARD_POS_X_MINUS	= 14 + 148;	// カード表示のX位置(画面左上からのマイナス側の値)
		static const int CARD_POS_Y_MINUS	= 14;		// カード表示のX位置(画面左上からのマイナス側の値)

	private:
		/* 変数 */
		// データリスト
		std::shared_ptr<DataList_GameResource>	pDataList_GameResource;	// ゲームリソース管理用データリスト
		// 画像
		std::shared_ptr<int>	Image_Coin;				// コイン
		std::shared_ptr<int>	Image_Frame_Corner;		// 角
		std::shared_ptr<int>	Image_Frame_Line;		// 線
		std::shared_ptr<int>	Image_Frame_Inside;		// 内側
		std::shared_ptr<int>	Image_Card;				// カード
		// その他
		int CoinScaleUp;		// コインアイコンのスケールアップ量
		int Coin_Old;			// 変更前のコイン枚数
		int CardScaleUp;		// カードアイコンのスケールアップ量
		int Card_Old;			// 変更前のカード枚数
};
