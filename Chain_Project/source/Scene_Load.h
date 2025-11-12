/* シーン"ロード画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// シーン"ロード画面"
class Scene_Load : public Scene_Base
{
	public:
		Scene_Load();				// コンストラクタ
		virtual ~Scene_Load() {};	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画
};
