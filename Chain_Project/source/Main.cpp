/* プログラム起動時の処理の定義 */

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <windows.h>
// 外部ライブラリ
#include "AppFrame.h"
// 関連クラス
#include "Scene_Title.h"
#include "DataList_Image.h"

// メインプログラム
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/* AppFrameのメインループを実行 */
	Main::MainLoop();

	return 0;
};

// 以下、AppFrameで宣言された関数の定義
// シーンの設定
namespace SCENE_SET
{
	// ゲーム起動時のシーンをセット
	void	SetFastScene()
	{
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Title>());
	}
}

// 初期化
namespace PROJECT_INIT
{
	// DXライブラリ初期化設定
	void	DxLibInit()
	{
		/* タイトルの設定 */
		SetMainWindowText("ChainCrash");
	}

	// プロジェクト初期化処理
	void	Init()
	{
		/* データリストサーバーへの登録 */
		gpDataListServer->AddDataList(std::make_shared<DataList_Image>());					// 画像データ管理
	}
}
