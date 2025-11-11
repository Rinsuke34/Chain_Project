/* フレームレート固定クラスの定義 */

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <math.h>
// 外部ライブラリ
#include "DxLib.h"
// ヘッダファイル
#include "Fps.h"
// 共通定義
#include "AppVariableDefine.h"
#include "AppConstantDefine.h"

// コンストラクタ(クラスが使用可能になったタイミングで実行する)
Fps::Fps()
{
	/* 変数の初期化 */
	this->iStartTime	= 0;	// 測定開始時刻	
	this->iCount		= 0;	// カウンタ
}

// フレームレート調整処理
void Fps::FpsAdjustment()
{
	/* 時刻計算 */
	FpsUpdate();

	/* フレームレート調整のため待機 */
	FpsWait();
}

// フレームレート描画(デバッグ用)
void Fps::DrawFPS()
{
	/* フレームレート描画 */
	DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS:%d", giNowFps);
}

// 時刻計算
void Fps::FpsUpdate()
{
	/* カウンタの確認 */
	if (this->iCount == 0)
	{
		// 1フレーム目であるなら
		/* 時刻を取得 */
		this->iStartTime = GetNowCount();
	}
	if (this->iCount == FPS_RATE)
	{
		// フレームレート設定値であるなら
		/* 現在時刻を取得 */
		int t = GetNowCount();

		/* 現在のフレームレートを取得 */
		giNowFps	= (int)(1000.f / ((t - iStartTime) / (float)FPS_RATE));

		/* カウンタ、開始時刻を更新 */
		this->iCount		= 0;
		this->iStartTime	= t;
	}

	/* カウンタの加算 */
	this->iCount++;

	return;
}

// フレームレート調整のため待機
void Fps::FpsWait()
{
	/* フレームレート設定値までにかかった時間の取得 */
	int iTookTime = GetNowCount() - this->iStartTime;

	/* 待機する時間を算出 */
	int iWaitTime	= this->iCount * 1000 / FPS_RATE - iTookTime;

	/* 待機が必要であるか確認 */
	if (iWaitTime > 0)
	{
		// 待機が必要である場合
		/* フレームレート調整のため、待機を行う */
		Sleep(iWaitTime);
	}
}
