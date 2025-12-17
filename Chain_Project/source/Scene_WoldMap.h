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

	protected:
		/* 変数 */
		// ワールドマップノード
		std::vector<std::shared_ptr<WoldMap_Node_Base>>	WoldMapNodeList;	// ワールドマップノードリスト
		// 画像
		std::shared_ptr<int> Image_BackGround;	// ワールドマップ背景背景

		/* 関数 */
		void Node_Update();				// 各ノードの更新処理
		void Node_Draw();				// 各ノードの描画処理
		void Node_SetCenterPosition();	// 各ノードの中心座標を設定
		void Road_Draw();				// 各ノードからつながる道を描写
};
