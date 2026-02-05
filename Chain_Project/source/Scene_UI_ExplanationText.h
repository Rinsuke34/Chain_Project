/* シーン"UI - 説明テキスト"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// シーン"UI-説明テキスト"
class Scene_UI_ExplanationText : public Scene_Base
{
	public:
		Scene_UI_ExplanationText(const int iLayer);		// コンストラクタ
		virtual ~Scene_UI_ExplanationText() {};			// デストラクタ

		/* 関数 */
		virtual void	Update()	override;	// 更新
		virtual void	Draw()		override;	// 描画

		/* セッター */
		void SetBasePos(Struct_2D::POSITION pos)					{ this->Base_Pos		= pos; }				// 基準座標を設定
		void SetExplanationText(const std::string& ExplanationText)	{ this->ExplanationText	= ExplanationText; }	// 説明テキストを設定

	private:
		/* 変数 */
		// 描写内容
		std::string 			ExplanationText;			// 説明テキスト
		// 画像
		std::shared_ptr<int>	Image_Frame_Corner;			// 角
		std::shared_ptr<int>	Image_Frame_Line;			// 線
		std::shared_ptr<int>	Image_Frame_Inside;			// 内側
		// その他
		Struct_2D::POSITION		Base_Pos;					// 基準座標
		bool					UpwardDisplayFlg;			// 上方向表示フラグ(有効なら基準座標から上側へむけて描写する)

		/* 関数 */
		void	Draw_BackGround();							// 背景描写
};
