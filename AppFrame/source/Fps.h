/* フレームレート固定クラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 外部ライブラリ
#include "DxLib.h"

// フレームレート固定クラス
class Fps
{
	public:
		Fps();				// コンストラクタ
		virtual ~Fps() {};	// デストラクタ

		/* 関数 */
		void	FpsAdjustment();		// フレームレート調整処理
		void	DrawFPS();				// フレームレート描画(デバッグ用)

	private:
		/* 関数 */
		void FpsUpdate();				// 時刻計算
		void FpsWait();					// フレームレート調整のため待機

		/* 変数 */
		int iStartTime;					// 測定開始時刻
		int iCount;						// カウンタ

};
