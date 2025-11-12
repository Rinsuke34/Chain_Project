/* シーン"ロード画面(完了チェッカー)"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// シーン"ロード画面(完了チェッカー)"
class Scene_Load_Checker : public Scene_Base
{
	public:
		Scene_Load_Checker();				// コンストラクタ
		virtual ~Scene_Load_Checker() {};	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
};
