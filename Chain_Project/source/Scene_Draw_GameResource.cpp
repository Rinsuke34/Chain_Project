/* シーン"リソース描写"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Draw_GameResource.h"
// 共通定義
#include "FunctionDefine.h"
#include "VariableDefine.h"
// 関連クラス
#include "DataList_GameResource.h"
#include "DataList_Image.h"

// コンストラクタ
Scene_Draw_GameResource::Scene_Draw_GameResource() : Scene_Base("Scene_Draw_GameResource", 60, false, false)
{
	/* データリスト取得*/
	this->pDataList_GameResource = std::dynamic_pointer_cast<DataList_GameResource>(gpDataListServer->GetDataList("DataList_GameResource"));

	/* 初期化 */
	this->CoinScaleUp	= 0;											// コインアイコンのスケールアップ量
	this->Coin_Old		= this->pDataList_GameResource->GetHaveCoin();	// 変更前のコイン枚数

	/* 画像取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));
	// コイン
	std::string FineName = "Common_Icon/Icon_Coin";
	this->Image_Coin = pDataList_Image->iGetImageHandle(FineName);
	// フレーム画像
	FineName = "UI/Button/Button_Frame_Corner_Over";
	this->Image_Frame_Corner = pDataList_Image->iGetImageHandle(FineName);
	FineName = "UI/Button/Button_Frame_Line_Over";
	this->Image_Frame_Line = pDataList_Image->iGetImageHandle(FineName);
	FineName = "UI/Button/Button_Frame_Inside_Over";
	this->Image_Frame_Inside = pDataList_Image->iGetImageHandle(FineName);
	// カード
	FineName = "Common_Icon/Icon_Card";
	this->Image_Card = pDataList_Image->iGetImageHandle(FineName);
}

// 更新
void Scene_Draw_GameResource::Update()
{
	/* コインの枚数に変化があるか確認 */
	int Coin_Now = this->pDataList_GameResource->GetHaveCoin();
	if (Coin_Now != this->Coin_Old)
	{
		// 変化があるならば
		/* コインアイコンのスケールアップ量を設定 */
		this->CoinScaleUp = 5;		
	}
	else
	{
		// 変化がないならば
		/* スケールを通常に戻す */
		if (this->CoinScaleUp > 0)
		{
			this->CoinScaleUp--;
		}
	}

	/* 変更前のコイン枚数を更新 */
	this->Coin_Old = Coin_Now;

	/* ドロップしたカードの枚数に変化があるか確認 */
	int Card_Now = static_cast<int>(this->pDataList_GameResource->GetDropCardList().size());
	if (Card_Now != this->Card_Old)
	{
		// 変化があるならば
		/* カードアイコンのスケールアップ量を設定 */
		this->CardScaleUp = 5;
	}
	else
	{
		// 変化がないならば
		/* スケールを通常に戻す */
		if (this->CardScaleUp > 0)
		{
			this->CardScaleUp--;
		}
	}

	/* 変更前のカード枚数を更新 */
	this->Card_Old = Card_Now;
}

// 描画
void Scene_Draw_GameResource::Draw()
{
	/* コイン枚数の描写 */
	{
		/* コインアイコンの枠の描写 */
		Struct_2D::POSITION CenterPos		= { 78,		30 };
		Struct_2D::POSITION	IconSize		= { 128,	32 };
		int					Frame_Thickness	= 8;
		DRAW_FUNCTION::DrawFrame_Image(CenterPos, IconSize, Frame_Thickness, *(this->Image_Frame_Corner), *(this->Image_Frame_Line), *(this->Image_Frame_Inside));
	
		/* コインアイコンの描写 */
		DrawModiGraph(
			COIN_POS_X_MINUS	- this->CoinScaleUp,	COIN_POS_Y_MINUS	- this->CoinScaleUp,
			COIN_POS_X_PLUS		+ this->CoinScaleUp,	COIN_POS_Y_MINUS	- this->CoinScaleUp,
			COIN_POS_X_PLUS		+ this->CoinScaleUp,	COIN_POS_Y_PLUS		+ this->CoinScaleUp,
			COIN_POS_X_MINUS	- this->CoinScaleUp,	COIN_POS_Y_PLUS		+ this->CoinScaleUp,
			*(this->Image_Coin),
			TRUE
		);
	
		/* 文字列の描写 */
		// "×"の部分
		std::string Kakeru = " × ";
		DrawStringToHandle(
			46,
			12,
			Kakeru.c_str(),
			GetColor(0, 0, 0),
			giFont_DonguriDuel_32);
	
		// 所持数
		std::string HaveCoin = std::to_string(this->pDataList_GameResource->GetHaveCoin());
		DrawStringToHandle(
			46 + GetDrawStringWidthToHandle(Kakeru.c_str(), static_cast<int>(strlenDx(Kakeru.c_str())), giFont_DonguriDuel_32),
			12 - this->CoinScaleUp,
			HaveCoin.c_str(),
			GetColor(0, 0, 0),
			giFont_DonguriDuel_32);
	}

	/* ドロップしたカード枚数の描写 */
	{
		/* カードアイコンの枠の描写 */
		Struct_2D::POSITION CenterPos	= { 226,	30 };
		Struct_2D::POSITION	IconSize	= { 128,	32 };
		int					Frame_Thickness = 8;
		DRAW_FUNCTION::DrawFrame_Image(CenterPos, IconSize, Frame_Thickness, *(this->Image_Frame_Corner), *(this->Image_Frame_Line), *(this->Image_Frame_Inside));

		/* カードアイコンの描写 */
		DrawModiGraph(
			CARD_POS_X_MINUS	- this->CardScaleUp, CARD_POS_Y_MINUS	- this->CardScaleUp,
			CARD_POS_X_PLUS		+ this->CardScaleUp, CARD_POS_Y_MINUS	- this->CardScaleUp,
			CARD_POS_X_PLUS		+ this->CardScaleUp, CARD_POS_Y_PLUS	+ this->CardScaleUp,
			CARD_POS_X_MINUS	- this->CardScaleUp, CARD_POS_Y_PLUS	+ this->CardScaleUp,
			*(this->Image_Card),
			TRUE
		);

		/* 文字列の描写 */
		// "×"の部分
		std::string Kakeru = " × ";
		DrawStringToHandle(
			46 + 148,
			12,
			Kakeru.c_str(),
			GetColor(0, 0, 0),
			giFont_DonguriDuel_32);

		// 所持数
		std::string HaveCoin = std::to_string(this->pDataList_GameResource->GetDropCardList().size());
		DrawStringToHandle(
			46 + 148 + GetDrawStringWidthToHandle(Kakeru.c_str(), static_cast<int>(strlenDx(Kakeru.c_str())), giFont_DonguriDuel_32),
			12 - this->CardScaleUp,
			HaveCoin.c_str(),
			GetColor(0, 0, 0),
			giFont_DonguriDuel_32);
	}
}
