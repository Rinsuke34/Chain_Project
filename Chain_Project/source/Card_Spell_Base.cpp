/* "魔法"カードのベースクラスの定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Spell_Base.h"
// 共通定義
#include "VariableDefine.h"
// 関連クラス
#include "Character_Base.h"
#include "DataList_Image.h"
#include "DataList_Battle.h"
#include "Action_Effect.h"

// コンストラクタ
Card_Spell_Base::Card_Spell_Base() : Card_Base()
{
	/* カード情報の設定 */
	this->iCardType = Card_Base::TYPE_SPELL;	// カード種類:魔法
}

// 戦闘行動
void Card_Spell_Base::BattleAction()
{
	/* スペルの処理を設定する */
	std::shared_ptr<Action_Effect_Extra> pSpellEffect = std::make_shared<Action_Effect_Extra>();
	pSpellEffect->Target_Camp		= Character_Base::CAMP_NONE;		// 効果対象の陣営:無し
	pSpellEffect->Target_Position	= 0;								// 効果対象の立ち位置:無し
	pSpellEffect->EffectUser		= this->pPlayer;					// 効果の使用者:プレイヤーキャラクター
	pSpellEffect->AllRange			= false;							// 全体効果でない
	pSpellEffect->ExEffectCard		= std::dynamic_pointer_cast<Card_Spell_Base>(shared_from_this());
	pSpellEffect->Priority			= 40;								// ちょっと低め
	this->pDataList_Battle->AddEffect(pSpellEffect, GetMyAreaNo());
}

// 背景を描写
void Card_Spell_Base::DrawBackGround()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 背景の画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_Frame/Spell/BackGround";
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
void Card_Spell_Base::DrawImage()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* イラストの画像を取得 */
	std::string ImageFilePath = "Card_Ilust/" + this->ImageName;
	std::shared_ptr<int> Image = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* イラスト描写 */
	DrawModiGraph(
		(IMAGE_SIZE_WIDTH / 2) - (SPELL_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) - (SPELL_CARD_IMAGE_HEIGHT / 2) + SPELL_ILLUSTRATION_POS_Y,
		(IMAGE_SIZE_WIDTH / 2) + (SPELL_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) - (SPELL_CARD_IMAGE_HEIGHT / 2) + SPELL_ILLUSTRATION_POS_Y,
		(IMAGE_SIZE_WIDTH / 2) + (SPELL_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) + (SPELL_CARD_IMAGE_HEIGHT / 2) + SPELL_ILLUSTRATION_POS_Y,
		(IMAGE_SIZE_WIDTH / 2) - (SPELL_CARD_IMAGE_WIDE / 2), (IMAGE_SIZE_HEIGHT / 2) + (SPELL_CARD_IMAGE_HEIGHT / 2) + SPELL_ILLUSTRATION_POS_Y,
		*(Image),
		TRUE
	);
}

// スートを描写
void Card_Spell_Base::DrawSuite()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	// 画像[0:フレーム, 1:スート]
	std::shared_ptr<int> Image[2];

	/* スートのフレームの画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_Frame/Spell/SuitFrame";
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
void Card_Spell_Base::DrawFrame()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* フレームの画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_Frame/Spell/Frame";
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
void Card_Spell_Base::DrawName()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* ネームプレートの画像を取得 */
	std::string ImageFilePath = "Card_Commoon/Card_Frame/Spell/NamePlate";
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
			GetColor(255, 255, 255),
			giFont_JF_Dot_MPlus10_20
		);
	}
}
