/* プログラムのメインループ */

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <chrono>
// ヘッダファイル
#include "AppMain.h"
// 関連クラス
#include "Fps.h"
#include "PlayerInput.h"
#include "SceneServer.h"
#include "DataListServer.h"
// 共通定義
#include "AppVariableDefine.h"
#include "AppConstantDefine.h"
#include "AppFunctionDefine.h"

// メインループ
void Main::MainLoop()
{
	/* 名前空間の省略 */
	using namespace Main;	// WinMain関数で使用する関数、変数宣言

	/* DXライブラリ初期化処理 */
	Main::DxLibInit();

	if (DxLib_Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return;
	}

	/* メインプログラム初期化処理 */
	Main::MainInit();

	/* メインループ */
	while (ProcessMessage() == 0)
	{
		/* プログラム終了フラグを確認*/
		if (gbEndFlg == true)
		{
			/* 有効であるならメインループを抜ける(プログラム終了) */
			break;
		}

		/* プレイヤーの入力を取得 */
		gpPlayerInput->Input();

		/* シーン処理 */
		/* 計算 */
		gpSceneServer->SceneProcess();

		/* 描写 */
		ClearDrawScreen();
		{
			/* 描写(通常) */
			gpSceneServer->SceneDraw();

			/* デバッグ用描写 */
			Main::DebugDraw();
		}
		ScreenFlip();

		/* フレームレート調整 */
		gpFps->FpsAdjustment();
	}

	/* 終了処理 */
	Main::MainEnd();

	return;
}

// DXライブラリ初期化
void Main::DxLibInit()
{
	/* ログ出力を無効にする */
	SetOutApplicationLogValidFlag(FALSE);

	/* DXLibの重複を可能にする */
	SetDoubleStartValidFlag(TRUE);

	/* 画面の解像度を横1920×縦1080、1ドットあたり32ビットに指定する */
	SetGraphMode(SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT, 32);

	/* ウィンドウモードに指定する */
	ChangeWindowMode(true);

	/* ウィンドウが非アクティブでもプログラムを実行させる */
	SetAlwaysRunFlag(true);

	/* 使用するDirect3DのバージョンをDirectX11に設定(デフォルトでなっているはずだが念のため) */
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	/* プロジェクトでの初期設定を行う */
	PROJECT_INIT::DxLibInit();
}

// メインプログラム初期化
void Main::MainInit()
{
	/* 描画先画面を裏画面にセット */
	SetDrawScreen(DX_SCREEN_BACK);

	/* FPSを固定するクラスを追加 */
	gpFps = std::make_unique<Fps>();

	/* プレイヤー入力を取得するクラスを追加 */
	gpPlayerInput = std::make_unique<PlayerInput>();

	/* シーンサーバーを作成 */
	gpSceneServer = std::make_unique<SceneServer>();

	/* データリストサーバーを作成 */
	gpDataListServer = std::make_unique<DataListServer>();

	/* プロジェクト初期化処理 */
	PROJECT_INIT::Init();

	/* シーンを初期状態に設定 */
	// ※共通のAppFlameを使用するため各プログラムに応じたシーンに設定する
	SCENE_SET::SetFastScene();

	/* Z深度設定 */
	SetZBufferBitDepth(SCREEN_Z_BUFFER_BIT_DEPTH);

	/* 乱数の初期値を現在の時間をベースに設定 */
	SRand(static_cast<int>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())));
}

// 終了処理
void Main::MainEnd()
{
	/* プレイヤー入力を取得するクラスを削除する */
	gpPlayerInput.reset();

	/* FPSを固定するクラスを削除する */
	gpFps.reset();

	/* シーンサーバーを削除する */
	gpSceneServer.reset();

	/* データリストサーバーを削除する */
	gpDataListServer.reset();

	/* DXライブラリの使用を終了する */
	DxLib_End();
}

// デバッグ描写
void Main::DebugDraw()
{
	/* 対応したデバッグ用フラグが有効であるならばデバッグ用描写を行う */
	/* シーンリスト描写 */
	if (gbDrawSceneListFlg == true)
	{
		gpSceneServer->DrawSceneList();
	}

	/* データリスト描写 */
	if (gbDrawDatalistFlg == true)
	{
		gpDataListServer->DrawDataList();
	}

	/* FPS描写 */
	if (gbDrawFpsFlg == true)
	{
		gpFps->DrawFPS();
	}
}
