/* "武具"カードのベースクラスの定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_Base.h"
// 共通定義
#include "VariableDefine.h"
// 関連クラス
#include "DataList_Image.h"

// コンストラクタ
Card_Arms_Base::Card_Arms_Base() : Card_Base()
{
	/* カード情報の設定 */
	this->iCardType = Card_Base::TYPE_ARMS;	// カード種類:武具
}

// 背景を描写
void Card_Arms_Base::DrawBackGround()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 背景の画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_Frame/Arms/BackGround";
	std::shared_ptr<int> Image = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* 背景描写 */
	DrawModiGraph(
		(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
		*(Image),
		TRUE
	);
}

// イラストを描写
void Card_Arms_Base::DrawImage()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* イラストの画像を取得 */
	std::string ImageFilePath = "Card_Ilust/" + this->ImageName;
	std::shared_ptr<int> Image = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* イラスト描写 */
	DrawModiGraph(
		(IMAGE_SIZE_WIDTH / 2) - (ARMS_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) - (ARMS_CARD_IMAGE_HEIGHT / 2) + ARMS_ILLUSTRATION_POS_Y,
		(IMAGE_SIZE_WIDTH / 2) + (ARMS_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) - (ARMS_CARD_IMAGE_HEIGHT / 2) + ARMS_ILLUSTRATION_POS_Y,
		(IMAGE_SIZE_WIDTH / 2) + (ARMS_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) + (ARMS_CARD_IMAGE_HEIGHT / 2) + ARMS_ILLUSTRATION_POS_Y,
		(IMAGE_SIZE_WIDTH / 2) - (ARMS_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) + (ARMS_CARD_IMAGE_HEIGHT / 2) + ARMS_ILLUSTRATION_POS_Y,
		*(Image),
		TRUE
	);

	/* 攻撃力を描写 */
	std::string Attack		= std::to_string(this->Strength);
	
	// 文字列の高さ、幅を取得
	int iSizeX = GetDrawStringWidthToHandle(this->Name.c_str(), static_cast<int>(strlenDx(Attack.c_str())), giFont_Cp_Period_16);
	int iSizeY = GetFontSizeToHandle(giFont_Cp_Period_16);

	// 文字列描写
	DrawStringToHandle(
		(IMAGE_SIZE_WIDTH / 2) - (iSizeX / 2) + ARMS_ATTACK_X_POS,
		(IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + ARMS_STATUS_Y_POS - (iSizeY / 2),
		Attack.c_str(),
		GetColor(255, 255, 255),
		giFont_Cp_Period_16
	);


	/* 防御力を描写 */
	std::string Diffence = std::to_string(this->Diffence);

	// 文字列の高さ、幅を取得
	iSizeX = GetDrawStringWidthToHandle(this->Name.c_str(), static_cast<int>(strlenDx(Diffence.c_str())), giFont_Cp_Period_16);

	// 文字列描写
	DrawStringToHandle(
		(IMAGE_SIZE_WIDTH / 2) - (iSizeX / 2) + ARMS_DIFFENSE_X_POS,
		(IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + ARMS_STATUS_Y_POS - (iSizeY / 2),
		Diffence.c_str(),
		GetColor(255, 255, 255),
		giFont_Cp_Period_16
	);
}

// スートを描写
void Card_Arms_Base::DrawSuite()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	// 画像[0:フレーム, 1:スート]
	std::shared_ptr<int> Image[2];

	/* スートのフレームの画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_Frame/Arms/SuitFrame";
	Image[0] = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* スートの画像を取得し、描写する */
	for (int i = 0; i < this->Suite_List.size(); i++)
	{
		/* スート画像を取得 */
		ImageFilePath = "Card_Suit/" + this->Suite_List[i];
		Image[1] = pDataList_Image->iGetImageHandle(ImageFilePath);

		/* 描写 */
		for (int x = 0; x < 2; x++)
		{
			DrawModiGraph(
				(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2) + (i * SUTE_SIZE_WIDTH) + SUTE_START_POS_X, (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + SUTE_POS_Y,
				(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2) + ((i + 1) * SUTE_SIZE_WIDTH) + SUTE_START_POS_X, (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + SUTE_POS_Y,
				(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2) + ((i + 1) * SUTE_SIZE_WIDTH) + SUTE_START_POS_X, (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + SUTE_POS_Y + SUTE_SIZE_HEIGHT,
				(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2) + (i * SUTE_SIZE_WIDTH) + SUTE_START_POS_X, (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + SUTE_POS_Y + SUTE_SIZE_HEIGHT,
				*(Image[x]),
				TRUE
			);
		}
	}
}

// フレームを描写
void Card_Arms_Base::DrawFrame()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* フレームの画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_Frame/Arms/Frame";
	std::shared_ptr<int> Image = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* フレームサイズ取得 */
	int SizeX, SizeY;
	GetGraphSize(*(Image), &SizeX, &SizeY);

	/* フレーム描写 */
	DrawModiGraph(
		(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) - (CARD_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) + (CARD_HEIGHT / 2),
		*(Image),
		TRUE
	);
}

// 名前を描写
void Card_Arms_Base::DrawName()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* ネームプレートの画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_Frame/Arms/NamePlate";
	std::shared_ptr<int> Image = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* カード名のフレーム描写 */
	DrawModiGraph(
		(IMAGE_SIZE_WIDTH / 2) - (NAMEPLATE_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + NAMEPLATE_POSITION_Y + (NAMEPLATE_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (NAMEPLATE_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + NAMEPLATE_POSITION_Y + (NAMEPLATE_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (NAMEPLATE_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + NAMEPLATE_POSITION_Y - (NAMEPLATE_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) - (NAMEPLATE_WIDTH / 2), (IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + NAMEPLATE_POSITION_Y - (NAMEPLATE_HEIGHT / 2),
		*(Image),
		TRUE
	);

	/* カード名描写 */
	{
		// 文字列の高さ、幅を取得
		int iSizeX = GetDrawStringWidthToHandle(this->Name.c_str(), static_cast<int>(strlenDx(this->Name.c_str())), giFont_Cp_Period_16);
		int iSizeY = GetFontSizeToHandle(giFont_Cp_Period_16);

		// 文字列描写
		DrawStringToHandle(
			(IMAGE_SIZE_WIDTH / 2) - (iSizeX / 2),
			(IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + NAMEPLATE_POSITION_Y - (iSizeY / 2),
			this->Name.c_str(),
			GetColor(255, 255, 255),
			giFont_Cp_Period_16
		);
	}
}
