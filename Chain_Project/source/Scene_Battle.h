/* シーン"バトル画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// シーン"バトル画面"
class Scene_Battle : public Scene_Base
{
	public:
		Scene_Battle();				// コンストラクタ
		virtual ~Scene_Battle();	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

	private:
};
