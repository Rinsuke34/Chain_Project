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

	private:
		/* 変数 */
		// カードリスト
		std::vector<std::shared_ptr<Card_Base>>	GetCardList;			// 取得したカードリスト
		// データリスト
		std::shared_ptr<DataList_GameResource>	pDataList_GameResource;	// ゲームリソース管理用データリスト
		// 画像
		int						Image_SceneGetDropItem;		// ドロップアイテム取得画面の画像
		// その他
		Struct_2D::POSITION		SceneGetDropItemDrawPos;	// ドロップアイテム取得画面の描写座標
		bool					OldActiveFlg;				// 以前のドロップアイテム確認シーン有効フラグ
		// UIのハンドル
		std::shared_ptr<Scene_UI_Button> UI_DecisionButton;	// 決定ボタン

		/* 関数 */
		void Update_DrawPos();				// 描写座標の更新
		void Update_Image();				// 画像の更新
		void SceneGetDropItem_Drow();		// ドロップアイテム取得画面の描画
		void BackGround_Drow();				// 背景描写
		void Setup();						// セットアップ
		void ActiveCheck();					// ドロップアイテム確認シーンが無効→有効と変化しているか確認
};
