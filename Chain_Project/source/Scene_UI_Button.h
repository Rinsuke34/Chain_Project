/* シーン"UI-ボタン"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// シーン"UI-ボタン"
class Scene_UI_Button : public Scene_Base
{
	public:
		Scene_UI_Button(const std::string& cName, const int iLayer);				// コンストラクタ
		virtual ~Scene_UI_Button() {};	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

		/* セッター */
		void SetFontHandle(int iFontHandle)						{ this->iFontHandle = iFontHandle; }	// フォントハンドルを設定
		void SetButtonText(const std::string& ButtonText)		{ this->ButtonText = ButtonText; }		// ボタンテキストを設定
		void SetCenterPos(const Struct_2D::POSITION& CenterPos)	{ this->CenterPos = CenterPos; }		// 位置座標を設定

		/* ゲッター */
		bool GetMouseOverFlg() const { return this->bMouseOverFlg; }		// カーソルが重なっている状態であるかのフラグを取得

		/* 定数 */
		static const int FRAME_THICKNESS = 16;	// 枠の太さ

	private:
		/* 変数 */
		int					iFontHandle;	// フォントハンドル
		std::string			ButtonText;		// ボタンテキスト
		Struct_2D::POSITION	CenterPos;		// 位置座標
		bool				bMouseOverFlg;	// カーソルが重なっている状態であるかのフラグ
		// 画像
		// ※ 0:通常時、1:カーソルが重なっている
		std::shared_ptr<int>	Image_Frame_Corner[2];	// 角
		std::shared_ptr<int>	Image_Frame_Line[2];	// 線
		std::shared_ptr<int>	Image_Frame_Inside[2];	// 内側
};
