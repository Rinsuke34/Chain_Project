/* メインのプロジェクトで使用する関数の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// ロード関係
namespace LOAD_FUNCTION
{
	void	AddLoadScene();	// ロード画面シーンの追加
}

// 描写関係
namespace DRAW_FUNCTION
{
	void DrawChain(Struct_2D::POSITION StartPos, Struct_2D::POSITION EndPos);						// 鎖の描写	
	void DrawChain_Anim(Struct_2D::POSITION StartPos, Struct_2D::POSITION EndPos, int AnimCount);	// 鎖の描写（アニメーション付き）
	void DrawRoad(Struct_2D::POSITION StartPos, Struct_2D::POSITION EndPos);						// 道の描写
	void DrawFrame_Image(Struct_2D::POSITION CenterPos, Struct_2D::POSITION IconSize, int Frame_Thickness, int Image_Corner, int Image_Line, int Image_Inside);	// 画像でのフレームの描写
}
