/* "移動先エリア"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_NextArea.h"
// 共通定義
#include "VariableDefine.h"
// 関連クラス
#include "DataList_Image.h"

// コンストラクタ
Card_NextArea::Card_NextArea(int NextAreaType) : Card_Base()
{
	// 引数
	// NextAreaType : 移動先エリアの種類

	/* 初期化 */
	this->NextAreaType		= NextAreaType;					// 移動先エリアの種類
	this->iCardType			= Card_Base::TYPE_NEXT_AREA;	// カード種類:移動先エリア
	this->pNextAreaNode		= nullptr;						// 移動先エリアのノード
	this->WoldMapMoveEndFlg	= false;						// ワールドマップの移動完了フラグ

	/* 移動先エリアに応じた初期化 */
	switch (this->NextAreaType)
	{
		// 敵
		case TYPE_ENEMY:
			this->Name		= "BATTLE";
			this->ImageName	= "Enemy";
			break;

		// ショップ
		case TYPE_SHOP:
			this->Name		= "SHOP";
			this->ImageName	= "Shop";
			break;

		// ボス
		case TYPE_BOSS:
			this->Name		= "BOSS";
			this->ImageName	= "Boss";
			break;
	}
}

// 描画
void Card_NextArea::Draw()
{
	/* マップの移動が完了しているなら親クラスの描画処理を実行 */
	if (this->WoldMapMoveEndFlg)
	{
		Card_Base::Draw();
	}
}

// 更新処理
void Card_NextArea::Update()
{
	/* マップの移動が完了しているなら親クラスの更新処理を実行 */
	if (this->WoldMapMoveEndFlg)
	{
		Card_Base::Update();
	}
}

// 背景を描写
void Card_NextArea::DrawBackGround()
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
void Card_NextArea::DrawImage()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* イラストの画像を取得 */
	std::string ImageFilePath = "WoldMap_Node/" + this->ImageName;
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
void Card_NextArea::DrawFrame()
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
void Card_NextArea::DrawName()
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
