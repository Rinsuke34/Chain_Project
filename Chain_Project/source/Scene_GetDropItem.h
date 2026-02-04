/* シーン"ドロップアイテム取得"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class DataList_GameResource;
class Card_Base;
class Scene_UI_Button;

// シーン"ドロップアイテム取得"
class Scene_GetDropItem : public Scene_Base
{
	public:
		Scene_GetDropItem();				// コンストラクタ
		virtual ~Scene_GetDropItem() {};	// デストラクタ

		/* 関数 */
		virtual void	Update()	override;		// 更新
		virtual void	Draw()		override;		// 描画

		/* 定数 */
		static const int DROPITEM_DRAW_WIDTH		= 1920;					// ドロップアイテム取得画面描写横幅
		static const int DROPITEM_DRAW_HEIGHT		= 780;					// ドロップアイテム取得画面描写縦幅
		static const int DROPITEM_DRAWPOS_Y_MAX		= 300;					// ドロップアイテム取得画面の最大Y座標
		static const int DROPITEM_DRAWPOS_Y_LOW		= SCREEN_SIZE_HEIGHT;	// ドロップアイテム取得画面の最低Y座標
		static const int DROPITEM_FRAME_THICKNESS	= 32;					// ドロップアイテム取得画面フレームの太さ
		static const int GETCARD_INTERVAL			= 255;					// 手札のカード間隔
		static const int GETCARD_POS_Y				= 390;					// 手札のY座標
		static const int PHASE_TIMER_MAX_MOVE		= 60;					// フェーズ用タイマー最大値(移動)
		static const int PHASE_TIMER_MAX_OPEN		= 30;					// フェーズ用タイマー最大値(表)
		// フェーズ
		static const int PHASE_CARD_LINE_UP		= 0;	// カード整列フェーズ
		static const int PHASE_CARD_TABLE		= 1;	// カードを表にするフェーズ
		static const int PHASE_WAIT_DECISION	= 2;	// 決定待機フェーズ

	private:
		/* 変数 */
		// カードリスト
		std::vector<std::shared_ptr<Card_Base>>	GetCardList;			// 取得したカードリスト
		// データリスト
		std::shared_ptr<DataList_GameResource>	pDataList_GameResource;	// ゲームリソース管理用データリスト
		// UIのハンドル
		std::shared_ptr<Scene_UI_Button>		UI_DecisionButton;		// 決定ボタン
		// 画像
		std::shared_ptr<int> Image_Frame_Corner;	// 角
		std::shared_ptr<int> Image_Frame_Line;		// 線
		std::shared_ptr<int> Image_Frame_Inside;	// 中央
		// その他
		Struct_2D::POSITION		SceneGetDropItemDrawPos;	// ドロップアイテム取得画面の描写座標
		bool					OldActiveFlg;				// 以前のドロップアイテム確認シーン有効フラグ
		int						Phase;						// フェーズ
		int						PhaseTimer;					// フェーズ用タイマー
		int						TableCardIndex;				// カードを表にするフェーズ用カードインデックス

		/* 関数 */
		void Update_DrawPos();				// 描写座標の更新
		void BackGround_Drow();				// 背景描写
		void Setup();						// セットアップ
		void ActiveCheck();					// ドロップアイテム確認シーンが無効→有効と変化しているか確認
		void CardPosition_Setup();			// カード位置セットアップ
		void Draw_GetCard();				// 取得カード描写
		void Update_Card();					// カードの更新
		void Update_Phase();				// フェーズの更新
		void EndCheck();					// 終了確認
};
