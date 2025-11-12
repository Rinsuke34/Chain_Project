/* シーン"タイトル画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// シーン"タイトル画面"
class Scene_Title : public Scene_Base
{
	public:
		Scene_Title();				// コンストラクタ
		virtual ~Scene_Title() {};	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

	private:

};
