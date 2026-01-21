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
		virtual void	Draw()		override;						// 描画

	private:
		/* 変数 */
		// データリスト
		std::shared_ptr<DataList_GameResource>	pDataList_GameResource;	// ゲームリソース管理用データリスト
		// 画像
		std::shared_ptr<int>	Image_Coin;				// コイン
		std::shared_ptr<int>	Image_Frame_Corner;		// 角
		std::shared_ptr<int>	Image_Frame_Line;		// 線
		std::shared_ptr<int>	Image_Frame_Inside;		// 内側
};
