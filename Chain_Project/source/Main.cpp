/* プログラム起動時の処理の定義 */

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <windows.h>
// 外部ライブラリ
#include "AppFrame.h"
// 関連クラス
#include "Scene_Title.h"
#include "Scene_Load_Checker.h"
#include "Scene_Load_Draw.h"

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
		//gpSceneServer->AddSceneReservation(std::make_shared<Scene_Load_Checker>());
		//gpSceneServer->AddSceneReservation(std::make_shared<Scene_Load_Draw>());
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Title>());
	}
}

// 初期化
namespace PROJECT_INIT
{
	// プロジェクト初期化処理
	void	Init()
	{
		/* データリストサーバーへの登録 */
		//gpDataListServer->AddDataList(std::make_shared<DataList_Image>());					// 画像データ管理
		//gpDataListServer->AddDataList(std::make_shared<DataList_Model>());					// 3Dモデル管理
		//gpDataListServer->AddDataList(std::make_shared<DataList_2DPartsAnimCreateTool>());	// 2Dパーツアニメーション作成ツール
		//gpDataListServer->AddDataList(std::make_shared<DataList_Sound>());					// サウンド管理
	}
}
