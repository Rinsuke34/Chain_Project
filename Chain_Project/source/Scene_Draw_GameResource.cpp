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
Scene_Draw_GameResource::Scene_Draw_GameResource() : Scene_Base("Scene_Draw_GameResource", 9, false, false)
{
	/* 初期化 */
	// データリスト取得
	this->pDataList_GameResource = std::dynamic_pointer_cast<DataList_GameResource>(gpDataListServer->GetDataList("DataList_GameResource"));
	// 画像取得
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
}

// 描画
void Scene_Draw_GameResource::Draw()
{
	/* コインアイコンの枠の描写 */
	Struct_2D::POSITION CenterPos		= { 78,		30 };
	Struct_2D::POSITION	IconSize		= { 128,	32 };
	int					Frame_Thickness	= 8;
	DRAW_FUNCTION::DrawFrame_Image(CenterPos, IconSize, Frame_Thickness, *(this->Image_Frame_Corner), *(this->Image_Frame_Line), *(this->Image_Frame_Inside));

	/* コインアイコンの描写 */
	DrawModiGraph(
		14,	14,
		46,	14,
		46,	46,
		14,	46,
		*(this->Image_Coin),
		TRUE
	);

	/* 文字列の描写 */
	std::string CoinString = " × " + std::to_string(this->pDataList_GameResource->GetHaveCoin());
	DrawStringToHandle(
		46,
		12,
		CoinString.c_str(),
		GetColor(0, 0, 0),
		giFont_DonguriDuel_32);
}
