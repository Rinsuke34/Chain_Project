/* シーン"ビルド設定画面"の定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Build.h"

// テスト
#include "Scene_Title.h"

// コンストラクタ
Scene_Build::Scene_Build() : Scene_Base("Scene_Build", 0, true)
{
	// テスト
	SetUseASyncLoadFlag(TRUE);	
	for (int i = 0; i < 100; i++)
	{
		GrHandle[i] = LoadGraph("resource/Title.png");
	}
	SetUseASyncLoadFlag(FALSE);
}

// デストラクタ
Scene_Build::~Scene_Build()
{
	for (int i = 0; i < 100; i++)
	{
		DeleteGraph(GrHandle[i]);
	}
}

// 更新
void Scene_Build::Update()
{
	if (gstKeyboardInputData.cgInput[INPUT_TRG][KEY_INPUT_Z] == TRUE)
	{
		gpSceneServer->SetDeleteCurrentSceneFlg(true);
		gpSceneServer->SetAddLoadSceneFlg(true);
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Title>());
		return;
	}
}

// 描画
void Scene_Build::Draw()
{
	DrawString(16, 16, "Build", GetColor(255, 255, 255));
}
