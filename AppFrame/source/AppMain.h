/* プログラムのメインループで使用する関数の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 外部ライブラリ
#include <DxLib.h>

// メインループで使用する関数、変数の宣言
namespace Main
{
	/* 関数宣言 */
	void MainLoop();	// メインループ
	void DxLibInit();	// DXライブラリ初期化
	void MainInit();	// メインプログラム初期化
	void MainEnd();		// メインプログラム終了処理
	void DebugDraw();	// デバッグ描写
}


