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
#include "Scene_GameManager.h"
#include "DataList_Image.h"
#include "DataList_Font.h"
#include "Card_Base.h"
#include "DataList_Sound.h"

// コンストラクタ
Scene_Title::Scene_Title(bool AnimFlg) : Scene_Base("Scene_Title", 0, false, false)
{
	// 引数
	// bool AnimFlg : アニメーションフラグ(有効ならばアニメーションあり、無効ならばアニメーションなし)

	/* 初期化 */
	this->RotationAngle_MagicalCircle	= 0.0;					// 魔法陣の回転角度
	this->LogoAlpha_White				= 0;					// タイトルロゴ(白)のアルファ値
	this->LogoAlpha						= 0;					// タイトルロゴのアルファ値
	this->CompassPosY					= -550;					// コンパスの描写座標
	this->MagicalCircleScale			= 0.f;					// 魔法陣の拡大率
	this->BuildingPosY					= SCREEN_SIZE_HEIGHT;	// 建物の描写座標Y
	this->SkyAlpha						= 0;					// 空のアルファ値

	if (AnimFlg)
	{
		// アニメーションありの場合はブラックアウトから開始
		this->iPhase		= PHASE_TEAMLOGO;		// フェーズ
		this->Counter_Phase = 300;					// フェーズ用カウンター
	}
	else
	{
		// アニメーションなしの場合は最初から完全に描写された状態で開始
		this->iPhase				= PHASE_ADD_BUTTON;
		this->LogoAlpha_White		= 0;		// タイトルロゴ(白)のアルファ値
		this->LogoAlpha				= 255;		// タイトルロゴのアルファ値
		this->CompassPosY			= 400;		// コンパスの描写座標
		this->MagicalCircleScale	= 1.75f;	// 魔法陣の拡大率
		this->BuildingPosY			= 100;		// 建物の描写座標Y
		this->SkyAlpha				= 255;		// 空のアルファ値
	}
	

	/* BGM音声データ読み込み */
	// サウンド管理データリスト取得
	std::shared_ptr<DataList_Sound> pDataList_Sound = std::dynamic_pointer_cast<DataList_Sound>(gpDataListServer->GetDataList("DataList_Sound"));
	// BGM音声データ読み込み
	std::string BgmFilePath = "fantasyX";
	pDataList_Sound->LoadBgmSound(BgmFilePath);

	/* 画像リソースの先行ロード */
	AdvanceImageLoad();

	/* フォントリソースの先行ロード */
	AdvanceFontLoad();
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
	/* フェーズに応じた処理を行う */
	switch (this->iPhase)
	{
		// チームロゴ描写
		case PHASE_TEAMLOGO:
			this->Counter_Phase--;
			if (this->Counter_Phase <= 0)
			{
				this->iPhase		= PHASE_BLACKOUT;
				this->Counter_Phase	= 100;
			}
			break;

		// ブラックアウト
		case PHASE_BLACKOUT:
			this->Counter_Phase--;
			if (this->Counter_Phase <= 0)
			{
				this->iPhase		= PHASE_LOGO_WHITE;
				this->Counter_Phase	= 200;
			}
			break;

		// タイトルロゴ(白)表示
		case PHASE_LOGO_WHITE:
			if (this->LogoAlpha_White < 255)
			{
				this->LogoAlpha_White++;

				this->Counter_Phase--;
				if (this->Counter_Phase <= 0)
				{
					this->LogoAlpha = 255;
					this->iPhase = PHASE_COMPASS_FALL;
					this->Counter_Phase = 200;
				}
			}
			break;

		// コンパス落下
		case PHASE_COMPASS_FALL:
			if (this->CompassPosY < 400)
			{
				this->CompassPosY += 10;
			}
			if (this->BuildingPosY > 100)
			{
				this->BuildingPosY -= 10;
			}
			this->Counter_Phase--;
			if (this->Counter_Phase <= 0)
			{
				this->CompassPosY	= 400;
				this->BuildingPosY	= 100;
				this->iPhase		= PHASE_LOGO_COLOR;
				this->Counter_Phase	= 200;
			}
			break;

		// タイトルロゴ(カラー)表示
		case PHASE_LOGO_COLOR:
			if (this->LogoAlpha_White > 0)
			{
				this->LogoAlpha_White--;
			}
			if (this->SkyAlpha < 255)
			{
				this->SkyAlpha += 2;
			}
			if (this->MagicalCircleScale < 1.75f)
			{
				this->MagicalCircleScale += 0.015f;
			}
			this->Counter_Phase--;
			if (this->Counter_Phase <= 0)
			{
				// 完全に描写されたら次のフェーズへ
				this->LogoAlpha_White		= 0;
				this->iPhase				= PHASE_ADD_BUTTON;
				this->SkyAlpha				= 255;
				this->MagicalCircleScale	= 1.75f;
			}
			break;

		// ボタン表示
		case PHASE_ADD_BUTTON:
			if (this->UI_Button[0] == nullptr)
			{
				/* UI(ボタン)の作成 */
				AddButton();

				/* BGM再生 */
				// サウンド管理データリスト取得
				std::shared_ptr<DataList_Sound> pDataList_Sound = std::dynamic_pointer_cast<DataList_Sound>(gpDataListServer->GetDataList("DataList_Sound"));
				if (!pDataList_Sound->CheckPlayBgm())
				{
					// 現在読み込まれているBGM音声データ再生
					pDataList_Sound->PlayBgmSound_Now();
				}
			}
			else
			{
				/* "はじめから"ボタンが押された場合の処理 */
				if (this->UI_Button[0]->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
				{
					/* ゲーム画面へ遷移 */
					gpSceneServer->SetDeleteCurrentSceneFlg(true);
					LOAD_FUNCTION::AddLoadScene();
					gpSceneServer->AddSceneReservation(std::make_shared<Scene_GameManager>());

					/* BGMを停止 */
					// サウンド管理データリスト取得
					std::shared_ptr<DataList_Sound> pDataList_Sound = std::dynamic_pointer_cast<DataList_Sound>(gpDataListServer->GetDataList("DataList_Sound"));
					pDataList_Sound->StopBgmSound();
					return;
				}

				/* "ビルド設定"ボタンが押された場合の処理 */
				if (this->UI_Button[1]->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
				{
					/* ビルド画面へ遷移 */
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
			break;
	}

	/* クリックが押されたらボタン表示フェーズまでスキップ */
	if ((this->iPhase != PHASE_ADD_BUTTON) && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
	{
		this->iPhase				= PHASE_ADD_BUTTON;
		this->LogoAlpha_White		= 0;		// タイトルロゴ(白)のアルファ値
		this->LogoAlpha				= 255;		// タイトルロゴのアルファ値
		this->CompassPosY			= 400;		// コンパスの描写座標
		this->MagicalCircleScale	= 1.75f;	// 魔法陣の拡大率
		this->BuildingPosY			= 100;		// 建物の描写座標Y
		this->SkyAlpha				= 255;		// 空のアルファ値
	}

	/* 魔法陣の回転角度更新 */
	this->RotationAngle_MagicalCircle += 0.005;
}

// 描画
void Scene_Title::Draw()
{
	/* ロゴの描写 */
	if (this->iPhase == PHASE_TEAMLOGO)
	{
		/* 背景(白)の描写 */
		// フェーズ用カウンターに基づいてアルファ値を計算
		int alpha = 0;
		if (this->Counter_Phase >= 200)
		{
			// 200～300 : 徐々に薄くなる (255 → 0)
			alpha = (300 - this->Counter_Phase) * 255 / 100;
		}
		else if (this->Counter_Phase >= 100)
		{
			// 100～200 : 完全に描写
			alpha = 255;
		}
		else if (this->Counter_Phase >= 0)
		{
			// 0～100 : 徐々に濃くなる (0 → 255)	
			alpha = this->Counter_Phase * 255 / 100;
		}

		// アルファブレンドモードを設定して描写
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(0, 0, SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT, GetColor(255, 255, 255), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		/* 文字の描写 */
		DrawExtendGraph(
			(SCREEN_SIZE_WIDE / 2) - 256, (SCREEN_SIZE_HEIGHT / 2) - 256,
			(SCREEN_SIZE_WIDE / 2) + 256, (SCREEN_SIZE_HEIGHT / 2) + 256,
			*(this->Image_TeamLogo), TRUE);
	}

	/* 空の描写 */
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->SkyAlpha);
	DrawExtendGraph(0, 0, SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT, *(this->Image_BackGround[0]), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	/* 建造物の描写 */
	DrawExtendGraph(0, 0 + this->BuildingPosY, SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT + this->BuildingPosY, *(this->Image_BackGround[1]), TRUE);

	/* 魔法陣の描写 */
	int LogoCenterPosX = SCREEN_SIZE_WIDE / 2 - 375;
	int LogoCenterPosY = this->CompassPosY;
	DrawRotaGraph(
		LogoCenterPosX,
		LogoCenterPosY,
		this->MagicalCircleScale,
		this->RotationAngle_MagicalCircle,
		*(this->Image_TitleLogo[2]),
		TRUE);

	/* コンパスの描写 */
	int LogoSizeX = 126 * 5, LogoSizeY = 126 * 5;
	DrawExtendGraph(
		LogoCenterPosX - (LogoSizeX / 2), LogoCenterPosY - (LogoSizeY / 2),
		LogoCenterPosX + (LogoSizeX / 2), LogoCenterPosY + (LogoSizeY / 2),
		*(this->Image_TitleLogo[1]),
		TRUE);

	/* タイトルの文字列の描写 */
	LogoSizeX = 256 * 5, LogoSizeY = 64 * 5;
	LogoCenterPosX = SCREEN_SIZE_WIDE / 2;
	LogoCenterPosY = 400;
	// カラー
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->LogoAlpha);
	DrawExtendGraph(
		LogoCenterPosX - (LogoSizeX / 2), LogoCenterPosY - (LogoSizeY / 2),
		LogoCenterPosX + (LogoSizeX / 2), LogoCenterPosY + (LogoSizeY / 2),
		*(this->Image_TitleLogo[0]),
		TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// 白
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->LogoAlpha_White);
	DrawExtendGraph(
		LogoCenterPosX - (LogoSizeX / 2), LogoCenterPosY - (LogoSizeY / 2),
		LogoCenterPosX + (LogoSizeX / 2), LogoCenterPosY + (LogoSizeY / 2),
		*(this->Image_TitleLogo[3]),
		TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
	this->Image_TitleLogo[0] = pDataList_Image->iGetImageHandle(ImageFilePath);
	ImageFilePath = "Logo/Title_Compass";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	this->Image_TitleLogo[1] = pDataList_Image->iGetImageHandle(ImageFilePath);
	ImageFilePath = "Logo/Title_MagicCircle";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	this->Image_TitleLogo[2] = pDataList_Image->iGetImageHandle(ImageFilePath);
	ImageFilePath = "Logo/Title_White";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	this->Image_TitleLogo[3] = pDataList_Image->iGetImageHandle(ImageFilePath);
	// タイトル背景
	ImageFilePath = "BackGround/Title_BackSky";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	Image_BackGround[0] = pDataList_Image->iGetImageHandle(ImageFilePath);
	ImageFilePath = "BackGround/Title_BackBuilding";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	Image_BackGround[1] = pDataList_Image->iGetImageHandle(ImageFilePath);
	// チームロゴ
	ImageFilePath = "Logo/TeamRogo";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	Image_TeamLogo = pDataList_Image->iGetImageHandle(ImageFilePath);
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
	// カード(ネームプレート)
	ImageFilePath = "Card_Commoon/NamePlate";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	// カード(スートフレーム)
	ImageFilePath = "Card_Commoon/SuitFrame_Test";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	// カード(スート)
	ImageFilePath = "Card_Suit/" + Card_Base::SUITE_SWORD;
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	ImageFilePath = "Card_Suit/" + Card_Base::SUITE_SHIELD;
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	// 鎖関係
	ImageFilePath = "Card_Commoon/Chain";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	ImageFilePath = "Card_Commoon/Chain_SuitFrame";
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
	// どんぐりデュエル(32px)
	std::string FontFilePath = "DonguriDuel_32px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 0);
	giFont_DonguriDuel_32 = pDataList_Font->iGetFontHnadle(FontFilePath);
	// どんぐりデュエル(64px)
	FontFilePath = "DonguriDuel_64px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 0);
	giFont_DonguriDuel_64 = pDataList_Font->iGetFontHnadle(FontFilePath);
	// JF-Dot-MPlus10(16px)
	FontFilePath = "JF-Dot-MPlus10_16px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 0);
	giFont_JF_Dot_MPlus10_16 = pDataList_Font->iGetFontHnadle(FontFilePath);
	// JF-Dot-MPlus10(16px/エッジ付き)
	FontFilePath = "JF-Dot-MPlus10_16px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 8);
	giFont_JF_Dot_MPlus10_16_Edge = pDataList_Font->iGetFontHnadle(FontFilePath);
	// JF-Dot-MPlus10(20px)
	FontFilePath = "JF-Dot-MPlus10_20px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 0);
	giFont_JF_Dot_MPlus10_20 = pDataList_Font->iGetFontHnadle(FontFilePath);
	// JF-Dot-MPlus10(20px/エッジ付き)
	FontFilePath = "JF-Dot-MPlus10_20px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 10);
	giFont_JF_Dot_MPlus10_20_Edge = pDataList_Font->iGetFontHnadle(FontFilePath);
	// JF-Dot-MPlus10(24px)
	FontFilePath = "JF-Dot-MPlus10_24px";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 0);
	giFont_JF_Dot_MPlus10_24 = pDataList_Font->iGetFontHnadle(FontFilePath);
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
