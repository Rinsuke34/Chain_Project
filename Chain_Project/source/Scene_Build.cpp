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
	/* "タイトルへ戻るボタンが押されるか、Escキーが入力されたらタイトル画面へ遷移" */
	if ((this->UI_Button[3]->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT)) || (gstKeyboardInputData.cgInput[INPUT_TRG][KEY_INPUT_ESCAPE] == TRUE))
	{
		gpSceneServer->SetDeleteCurrentSceneFlg(true);
		LOAD_FUNCTION::AddLoadScene();
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Title>());
		return;
	}

	/* クラス選択の更新 */
	Update_SelectClass();

	/* レベルアップ処理 */
	Update_LevelUp();
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

	/* 所持経験値の描写 */
	Draw_HaveExp();
}

// ボタン追加
void Scene_Build::AddButton()
{
	/* アップグレードボタンの作成 */
	// HP
	this->UI_Button[0] = std::make_shared<Scene_UI_Button>("Upgrade_Hp", 1);
	this->UI_Button[0]->SetButtonText("きょうか");
	this->UI_Button[0]->SetCenterPos({ 200, (SCREEN_SIZE_HEIGHT / 2) - 20 });
	this->UI_Button[0]->SetFontHandle(giFont_DonguriDuel_32);
	gpSceneServer->AddSceneReservation(this->UI_Button[0]);

	// アビリティ
	this->UI_Button[1] = std::make_shared<Scene_UI_Button>("Upgrade_Ability", 1);
	this->UI_Button[1]->SetButtonText("きょうか");
	this->UI_Button[1]->SetCenterPos({ 430, (SCREEN_SIZE_HEIGHT / 2) - 20 });
	this->UI_Button[1]->SetFontHandle(giFont_DonguriDuel_32);
	gpSceneServer->AddSceneReservation(this->UI_Button[1]);

	// デッキ
	this->UI_Button[2] = std::make_shared<Scene_UI_Button>("Upgrade_Deck", 1);
	this->UI_Button[2]->SetButtonText("きょうか");
	this->UI_Button[2]->SetCenterPos({ 200, 900 - 10 });
	this->UI_Button[2]->SetFontHandle(giFont_DonguriDuel_32);
	gpSceneServer->AddSceneReservation(this->UI_Button[2]);

	/* タイトルへもどるボタンの作成 */
	this->UI_Button[3] = std::make_shared<Scene_UI_Button>("Return_Title", 1);
	this->UI_Button[3]->SetButtonText("タイトルへ\nもどる");
	this->UI_Button[3]->SetCenterPos({ 200, 100 });
	this->UI_Button[3]->SetFontHandle(giFont_DonguriDuel_32);
	gpSceneServer->AddSceneReservation(this->UI_Button[3]);
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
void Scene_Build::Update_SelectClass()
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
	int CenterPosX = 1070;
	int CenterPosY = 190;
	DRAW_FUNCTION::DrawFrame_Image(
		{ CenterPosX,	CenterPosY },
		{ 1460,			295 },
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
	CenterPosY = (SCREEN_SIZE_HEIGHT / 2) - 120;
	DrawStringToHandle(
		CenterPosX - GetDrawStringWidthToHandle("たいりょく", static_cast<int>(strlenDx("たいりょく")), giFont_DonguriDuel_32) / 2,
		CenterPosY,
		"たいりょく",
		GetColor(255, 255, 255),
		giFont_DonguriDuel_32
	);
	std::string HpLevelText	= "レベル　";
	int Color				= GetColor(255, 255, 255);
	if (this->pDataList_SaveData->GetLevel_Hp(this->pDataList_SaveData->GetPlayerClassNo()) >= 5)
	{
		HpLevelText += "MAX";
		Color = GetColor(255, 255, 0);
	}
	else
	{
		HpLevelText += std::to_string(this->pDataList_SaveData->GetLevel_Hp(this->pDataList_SaveData->GetPlayerClassNo()));
	}
	DrawStringToHandle(
		CenterPosX - GetDrawStringWidthToHandle(HpLevelText.c_str(), static_cast<int>(strlenDx(HpLevelText.c_str())), giFont_DonguriDuel_32) / 2,
		CenterPosY + 32,
		HpLevelText.c_str(),
		Color,
		giFont_DonguriDuel_32
	);
	// レベルが最大でない場合、次のレベルまでの経験値を描写する
	if (this->pDataList_SaveData->GetLevel_Hp(this->pDataList_SaveData->GetPlayerClassNo()) < 5)
	{
		int NextLevelExp = 10 + (10 * (this->pDataList_SaveData->GetLevel_Hp(this->pDataList_SaveData->GetPlayerClassNo())));
		std::string NextLevelExpText = std::to_string(NextLevelExp) + "Exp";
		DrawStringToHandle(
			CenterPosX - GetDrawStringWidthToHandle(NextLevelExpText.c_str(), static_cast<int>(strlenDx(NextLevelExpText.c_str())), giFont_DonguriDuel_32) / 2,
			CenterPosY + 32 + 96,
			NextLevelExpText.c_str(),
			GetColor(255, 255, 255),
			giFont_DonguriDuel_32
		);
	}

	/* HP量の描写 */
	int ClassNo		= this->pDataList_SaveData->GetPlayerClassNo();		// クラス番号
	int Level_Hp	= this->pDataList_SaveData->GetLevel_Hp(ClassNo);	// HPレベル
	int Hp_IncreaseAmount = 0;											// レベルごとのHP上昇量
	switch (ClassNo)
	{
		// 魔法使い
		case DataList_SaveData::CLASS_WIZARD:
			Hp_IncreaseAmount = 1;
			break;

		// 盗賊
		// 商人
		case DataList_SaveData::CLASS_ROGUE:
		case DataList_SaveData::CLASS_MARCHANT:
			Hp_IncreaseAmount = 2;
			break;

		// 旅人
		// 剣士
		case DataList_SaveData::CLASS_TRAVELER:
		case DataList_SaveData::CLASS_SWORDSMAN:
			Hp_IncreaseAmount = 3;
			break;
	}
	int Hp_Max = 20 + (Level_Hp * Hp_IncreaseAmount);
	std::string HpText = std::to_string(Hp_Max);
	DrawStringToHandle(
		CenterPosX - GetDrawStringWidthToHandle(HpText.c_str(), static_cast<int>(strlenDx(HpText.c_str())), giFont_DonguriDuel_64) / 2,
		CenterPosY + 170,
		HpText.c_str(),
		GetColor(255, 255, 255),
		giFont_DonguriDuel_64
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
	CenterPosX = 430;
	CenterPosY = (SCREEN_SIZE_HEIGHT / 2) - 120;
	DrawStringToHandle(
		CenterPosX - GetDrawStringWidthToHandle("アビリティ", static_cast<int>(strlenDx("アビリティ")), giFont_DonguriDuel_32) / 2,
		CenterPosY,
		"アビリティ",
		GetColor(255, 255, 255),
		giFont_DonguriDuel_32
	);
	std::string AvillityLevelText = "レベル　";
	int Color = GetColor(255, 255, 255);
	if (this->pDataList_SaveData->GetLevel_Ability(this->pDataList_SaveData->GetPlayerClassNo()) >= 3)
	{
		AvillityLevelText += "MAX";
		Color = GetColor(255, 255, 0);
	}
	else
	{
		AvillityLevelText += std::to_string(this->pDataList_SaveData->GetLevel_Ability(this->pDataList_SaveData->GetPlayerClassNo()));
	}
	DrawStringToHandle(
		CenterPosX - GetDrawStringWidthToHandle(AvillityLevelText.c_str(), static_cast<int>(strlenDx(AvillityLevelText.c_str())), giFont_DonguriDuel_32) / 2,
		CenterPosY + 32,
		AvillityLevelText.c_str(),
		Color,
		giFont_DonguriDuel_32
	);
	/* レベルが最大でない場合、次のレベルまでの経験値を描写する */
	if (this->pDataList_SaveData->GetLevel_Ability(this->pDataList_SaveData->GetPlayerClassNo()) < 3)
	{
		int NextLevelExp = 30 + (30 * (this->pDataList_SaveData->GetLevel_Ability(this->pDataList_SaveData->GetPlayerClassNo())));
		std::string NextLevelExpText = std::to_string(NextLevelExp) + "Exp";
		DrawStringToHandle(
			CenterPosX - GetDrawStringWidthToHandle(NextLevelExpText.c_str(), static_cast<int>(strlenDx(NextLevelExpText.c_str())), giFont_DonguriDuel_32) / 2,
			CenterPosY + 32 + 96,
			NextLevelExpText.c_str(),
			GetColor(255, 255, 255),
			giFont_DonguriDuel_32
		);
	}

	/* アビリティの文字列描写 */
	std::string AvillityText;
	int ClassNo			= this->pDataList_SaveData->GetPlayerClassNo();			// クラス番号
	int AbilityLevel	= this->pDataList_SaveData->GetLevel_Ability(ClassNo);	// アビリティレベル
	switch (ClassNo)
	{
		// 旅人
		case DataList_SaveData::CLASS_TRAVELER:
			AvillityText = "スート：たびびと をもつカードのこうかはつどうじチェインすうを+" + std::to_string(AbilityLevel) + "してじっこう\nターンかいしじのドローまいすう+１";
			break;

		// 剣士
		case DataList_SaveData::CLASS_SWORDSMAN:
			AvillityText = "スート：けん をもつカードのこうかはつどうじチェインすうを+" + std::to_string(AbilityLevel) + "してじっこう";
			break;

		// 魔法使い
		case DataList_SaveData::CLASS_WIZARD:
			AvillityText = "カードタイプ：スペル のカードのこうかはつどうじチェインすうを+" + std::to_string(AbilityLevel) + "してじっこう\nバトルかいしじつかいまをしょうかん";
			break;

		// 盗賊
		case DataList_SaveData::CLASS_ROGUE:
			AvillityText = "カードタイプ：たんけん のカードしようじ、ついかで+" + std::to_string(AbilityLevel) + "かいこうげきする\nカードドロップりつＵＰ";
			break;
		
		// 商人
		case DataList_SaveData::CLASS_MARCHANT:
			AvillityText = "カードタイプ：どうぐ のカードのこうかはつどうじチェインすうを+" + std::to_string(AbilityLevel) + "してじっこう\nショップがはんがく";
			break;
	}

	CenterPosX = 630;
	CenterPosY = (SCREEN_SIZE_HEIGHT / 2) - 120;
	DrawStringToHandle(
		CenterPosX,
		CenterPosY,
		AvillityText.c_str(),
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
	CenterPosX = 200;
	CenterPosY = 900 - 110;
	DrawStringToHandle(
		CenterPosX - GetDrawStringWidthToHandle("デッキ", static_cast<int>(strlenDx("デッキ")), giFont_DonguriDuel_32) / 2,
		CenterPosY,
		"デッキ",
		GetColor(255, 255, 255),
		giFont_DonguriDuel_32
	);
	std::string DeckLevelText = "レベル　";
	int Color = GetColor(255, 255, 255);
	if (this->pDataList_SaveData->GetLevel_Deck(this->pDataList_SaveData->GetPlayerClassNo()) >= 3)
	{
		DeckLevelText += "MAX";
		Color = GetColor(255, 255, 0);
	}
	else
	{
		DeckLevelText += std::to_string(this->pDataList_SaveData->GetLevel_Deck(this->pDataList_SaveData->GetPlayerClassNo()));
	}
	DrawStringToHandle(
		CenterPosX - GetDrawStringWidthToHandle(DeckLevelText.c_str(), static_cast<int>(strlenDx(DeckLevelText.c_str())), giFont_DonguriDuel_32) / 2,
		CenterPosY + 32,
		DeckLevelText.c_str(),
		Color,
		giFont_DonguriDuel_32
	);
	/* レベルが最大でない場合、次のレベルまでの経験値を描写する */
	if (this->pDataList_SaveData->GetLevel_Deck(this->pDataList_SaveData->GetPlayerClassNo()) < 3)
	{
		int NextLevelExp = 30 + (30 * (this->pDataList_SaveData->GetLevel_Deck(this->pDataList_SaveData->GetPlayerClassNo())));
		std::string NextLevelExpText = std::to_string(NextLevelExp) + "Exp";
		DrawStringToHandle(
			CenterPosX - GetDrawStringWidthToHandle(NextLevelExpText.c_str(), static_cast<int>(strlenDx(NextLevelExpText.c_str())), giFont_DonguriDuel_32) / 2,
			CenterPosY + 32 + 96,
			NextLevelExpText.c_str(),
			GetColor(255, 255, 255),
			giFont_DonguriDuel_32
		);
	}

	/* デッキカードの描写 */
	for (int i = 0; i < DeckCardList.size(); i++)
	{
		DeckCardList[i]->Draw();
	}
}

// レベルアップ処理
void Scene_Build::Update_LevelUp()
{
	/* 現在選択中のクラスを取得 */
	int ClassNo = this->pDataList_SaveData->GetPlayerClassNo();

	/* HP強化 */
	if ((this->UI_Button[0]->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT)))
	{
		/* 経験値が足りているか確認 */
		// ※ (10 + (10 * 現在のHPレベル))の経験値が必要
		int RequiredExp = 10 + (10 * this->pDataList_SaveData->GetLevel_Hp(ClassNo));
		if (this->pDataList_SaveData->GetHaveExp() < RequiredExp)
		{
			// 足りていない場合は処理を終了
			return;
		}
		else
		{
			// 足りている場合
			/* 経験値を減らす */
			this->pDataList_SaveData->SetHaveExp(this->pDataList_SaveData->GetHaveExp() - RequiredExp);

			/* HPレベルを更新する(最大5Levl) */
			int Level_Hp = this->pDataList_SaveData->GetLevel_Hp(ClassNo);
			if (Level_Hp < 5)
			{
				this->pDataList_SaveData->SetLevel_Hp(ClassNo, Level_Hp + 1);
			}
		}
	}

	/* アビリティ強化 */
	if ((this->UI_Button[1]->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT)))
	{
		/* 経験値が足りているか確認 */
		// ※ (30 + (30 * 現在のアビリティレベル))の経験値が必要
		int RequiredExp = 30 + (30 * this->pDataList_SaveData->GetLevel_Ability(ClassNo));
		if (this->pDataList_SaveData->GetHaveExp() < RequiredExp)
		{
			// 足りていない場合は処理を終了
			return;
		}
		else
		{
			// 足りている場合
			/* 経験値を減らす */
			this->pDataList_SaveData->SetHaveExp(this->pDataList_SaveData->GetHaveExp() - RequiredExp);

			/* アビリティレベルを更新する(最大3Level) */
			int Level_Ability = this->pDataList_SaveData->GetLevel_Ability(ClassNo);
			if (Level_Ability < 3)
			{
				this->pDataList_SaveData->SetLevel_Ability(ClassNo, Level_Ability + 1);
			}
		}
	}

	/* デッキ強化 */
	if ((this->UI_Button[2]->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT)))
	{
		/* 経験値が足りているか確認 */
		// ※ (30 + (30 * 現在のデッキレベル))の経験値が必要
		int RequiredExp = 30 + (30 * this->pDataList_SaveData->GetLevel_Deck(ClassNo));
		if (this->pDataList_SaveData->GetHaveExp() < RequiredExp)
		{
			// 足りていない場合は処理を終了
			return;
		}
		else
		{
			// 足りている場合
			/* 経験値を減らす */
			this->pDataList_SaveData->SetHaveExp(this->pDataList_SaveData->GetHaveExp() - RequiredExp);
		}

		/* デッキレベルを更新する(最大い3Level) */
		int Level_Deck = this->pDataList_SaveData->GetLevel_Deck(ClassNo);
		if (Level_Deck < 3)
		{
			this->pDataList_SaveData->SetLevel_Deck(ClassNo, Level_Deck + 1);
		}
	}
}

// 所持経験値の描写
void Scene_Build::Draw_HaveExp()
{
	/* 現在の所持経験値を描写 */
	Struct_2D::POSITION CenterPos	= { 200, 290 };
	Struct_2D::POSITION IconSize	= { 170, 64 };
	DRAW_FUNCTION::DrawFrame_Image(CenterPos, IconSize, 32, *(this->Image_Frame_Corner), *(this->Image_Frame_Line), *(this->Image_Frame_Inside));

	CenterPos.iY -= 16;
	std::string HaveExpText = "けいけんち\n" + std::to_string(this->pDataList_SaveData->GetHaveExp()) + "EXP";
	DrawStringToHandle(
		CenterPos.iX - 80,
		CenterPos.iY - 16,
		HaveExpText.c_str(),
		GetColor(255, 255, 255),
		giFont_DonguriDuel_32
	);
}
