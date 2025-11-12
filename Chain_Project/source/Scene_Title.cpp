/* シーン"タイトル画面"の定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Title.h"

// テスト
#include "Scene_Build.h"

// コンストラクタ
Scene_Title::Scene_Title() : Scene_Base("Scene_Title", 0, true)
{

}

// 更新
void Scene_Title::Update()
{
	if (gstKeyboardInputData.cgInput[INPUT_TRG][KEY_INPUT_Z] == TRUE)
	{
		gpSceneServer->SetDeleteCurrentSceneFlg(true);
		gpSceneServer->SetAddLoadSceneFlg(true);
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Build>());
		return;
	}
}

// 描画
void Scene_Title::Draw()
{
	DrawString(16, 16, "Title", GetColor(255, 255, 255));
}
