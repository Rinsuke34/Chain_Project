/* シーン"パーティクル(文字)"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// シーン"パーティクル(文字)"
class Scene_Particles_Text : public Scene_Base
{
	public:
		Scene_Particles_Text();				// コンストラクタ
		virtual ~Scene_Particles_Text() {};	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

		/* セッター */
		void SetText(const std::string& Text)				{ this->Text		= Text; }			// 表示テキストを設定
		void SetPosition(const Struct_2D::POSITION& Pos)	{ this->Pos			= Pos; }			// 位置座標を設定
		void SetFontHandle(int iFontHandle)					{ this->iFontHandle	= iFontHandle; }	// フォントハンドルを設定
		void SetMove(const Struct_2D::POSITION& Move)		{ this->Move		= Move; }			// 移動量を設定
		void SetAlpha(int iAlpha)							{ this->iAlpha		= iAlpha; }			// 透明度を設定
		void SetColor(unsigned int Color)					{ this->Color		= Color; }			// 色を設定

	private:
		/* 変数 */
		std::string 		Text;			// 表示テキスト
		Struct_2D::POSITION	Pos;			// 位置座標
		int					iFontHandle;	// フォントハンドル
		Struct_2D::POSITION	Move;			// 移動量
		int					iAlpha;			// 透明度
		unsigned int		Color;          // 色
};
