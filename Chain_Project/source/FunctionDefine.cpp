/* メインのプロジェクトで使用する関数の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "FunctionDefine.h"

// ロード関係
namespace LOAD_FUNCTION
{
	// ロード画面シーンの追加
	void	AddLoadScene()
	{
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Load_Checker>());
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Load_Draw>());
	}
}
