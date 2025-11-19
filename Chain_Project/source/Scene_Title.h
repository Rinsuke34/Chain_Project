/* シーン"タイトル画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class Scene_UI_Button;

// シーン"タイトル画面"
class Scene_Title : public Scene_Base
{
	public:
		Scene_Title();			// コンストラクタ
		virtual ~Scene_Title();	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

	private:
		/* 関数 */
		void AdvanceImageLoad();		// 画像リソースの先行ロード
		void AdvanceFontLoad();			// フォントリソースの先行ロード
		void AddButton();				// UI(ボタン)の作成

		/* 変数 */
		// 画像
		std::shared_ptr<int> Image_TitleLogo;	// タイトルロゴ
		std::shared_ptr<int> Image_BackGround;	// タイトル背景
		// UIのハンドル
		std::shared_ptr<Scene_UI_Button> UI_Button[3];	// ボタンUI[0:はじめから, 1:ビルド設定, 2:終了]
};
