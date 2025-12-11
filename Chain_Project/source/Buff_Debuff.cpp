/* ゲーム内でのキャラクターのバフ、デバフの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Buff_Debuff.h"
// 関連クラス
#include "DataList_Image.h"

/* バフ、デバフ関連 */
/* ベース */
// コンストラクタ
Character_Buff_Debuff_Base::Character_Buff_Debuff_Base()
{
	/* 初期化 */
	this->Name				= "";		// バフ、デバフの名称
	this->Buff_Debuff_Time	= 0;		// バフ、デバフの残りターン数
	this->DeleteFlag		= false;	// 削除フラグ
	this->Image				= nullptr;	// 画像
}

// 更新処理
void Character_Buff_Debuff_Base::Update()
{
	/* 残りターン数を1減らす */
	this->Buff_Debuff_Time--;

	/* 残りターン数が0であるなら削除フラグを立てる */
	if (this->Buff_Debuff_Time <= 0)
	{
		this->DeleteFlag = true;
	}
}

/* 攻撃力バフ */
// コンストラクタ
Character_Buff_Strength::Character_Buff_Strength()
{
	/* 初期化 */
	this->Name = "Buff_Strength";		// バフ、デバフの名称
	this->Strength_Buff_Amount	= 0;	// 攻撃力バフ量

	/* 画像取得 */
	// 画像管理データリストを取得
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	// 背景の画像を取得
	std::string ImageFilePath = "Buff_Debuff_Icon/ATK_UP";
	this->Image = pDataList_Image->iGetImageHandle(ImageFilePath);
}

/* 防御力バフ */
// コンストラクタ
Character_Buff_Diffence::Character_Buff_Diffence()
{
	/* 初期化 */
	this->Name	= "Buff_Diffence";		// バフ、デバフの名称
	this->Diffence_Buff_Amount	= 0;	// 防御力バフ量

	/* 画像取得 */
	// 画像管理データリストを取得
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	// 背景の画像を取得
	std::string ImageFilePath	= "Buff_Debuff_Icon/DEF_UP";
	this->Image	= pDataList_Image->iGetImageHandle(ImageFilePath);
}

/* 毒状態 */
// コンストラクタ
Character_Debuff_Poison::Character_Debuff_Poison()
{
	/* 初期化 */
	this->Name = "Debuff_Poison";		// バフ、デバフの名称

	/* 画像取得 */
	// 画像管理データリストを取得
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	// 背景の画像を取得
	std::string ImageFilePath = "Buff_Debuff_Icon/Poison";
	this->Image = pDataList_Image->iGetImageHandle(ImageFilePath);
}
