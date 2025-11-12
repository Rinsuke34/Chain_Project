/* シーン"ビルド設定画面"の定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Build.h"

// テスト
#include "Scene_Title.h"
#include "Scene_Load_Checker.h"
#include "Scene_Load_Draw.h"

// コンストラクタ
Scene_Build::Scene_Build() : Scene_Base("Scene_Build", 0, false, false)
{
	// テスト
	SetUseASyncLoadFlag(TRUE);	
	for (int i = 0; i < 1000; i++)
	{
		GrHandle[i] = LoadGraph("resource/Test.png");
	}
	SetUseASyncLoadFlag(FALSE);
}

// デストラクタ
Scene_Build::~Scene_Build()
{
	for (int i = 0; i < 1000; i++)
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
		//gpSceneServer->AddSceneReservation(std::make_shared<Scene_Load_Checker>());
		//gpSceneServer->AddSceneReservation(std::make_shared<Scene_Load_Draw>());
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Title>());
		return;
	}
}

// 描画
void Scene_Build::Draw()
{
	DrawString(16, 16, "Build", GetColor(255, 255, 255));
}
