/* シーン"タイトル画面"の定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Title.h"
// 共通定義
#include "FunctionDefine.h"
#include "VariableDefine.h"
// 関連クラス
#include "Scene_Build.h"
#include "DataList_Image.h"
#include "DataList_Font.h"

// コンストラクタ
Scene_Title::Scene_Title() : Scene_Base("Scene_Title", 0, false, false)
{
	/* 画像リソースの先行ロード */
	AdvanceImageLoad();

	/* フォントリソースの先行ロード */
	AdvanceFontLoad();
}

// 更新
void Scene_Title::Update()
{
	if (gstKeyboardInputData.cgInput[INPUT_TRG][KEY_INPUT_Z] == TRUE)
	{
		gpSceneServer->SetDeleteCurrentSceneFlg(true);
		LOAD_FUNCTION::AddLoadScene();
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Build>());
		return;
	}
}

// 描画
void Scene_Title::Draw()
{
//	DrawString(16, 16, "Title", GetColor(255, 255, 255));
	DrawStringToHandle(16, 16, "タイトル", GetColor(255, 0, 0), giFont_DonguriDuel);
//	DrawExtendGraph(0, int y1, int x2, int y2, int GrHandle, int TransFlag);
//	DrawGraph(0, 0, *(this->Image_TitleLogo), TRUE);
}

// 画像リソースの先行ロード
void Scene_Title::AdvanceImageLoad()
{
	/* 非同期で使用予定の画像を事前に読み込む */

	/* データリスト取得 */
	// 画像管理データリスト
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 画像データ読み込み */
	// タイトルロゴ
	std::string ImageFilePath = "Logo/Title";
	pDataList_Image->LoadImageHandle_ASync(ImageFilePath);
	this->Image_TitleLogo = pDataList_Image->iGetImageHandle(ImageFilePath);
}

// フォントリソースの先行ロード
void Scene_Title::AdvanceFontLoad()
{
	/* 非同期で使用予定のフォントを事前に読み込む */

	/* データリスト取得 */
	// フォント管理データリスト
	std::shared_ptr<DataList_Font> pDataList_Font = std::dynamic_pointer_cast<DataList_Font>(gpDataListServer->GetDataList("DataList_Font"));

	/* フォントデータ読み込み */
	// どんぐりデュエル
	std::string FontFilePath = "DonguriDuel";
	pDataList_Font->LoadFontHandle_ASync(FontFilePath, 0);
	giFont_DonguriDuel = pDataList_Font->iGetFontHnadle(FontFilePath);
}
