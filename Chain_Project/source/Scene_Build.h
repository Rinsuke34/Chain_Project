/* シーン"ビルド設定画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// シーン"ビルド設定画面"
class Scene_Build : public Scene_Base
{
	public:
		Scene_Build();				// コンストラクタ
		virtual ~Scene_Build();		// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

	private:
		// テスト用
		int GrHandle[100];
};
