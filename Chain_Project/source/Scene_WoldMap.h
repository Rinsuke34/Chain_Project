/* シーン"ワールドマップ画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class WoldMap_Node_Base;

// シーン"ワールドマップ"
class Scene_WoldMap : public Scene_Base
{
	public:
		Scene_WoldMap();			// コンストラクタ
		virtual ~Scene_WoldMap();	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

		/* 定数 */
		// 画像サイズ
		static const int 	WOLDMAP_IMAGE_WIDTH			= 990;	// ワールドマップ画像の幅
		static const int 	WOLDMAP_IMAGE_HEIGHT		= 1080;	// ワールドマップ画像の高さ
		// 描写座標
		static const int 	WOLDMAP_DRAW_POS_X			= 990;	// ワールドマップ描写X座標
		static const int 	WOLDMAP_DRAW_POS_Y			= 0;	// ワールドマップ描写Y座標
		static const int	WOLDMAP_DRAW_START_POS_X	= 1980;	// ワールドマップ描写X初期座標
		static const int	WOLDMAP_DRAW_POS_MOVE_SPEED	= 20;	// ワールドマップ描写X座標移動速度
		static const int	WOLDMAP_NODE_INTERVAL_X		= 140;	// ワールドマップノードX間隔
		static const int	WOLDMAP_NODE_INTERVAL_Y		= 200;	// ワールドマップノードY間隔

	protected:
		/* 変数 */
		// ワールドマップノード
		std::vector<std::shared_ptr<WoldMap_Node_Base>>	WoldMapNodeList;	// ワールドマップノードリスト
		// 画像
		int Image_WoldMap;					// ワールドマップの画像
		// その他
		Struct_2D::POSITION	WoldMapDrawPos;	// ワールドマップの描写座標

		/* 関数 */
		void Node_Update();				// 各ノードの更新処理
		void Node_Draw();				// 各ノードの描画処理
		void Node_SetCenterPosition();	// 各ノードの中心座標を設定
		void Road_Draw();				// 各ノードからつながる道を描写
		void Update_Image();			// 画像の更新
		void Update_DrawPos();			// 描写座標の更新
};
