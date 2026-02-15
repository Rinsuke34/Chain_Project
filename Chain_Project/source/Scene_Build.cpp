/* シーン"ビルド設定画面"の定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Build.h"
// 共通定義
#include "FunctionDefine.h"
#include "VariableDefine.h"
// 関連クラス
#include "Scene_Title.h"
#include "Scene_UI_Button.h"
#include "DataList_Image.h"
#include "DataList_SaveData.h"
#include "Card_Class.h"
#include "Card_Include.h"

// コンストラクタ
Scene_Build::Scene_Build() : Scene_Base("Scene_Build", 0, false, false)
{
	/* データリスト取得 */
	// セーブデータ管理データリスト取得
	this->pDataList_SaveData = std::dynamic_pointer_cast<DataList_SaveData>(gpDataListServer->GetDataList("DataList_SaveData"));

	/* 画像取得 */
	{
		// 画像管理データリスト取得
		std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

		// 背景
		std::string ImageFilePath = "BackGround/Title_BackSky";
		this->Image_BackGround = pDataList_Image->iGetImageHandle(ImageFilePath);
		// 背景の枠の画像
		ImageFilePath = "UI/Button/Button_Frame_Corner";
		this->Image_Frame_Corner = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Button/Button_Frame_Line";
		this->Image_Frame_Line = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Button/Button_Frame_Inside";
		this->Image_Frame_Inside = pDataList_Image->iGetImageHandle(ImageFilePath);
	}

	/* ボタン追加 */
	AddButton();

	/* クラスカードの設定 */
	SetCard_Class();

	/* デッキカードの設定 */
	SetCard_Deck();

	/* カードの位置を設定 */
	SetCardPosition();
}

// デストラクタ
Scene_Build::~Scene_Build()
{
	/* 紐づいたUIを削除 */
	for (int i = 0; i < 3; i++)
	{
		this->UI_Button[i]->SetDeleteFlg(true);
		this->UI_Button[i] = nullptr;
	}
}

// 更新
void Scene_Build::Update()
{
	if (gstKeyboardInputData.cgInput[INPUT_TRG][KEY_INPUT_Z] == TRUE)
	{
		gpSceneServer->SetDeleteCurrentSceneFlg(true);
		LOAD_FUNCTION::AddLoadScene();
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Title>());
		return;
	}

	/* クラス選択の更新 */
	Updaate_SelectClass();
}

// 描画
void Scene_Build::Draw()
{
	/* 背景描写 */
	DrawExtendGraph(
		0, 0,
		SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT,
		*(this->Image_BackGround),
		TRUE
	);

	/* クラス関連の描写 */
	Draw_Class();

	/* HP関連の描写 */
	Draw_Hp();

	/* アビリティ関連の描写 */
	Draw_Ability();

	/* デッキ関連の描写 */
	Draw_Deck();
}

// ボタン追加
void Scene_Build::AddButton()
{
	/* アップグレードボタンの作成 */
	// HP
	this->UI_Button[0] = std::make_shared<Scene_UI_Button>("Upgrade_Hp", 1);
	this->UI_Button[0]->SetButtonText("きょうか");
	this->UI_Button[0]->SetCenterPos({ 200, SCREEN_SIZE_HEIGHT / 2 });
	this->UI_Button[0]->SetFontHandle(giFont_DonguriDuel_32);
	gpSceneServer->AddSceneReservation(this->UI_Button[0]);

	// アビリティ
	this->UI_Button[1] = std::make_shared<Scene_UI_Button>("Upgrade_Ability", 1);
	this->UI_Button[1]->SetButtonText("きょうか");
	this->UI_Button[1]->SetCenterPos({ 430, SCREEN_SIZE_HEIGHT / 2 });
	this->UI_Button[1]->SetFontHandle(giFont_DonguriDuel_32);
	gpSceneServer->AddSceneReservation(this->UI_Button[1]);

	// デッキ
	this->UI_Button[2] = std::make_shared<Scene_UI_Button>("Upgrade_Deck", 1);
	this->UI_Button[2]->SetButtonText("きょうか");
	this->UI_Button[2]->SetCenterPos({ 200, 900 + 20 });
	this->UI_Button[2]->SetFontHandle(giFont_DonguriDuel_32);
	gpSceneServer->AddSceneReservation(this->UI_Button[2]);
}

// クラスカードの設定
void Scene_Build::SetCard_Class()
{
	/* クラスカードを作成 */
	for (int i = 0; i < DataList_SaveData::CLASS_MAX; i++)
	{
		std::shared_ptr<Card_Class> ClassCard = std::make_shared<Card_Class>(i);
		if (i == this->pDataList_SaveData->GetPlayerClassNo())
		{
			// プレイヤーの現在のクラスである場合
			ClassCard->SetCardState(Card_Base::CARDSTATE_CLASS_SELECTED);
		}
		else
		{
			// プレイヤーの現在のクラスでない場合
			ClassCard->SetCardState(Card_Base::CARDSTATE_CLASS_UNSELECTED);
		}

		this->ClassCardList.push_back(ClassCard);
		ClassCard->UpdateImage();
	}
}

