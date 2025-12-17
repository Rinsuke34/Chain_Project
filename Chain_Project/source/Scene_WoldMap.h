/* シーン"ワールドマップ画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class Scene_UI_Button;

// シーン"タイトル画面"
class Scene_WoldMap : public Scene_Base
{
	public:
		Scene_WoldMap();			// コンストラクタ
		virtual ~Scene_WoldMap();	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

	private:
		/* 変数 */
		// 画像
		std::shared_ptr<int> Image_BackGround;	// ワールドマップ背景背景
};
