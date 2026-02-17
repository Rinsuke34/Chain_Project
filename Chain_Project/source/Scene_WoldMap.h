/* シーン"ワールドマップ画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class WoldMap_Node_Base;
class Card_NextArea;
class DataList_GameResource;

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
		static const int 	WOLDMAP_IMAGE_WIDTH			= 960;	// ワールドマップ画像の幅
		static const int 	WOLDMAP_IMAGE_HEIGHT		= 1080;	// ワールドマップ画像の高さ
		// 描写座標
		static const int 	WOLDMAP_DRAW_POS_X			= 960;	// ワールドマップ描写X座標
		static const int 	WOLDMAP_DRAW_POS_Y			= 0;	// ワールドマップ描写Y座標
		static const int	WOLDMAP_DRAW_START_POS_X	= 1920;	// ワールドマップ描写X初期座標
		static const int	WOLDMAP_DRAW_POS_MOVE_SPEED	= 20;	// ワールドマップ描写X座標移動速度
		static const int	WOLDMAP_NODE_INTERVAL_X		= 140;	// ワールドマップノードX間隔
		static const int	WOLDMAP_NODE_INTERVAL_Y		= 200;	// ワールドマップノードY間隔
		static const int	WOLDMAP_FRAME_THICKNESS		= 32;	// フレームの太さ
		// 座標関係
		static const int	NEXTAREA_CARD_INTERVAL		= 200;	// 移動先エリアのカード間隔
		static const int	NEXTAREA_CARD_CENTER_X		= 480;	// 移動先エリアのカード描写位置の中心X座標
		static const int	NEXTAREA_CARD_POS_Y			= 735;	// 移動先エリアのカード描写位置のY座標

	protected:
		/* 変数 */
		// カードリスト
		std::vector<std::shared_ptr<Card_NextArea>>		NextAreaCardList;	// 移動先エリアカードリスト
		// ワールドマップノード
		std::vector<std::shared_ptr<WoldMap_Node_Base>>	WoldMapNodeList;	// ワールドマップノードリスト
		// 画像
		int Image_WoldMap;					// ワールドマップの画像
		// その他
		Struct_2D::POSITION						WoldMapDrawPos;			// ワールドマップの描写座標
		std::shared_ptr<WoldMap_Node_Base>		NowNode;				// 現在地点のノード
		std::shared_ptr<DataList_GameResource>	pDataList_GameResource;	// ゲームリソース管理用データリスト
		bool									GameOverCreateFlg;		// ゲームオーバーシーン作成フラグ(作成済みならTrue)

		/* 関数 */
		void CheckNowNode();					// 現在のノードを確認
		void NextAreaCard_Create();				// 移動先エリアカードの作成
		void Node_Update();						// 各ノードの更新処理
		void Node_Draw();						// 各ノードの描画処理
		void Node_SetCenterPosition();			// 各ノードの中心座標を設定
		void Road_Draw();						// 各ノードからつながる道を描写
		void Update_Image();					// 画像の更新
		void Update_DrawPos();					// 描写座標の更新
		void WoldMap_Draw();					// ワールドマップの描写
		void NextAreaCard_Update();				// 移動先エリアカードの更新
		void NextAreaCard_Draw();				// 移動先エリアカードの描画
		void BackGround_Draw(bool FrameOnly);	// 背景描写
		void Select_Card();						// カード選択
		void Load_MapData();					// マップデータの読み込み
		void Node_SetResource();				// ゲームリソース管理に現在のノード情報を設定
		void CheckStageEnd();					// ステージクリアの確認
};
