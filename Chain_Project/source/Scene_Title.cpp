/* シーン"タイトル画面"の定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Title.h"
// 共通定義
#include "FunctionDefine.h"
// 関連クラス
#include "Scene_Build.h"

// コンストラクタ
Scene_Title::Scene_Title() : Scene_Base("Scene_Title", 0, true, false)
{

}

// 更新
void Scene_Title::Update()
{
	if (gstKeyboardInputData.cgInput[INPUT_TRG][KEY_INPUT_Z] == TRUE)
	{
		gpSceneServer->SetDeleteCurrentSceneFlg(true);
		LOAD_FUNCTION::AddLoadScene();
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Build>());
		return;
	}
}

// 描画
void Scene_Title::Draw()
{
	DrawString(16, 16, "Title", GetColor(255, 255, 255));
}
