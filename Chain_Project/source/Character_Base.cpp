/* キャラクターのベースクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Character_Base.h"
// 関連クラス
#include "DataList_Image.h"

// コンストラクタ
Character_Base::Character_Base()
{
	/* 初期化 */
	this->iHP_Max			= 0;			// 体力(最大値)
	this->iHP_Now			= 0;			// 体力(現在値)
	this->Image				= nullptr;		// 画像
	this->Center_Position	= { 0, 0 };		// 中心座標
}

// 描画
void Character_Base::Draw()
{
	/* キャラクターのサイズを取得 */
	int SizeX, SizeY;
	GetGraphSize(*(this->Image), &SizeX, &SizeY);

	/* キャラクター画像の描画 */
	DrawGraph(
		this->Center_Position.iX - (SizeX / 2),
		this->Center_Position.iY - (SizeY / 2),
		*(this->Image),
		TRUE
	);
}

// 体力バー描画
void Character_Base::Draw_HPBar()
{
	/* 体力バーを描写 */
	int BarWidth = 100;
	int BarHeight = 10;
	DrawBox(
		this->Center_Position.iX - (BarWidth / 2),
		this->Center_Position.iY + 50,
		this->Center_Position.iX - (BarWidth / 2) + (BarWidth * this->iHP_Now / this->iHP_Max),
		this->Center_Position.iY + 50 + BarHeight,
		GetColor(0, 255, 0),
		TRUE
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
