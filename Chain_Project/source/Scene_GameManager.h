/* シーン"ゲーム進行管理"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class DataList_GameResource;

// シーン"ゲーム進行管理"
class Scene_GameManager : public Scene_Base
{
	public:
		Scene_GameManager();			// コンストラクタ
		virtual ~Scene_GameManager();	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新

		/* 定数 */

	private:
		/* 関数 */
		virtual void	Create_StageScene();		// ステージシーンの作成

		/* 変数 */
		std::shared_ptr<DataList_GameResource>	pDataList_GameResource;		// ゲームリソース管理用データリスト
};
