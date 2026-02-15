/* "クラス"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Class.h"
// 関連クラス
#include "DataList_Image.h"
#include "DataList_SaveData.h"
// 共通クラス
#include "FunctionDefine.h"
#include "VariableDefine.h"

// コンストラクタ
Card_Class::Card_Class(int ClassNo) : Card_Base()
{
	// 引数
	// ClassNo : クラスの番号

	/* 初期化 */
	this->ClassNo	= ClassNo;					// クラスの番号
	this->iCardType	= Card_Base::TYPE_CLASS;	// カード種類:クラス
	this->Name		= "";						// 名前
	this->ImageName = "";						// 画像名

	switch (this->ClassNo)
	{
		// 旅人
		case DataList_SaveData::CLASS_TRAVELER:
			this->Name		= "たびびと";
			this->ImageName	= "Icon_Traveler";
			break;

		// 剣士
		case DataList_SaveData::CLASS_SWORDSMAN:
			this->Name		= "けんし";
			this->ImageName = "Icon_Swordsman";
			break;

		// 魔法使い
		case DataList_SaveData::CLASS_WIZARD:
			this->Name		= "まほうつかい";
			this->ImageName = "Icon_Wizard";
			break;

		// 盗賊
		case DataList_SaveData::CLASS_ROGUE:
			this->Name		= "とうぞく";
			this->ImageName = "Icon_Rogue";
			break;

		// 商人
		case DataList_SaveData::CLASS_MARCHANT:
			this->Name		= "しょうにん";
			this->ImageName = "Icon_Marchant";
			break;
	}
}

// 背景を描写
void Card_Class::DrawBackGround()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 背景の画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_Frame/NextArea/BackGround";
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
void Card_Class::DrawImage()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* イラストの画像を取得 */
	std::string ImageFilePath = "Character_Ilust/Player/Icon/" + this->ImageName;
	std::shared_ptr<int> Image = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* イラスト描写 */
	DrawModiGraph(
		(IMAGE_SIZE_WIDTH / 2) - (SPELL_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) - (SPELL_CARD_IMAGE_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (SPELL_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) - (SPELL_CARD_IMAGE_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) + (SPELL_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) + (SPELL_CARD_IMAGE_HEIGHT / 2),
		(IMAGE_SIZE_WIDTH / 2) - (SPELL_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) + (SPELL_CARD_IMAGE_HEIGHT / 2),
		*(Image),
		TRUE
	);
}

// フレームを描写
void Card_Class::DrawFrame()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 背景の画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_Frame/NextArea/Frame";
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

// 名前を描写
void Card_Class::DrawName()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* ネームプレートの画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_Frame/NextArea/NamePlate";
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
		int iSizeX = GetDrawStringWidthToHandle(this->Name.c_str(), static_cast<int>(strlenDx(this->Name.c_str())), giFont_JF_Dot_MPlus10_20);
		int iSizeY = GetFontSizeToHandle(giFont_JF_Dot_MPlus10_20);

		// 文字列描写
		DrawStringToHandle(
			(IMAGE_SIZE_WIDTH / 2) - (iSizeX / 2),
			(IMAGE_SIZE_HEIGHT / 2) - (CARD_HEIGHT / 2) + NAMEPLATE_POSITION_Y - (iSizeY / 2),
			this->Name.c_str(),
			GetColor(0, 0, 0),
			giFont_JF_Dot_MPlus10_20
		);
	}
}
