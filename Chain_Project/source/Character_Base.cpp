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
	this->iShield_Now		= 0;			// シールド(現在値)
	this->Image				= nullptr;		// 画像
	this->BasePos			= { 0, 0 };		// 基準座標
	this->Camp				= -1;			// 陣営
	this->SizeX				= -1;			// キャラクターの幅
	this->SizeY				= -1;			// キャラクターの高さ
}

// 描画
void Character_Base::Draw()
{
	/* キャラクターのサイズが設定されていないなら画像サイズを算出 */
	if (this->SizeX == -1 || this->SizeY == -1)
	{
		GetGraphSize(*(this->Image), &this->SizeX, &this->SizeY);
	}

	/* キャラクター画像の描画 */
	DrawModiGraph(
		this->BasePos.iX - (this->SizeX / 2), this->BasePos.iY - (this->SizeY),
		this->BasePos.iX + (this->SizeX / 2), this->BasePos.iY - (this->SizeY),
		this->BasePos.iX + (this->SizeX / 2), this->BasePos.iY,
		this->BasePos.iX - (this->SizeX / 2), this->BasePos.iY,
		*(this->Image),
		TRUE
	);

	/* 体力バー描画 */
	Draw_HPBar();
}

// 体力バー描画
void Character_Base::Draw_HPBar()
{
	/* 体力バーの描写中心座標を設定 */
	Struct_2D::POSITION HPBar_CenterPos = {
		this->BasePos.iX,
		this->BasePos.iY - this->SizeY - HPBAR_UPPER
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
	int iSizeX = GetDrawStringWidthToHandle(HPText.c_str(), static_cast<int>(strlenDx(HPText.c_str())), giFont_Cp_Period_16);
	int iSizeY = GetFontSizeToHandle(giFont_Cp_Period_16);

	/* 残り体力の文字列描写 */
	DrawStringToHandle(
		HPBar_CenterPos.iX - iSizeX / 2,
		HPBar_CenterPos.iY - iSizeY / 2,
		HPText.c_str(),
		GetColor(255, 255, 255),
		giFont_Cp_Period_16
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

// ダメージ処理
void Character_Base::Damage(int DamageAmount)
{
	// 引数
	// DamageAmount : ダメージ量

	/* シールドでダメージを軽減 */
	if (this->iShield_Now > 0)
	{
		// シールドがある場合
		/* シールドでダメージを軽減 */
		if (DamageAmount <= this->iShield_Now)
		{
			// ダメージ量がシールド以下の場合
			this->iShield_Now -= DamageAmount;
			DamageAmount = 0;
		}
		else
		{
			// ダメージ量がシールド以上の場合
			DamageAmount -= this->iShield_Now;
			this->iShield_Now = 0;
		}
	}

	/* 体力からダメージを減算 */
	this->iHP_Now -= DamageAmount;

	/* 体力が0以下になった場合、0に設定 */
	if (this->iHP_Now < 0)
	{
		this->iHP_Now = 0;
	}
}

// シールドリセット(行動終了時)
void Character_Base::ShieldReset_EndAction()
{
	/* シールドを0に設定 */
	this->iShield_Now = 0;
}

// シールドリセット(ターン終了時)
void Character_Base::ShieldReset_EndTurn()
{
	/* シールドを0に設定 */
	this->iShield_Now = 0;
}