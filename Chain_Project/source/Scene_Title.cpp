/* シーン"タイトル画面"の定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Title.h"
// 共通定義
#include "FunctionDefine.h"
#include "VariableDefine.h"
// 関連クラス
#include "Scene_Build.h"
#include "Scene_UI_Button.h"
#include "Scene_Battle.h"
#include "DataList_Image.h"
#include "DataList_Font.h"

// コンストラクタ
Scene_Title::Scene_Title() : Scene_Base("Scene_Title", 0, false, false)
{
	/* 画像リソースの先行ロード */
	AdvanceImageLoad();

	/* フォントリソースの先行ロード */
	AdvanceFontLoad();

	/* UI(ボタン)の作成 */
	AddButton();
}

// デストラクタ
Scene_Title::~Scene_Title()
{
	/* 紐づいたUIを削除 */
	for (int i = 0; i < 3; i++)
	{
		this->UI_Button[i]->SetDeleteFlg(true);
		this->UI_Button[i] = nullptr;
	}
}

// 更新
void Scene_Title::Update()
{
	/* "はじめから"ボタンが押された場合の処理 */
	if (this->UI_Button[0]->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
	{
		gpSceneServer->SetDeleteCurrentSceneFlg(true);
		LOAD_FUNCTION::AddLoadScene();
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Battle>());
		return;
	}

	/* "ビルド設定"ボタンが押された場合の処理 */
	if (this->UI_Button[1]->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
	{
		gpSceneServer->SetDeleteCurrentSceneFlg(true);
		LOAD_FUNCTION::AddLoadScene();
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Build>());
		return;
	}

	/* "終了"ボタンが押された場合の処理 */
	if (this->UI_Button[2]->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
	{
		gbEndFlg = true;
		return;
	}
}

// 描画
void Scene_Title::Draw()
{
	DrawExtendGraph(0, 0, SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT, *(this->Image_BackGround), TRUE);
	DrawExtendGraph(0, 0, SCREEN_SIZE_WIDE, 500, *(this->Image_TitleLogo), TRUE);
}

// 画像リソースの先行ロード
void Scene_Title::AdvanceImageLoad()
{
	/* 非同期で使用予定の画像を事前に読み込む */

	/* データリスト取得 */
	// 画像管理データリスト
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 画像データ読み込み */
	// タイトルロゴ
	std::string ImageFilePath = "Logo/Title";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	this->Image_TitleLogo = pDataList_Image->iGetImageHandle(ImageFilePath);
	// タイトル背景
	ImageFilePath = "BackGround/Title_BackGround";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	this->Image_BackGround = pDataList_Image->iGetImageHandle(ImageFilePath);
	// UI(ボタン)
	ImageFilePath = "UI/Button/Button_Frame_Corner";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	ImageFilePath = "UI/Button/Button_Frame_Corner_Over";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	ImageFilePath = "UI/Button/Button_Frame_Line";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	ImageFilePath = "UI/Button/Button_Frame_Line_Over";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	ImageFilePath = "UI/Button/Button_Frame_Inside";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	ImageFilePath = "UI/Button/Button_Frame_Inside_Over";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	// UI(バトル-バトルエリア)
	ImageFilePath = "UI/Battle/UI_BattleArea";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	ImageFilePath = "UI/Battle/UI_BattleArea_Over";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	// キャラクター(テスト)
	ImageFilePath = "Character/Player/Test_Player";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	ImageFilePath = "Character/Monster/Test";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	// カード(フレーム)
	ImageFilePath = "Card_Commoon/Frame_Test";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	// カード(背景)
	ImageFilePath = "Card_Commoon/BackGround_Test";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	// カード(スート)
	ImageFilePath = "Card_Suit/Suit_Sword";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	ImageFilePath = "Card_Suit/Suit_Shield";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
}

// フォントリソースの先行ロード
void Scene_Title::AdvanceFontLoad()
{
	/* 非同期で使用予定のフォントを事前に読み込む */

	/* データリスト取得 */
	// フォント管理データリスト
	std::shared_ptr<DataList_Font> pDataList_Font = std::dynamic_pointer_cast<DataList_Font>(gpDataListServer->GetDataList("DataList_Font"));

	/* フォントデータ読み込み */
	// どんぐりデュエル(16px)
	std::string FontFilePath = "DonguriDuel_16px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 0);
	giFont_DonguriDuel_16 = pDataList_Font->iGetFontHnadle(FontFilePath);
	// どんぐりデュエル(32px)
	FontFilePath = "DonguriDuel_32px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 0);
	giFont_DonguriDuel_32 = pDataList_Font->iGetFontHnadle(FontFilePath);
	// どんぐりデュエル(64px)
	FontFilePath = "DonguriDuel_64px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 0);
	giFont_DonguriDuel_64 = pDataList_Font->iGetFontHnadle(FontFilePath);
	// チェックポイント・ピリオド(16px)
	FontFilePath = "Cp_Period_16px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 0);
	giFont_Cp_Period_16 = pDataList_Font->iGetFontHnadle(FontFilePath);
	// チェックポイント・ピリオド(32px)
	FontFilePath = "Cp_Period_32px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 0);
	giFont_Cp_Period_32 = pDataList_Font->iGetFontHnadle(FontFilePath);
}

// UI(ボタン)の作成
void Scene_Title::AddButton()
{
	/* "はじめから"ボタンの作成 */
	this->UI_Button[0] = std::make_shared<Scene_UI_Button>("Title_StartButton", 1);
	this->UI_Button[0]->SetButtonText("はじめから");
	this->UI_Button[0]->SetCenterPos({ SCREEN_SIZE_WIDE / 2, SCREEN_SIZE_HEIGHT / 2 + 100 });
	this->UI_Button[0]->SetFontHandle(giFont_DonguriDuel_32);
	gpSceneServer->AddSceneReservation(this->UI_Button[0]);

	/* "ビルド設定"ボタンの作成 */
	this->UI_Button[1] = std::make_shared<Scene_UI_Button>("Title_BuildButton", 1);
	this->UI_Button[1]->SetFontHandle(giFont_DonguriDuel_32);
	this->UI_Button[1]->SetButtonText("ビルドせってい");
	this->UI_Button[1]->SetCenterPos({ SCREEN_SIZE_WIDE / 2, SCREEN_SIZE_HEIGHT / 2 + 200 });
	gpSceneServer->AddSceneReservation(this->UI_Button[1]);

	/* "終了"ボタンの作成 */
	this->UI_Button[2] = std::make_shared<Scene_UI_Button>("Title_EndButton", 1);
	this->UI_Button[2]->SetFontHandle(giFont_DonguriDuel_32);
	this->UI_Button[2]->SetButtonText("しゅうりょう");
	this->UI_Button[2]->SetCenterPos({ SCREEN_SIZE_WIDE / 2, SCREEN_SIZE_HEIGHT / 2 + 300 });
	gpSceneServer->AddSceneReservation(this->UI_Button[2]);
}
