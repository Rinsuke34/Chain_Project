/* シーン"ショップ画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class DataList_GameResource;
class Scene_UI_Button;

// シーン"ショップ画面"
class Scene_Shop : public Scene_Base
{
	public:
		Scene_Shop();			// コンストラクタ
		virtual ~Scene_Shop();	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

	private:
		/* 変数 */
		// データリスト
		std::shared_ptr<DataList_GameResource>	pDataList_GameResource;	// ゲームリソース管理用データリスト
		// UIのハンドル
		std::shared_ptr<Scene_UI_Button>		UI_DecisionButton;		// 決定ボタン
		// その他
		bool									ShopEndFlg;		// ショップ終了フラグ
};
