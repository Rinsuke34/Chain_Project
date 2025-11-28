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
	void DrawChain(Struct_2D::POSITION StartPos, Struct_2D::POSITION EndPos);	// 鎖の描写
}