// デッキカードの設定
void Scene_Build::SetCard_Deck()
{
	/* 現在のクラスに応じた種類のカードを取得 */
	int PlayerClassNo = this->pDataList_SaveData->GetPlayerClassNo();
	
}

// カードの位置を設定
void Scene_Build::SetCardPosition()
{
	/* クラスカードの総数を取得 */
	int HandCardCount = static_cast<int>(this->ClassCardList.size());

	/* クラスカードの座標設定 */
	for (int i = 0; i < HandCardCount; i++)
	{
		Struct_2D::POSITION SettingPos =
		{
			(SCREEN_SIZE_WIDE / 2) - ((CARD_INTERVAL * (HandCardCount - 1)) / 2) + (CARD_INTERVAL * i),
			190
		};

		this->ClassCardList[i]->SetSettingPos(SettingPos);
		this->ClassCardList[i]->SetNowPos(SettingPos);
	}
}

// クラス選択の更新
void Scene_Build::Updaate_SelectClass()
{
	/* 左クリックが入力されたか確認 */
	if (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT)
	{
		for (int i = 0; i < this->ClassCardList.size(); i++)
		{
			/* カーソルが重なっているか確認 */
			if (this->ClassCardList[i]->MouseInCard())
			{
				// 重なっている場合
				/* クラスカードの状態をすべて"未選択"にする */
				for (int j = 0; j < this->ClassCardList.size(); j++)
				{
					this->ClassCardList[j]->SetCardState(Card_Base::CARDSTATE_CLASS_UNSELECTED);
				}

				/* 選択したクラスカードの状態を"選択中"にする */
				this->ClassCardList[i]->SetCardState(Card_Base::CARDSTATE_CLASS_SELECTED);

				/* プレイヤーのクラスを選択したクラスに設定する */
				this->pDataList_SaveData->SetPlayerClassNo(i);
			}
		}
	}
}

// クラス関連の描写
void Scene_Build::Draw_Class()
{
	/* 枠の描写 */
	int CenterPosX = SCREEN_SIZE_WIDE / 2;
	int CenterPosY = 190;
	DRAW_FUNCTION::DrawFrame_Image(
		{ CenterPosX,	CenterPosY },
		{ 1690,			290 },
		32,
		*(this->Image_Frame_Corner),
		*(this->Image_Frame_Line),
		*(this->Image_Frame_Inside)
	);

	/* クラスカードの描写 */
	for (int i = 0; i < ClassCardList.size(); i++)
	{
		ClassCardList[i]->Draw();
	}
}

// HP関連の描写
void Scene_Build::Draw_Hp()
{
	/* 枠の描写 */
	int CenterPosX = 200;
	int CenterPosY = SCREEN_SIZE_HEIGHT / 2;
	DRAW_FUNCTION::DrawFrame_Image(
		{ CenterPosX,	CenterPosY },
		{ 170,			295 },
		32,
		*(this->Image_Frame_Corner),
		*(this->Image_Frame_Line),
		*(this->Image_Frame_Inside)
	);

	/* 文字列の描写 */
	DrawStringToHandle(
		CenterPosX - GetDrawStringWidthToHandle("たいりょく", static_cast<int>(strlenDx("たいりょく")), giFont_DonguriDuel_32) / 2,
		CenterPosY - 100,
		"たいりょく",
		GetColor(255, 255, 255),
		giFont_DonguriDuel_32
	);
}

// アビリティ関連の描写
void Scene_Build::Draw_Ability()
{
	/* 枠の描写 */
	int CenterPosX = 1070;
	int CenterPosY = SCREEN_SIZE_HEIGHT / 2;
	DRAW_FUNCTION::DrawFrame_Image(
		{ CenterPosX,	CenterPosY },
		{ 1460,			295 },
		32,
		*(this->Image_Frame_Corner),
		*(this->Image_Frame_Line),
		*(this->Image_Frame_Inside)
	);

	/* 文字列の描写 */
	DrawStringToHandle(
		430 - GetDrawStringWidthToHandle("アビリティ", static_cast<int>(strlenDx("アビリティ")), giFont_DonguriDuel_32) / 2,
		(SCREEN_SIZE_HEIGHT / 2) - 100,
		"アビリティ",
		GetColor(255, 255, 255),
		giFont_DonguriDuel_32
	);
}

// デッキ関連の描写
void Scene_Build::Draw_Deck()
{
	/* 枠の描写 */
	int CenterPosX = SCREEN_SIZE_WIDE / 2;
	int CenterPosY = 900 - 10;
	DRAW_FUNCTION::DrawFrame_Image(
		{ CenterPosX,	CenterPosY },
		{ 1690,			290 },
		32,
		*(this->Image_Frame_Corner),
		*(this->Image_Frame_Line),
		*(this->Image_Frame_Inside)
	);

	/* 文字列の描写 */
	DrawStringToHandle(
		200 - GetDrawStringWidthToHandle("デッキ", static_cast<int>(strlenDx("デッキ")), giFont_DonguriDuel_32) / 2,
		900 + 20 - 100,
		"デッキ",
		GetColor(255, 255, 255),
		giFont_DonguriDuel_32
	);

	/* デッキカードの描写 */
	for (int i = 0; i < DeckCardList.size(); i++)
	{
		DeckCardList[i]->Draw();
	}
}
