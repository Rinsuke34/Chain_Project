/* キャラクターのベースクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Base.h"
// 共通定義
#include "VariableDefine.h"
// 関連クラス
#include "DataList_Image.h"

// コンストラクタ
Character_Base::Character_Base()
{
	/* 初期化 */
	this->iHP_Max			= 0;			// 体力(最大値)
	this->iHP_Now			= 0;			// 体力(現在値)
	this->Image				= nullptr;		// 画像
	this->BasePos			= { 0, 0 };		// 基準座標
}

// 描画
void Character_Base::Draw()
{
	/* キャラクターのサイズを取得 */
	int SizeX, SizeY;
	GetGraphSize(*(this->Image), &SizeX, &SizeY);

	/* キャラクター画像の描画 */
	DrawGraph(
		this->BasePos.iX - (SizeX / 2),
		this->BasePos.iY - (SizeY),
		*(this->Image),
		TRUE
	);

	/* 体力バー描画 */
	Draw_HPBar();
}

// 体力バー描画
void Character_Base::Draw_HPBar()
{
	/* キャラクターのサイズを取得 */
	int SizeX, SizeY;
	GetGraphSize(*(this->Image), &SizeX, &SizeY);

	/* 体力バーの描写中心座標を設定 */
	Struct_2D::POSITION HPBar_CenterPos = {
		this->BasePos.iX,
		this->BasePos.iY - SizeY - HPBAR_UPPER
	};

	/* 体力バーの背景描写 */
	DrawBox(
		HPBar_CenterPos.iX - (HPBAR_WIDE / 2),
		HPBar_CenterPos.iY - (HPBAR_HEIGHT / 2),
		HPBar_CenterPos.iX + (HPBAR_WIDE / 2),
		HPBar_CenterPos.iY + (HPBAR_HEIGHT / 2),
		GetColor(0, 0, 0),
		TRUE
	);

	/* 体力バーの残り体力描写 */
	DrawBox(
		HPBar_CenterPos.iX - (HPBAR_WIDE / 2),
		HPBar_CenterPos.iY - (HPBAR_HEIGHT / 2),
		HPBar_CenterPos.iX - (HPBAR_WIDE / 2) + (HPBAR_WIDE * this->iHP_Now / this->iHP_Max),
		HPBar_CenterPos.iY + (HPBAR_HEIGHT / 2),
		GetColor(255, 0, 0),
		TRUE
	);

	/* 残り体力を文字列に設定 */
	std::string HPText = std::to_string(this->iHP_Now) + " / " + std::to_string(this->iHP_Max);

	/* 文字列の高さ、幅を取得 */
	int iSizeX = GetDrawStringWidthToHandle(HPText.c_str(), static_cast<int>(strlenDx(HPText.c_str())), giFont_DonguriDuel_16);
	int iSizeY = GetFontSizeToHandle(giFont_DonguriDuel_16);

	/* ボタンテキスト描写 */
	DrawStringToHandle(
		HPBar_CenterPos.iX - iSizeX / 2,
		HPBar_CenterPos.iY - iSizeY / 2,
		HPText.c_str(),
		GetColor(255, 255, 255),
		giFont_DonguriDuel_16
	);
}

// 指定の名称の画像を設定する
void Character_Base::SetUpImage(std::string ImageName)
{
	// 引数
	// ImageName : 画像名称

	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 指定の画像を読み込む */
	this->Image = pDataList_Image->iGetImageHandle(ImageName);
}
