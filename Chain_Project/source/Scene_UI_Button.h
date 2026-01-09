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
		static const int FRAME_THICKNESS				= 16;	// 枠の太さ
		static const int EMPHASIS_ANIMATION_THICKNESS	= 32;	// 強調表示アニメーションの太さ
		static const int EMPHASIS_ANIMATION_MAX			= 5;	// 強調表示アニメーションの最大数
		static const int EMPHASIS_ANIMATION_SPEED		= 10;	// 強調表示アニメーションの速度(変更までのフレーム数)

	private:
		/* 変数 */
		int					iFontHandle;		// フォントハンドル
		std::string			ButtonText;			// ボタンテキスト
		Struct_2D::POSITION	CenterPos;			// 位置座標
		bool				bMouseOverFlg;		// カーソルが重なっている状態であるかのフラグ
		int					Anim_ImageNo;		// アニメーションの画像番号
		int					Anim_ChangeDelay;	// アニメーションの変更までの待機時間
		// 画像
		std::shared_ptr<int>	Image_Frame_Corner[2];								// 角[0:通常時, 1:カーソル接触時]
		std::shared_ptr<int>	Image_Frame_Line[2];								// 線[0:通常時, 1:カーソル接触時]
		std::shared_ptr<int>	Image_Frame_Inside[2];								// 内側[0:通常時, 1:カーソル接触時]
		std::shared_ptr<int>	Image_Frame_Emphasis[2][EMPHASIS_ANIMATION_MAX];	// 強調表示アニメーション[0:角, 1:線][アニメーション番号]
};
