/* シーン"ショップ画面"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Shop.h"
// 共通定義
#include "VariableDefine.h"
// 関連クラス
#include "DataList_GameResource.h"
#include "Scene_UI_Button.h"

// コンストラクタ
Scene_Shop::Scene_Shop() : Scene_Base("Scene_Shop", 10, false, false)
{
	/* 初期化 */
	this->ShopEndFlg	= false;		// ショップ終了フラグ

	/* データリスト取得 */
	// ゲームリソース管理用データリスト
	this->pDataList_GameResource = std::dynamic_pointer_cast<DataList_GameResource>(gpDataListServer->GetDataList("DataList_GameResource"));

	/* UI作成 */
	{
		/* "決定"ボタンの作成 */
		this->UI_DecisionButton = std::make_shared<Scene_UI_Button>("Battle_DecisionButton", 1);
		this->UI_DecisionButton->SetButtonText("しゅうりょう");
		this->UI_DecisionButton->SetCenterPos({ SCREEN_SIZE_WIDE / 2, SCREEN_SIZE_HEIGHT / 2 });
		this->UI_DecisionButton->SetFontHandle(giFont_DonguriDuel_32);
		gpSceneServer->AddSceneReservation(this->UI_DecisionButton);
	}
}

// デストラクタ
Scene_Shop::~Scene_Shop()
{
	/* 紐づいたUIを削除 */
	this->UI_DecisionButton->SetDeleteFlg(true);
	this->UI_DecisionButton = nullptr;
}

// 更新
void Scene_Shop::Update()
{
	/* ショップ終了フラグを確認 */
	if(this->ShopEndFlg == false)
	{
		// 無効であるならば
		/* "決定"ボタンが入力されたならば */
		if (this->UI_DecisionButton->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
		{
			/* ワールドマップを有効にする */
			this->pDataList_GameResource->SetWoldMapActiveFlg(true);

			/* ショップ終了フラグを有効化 */
			this->ShopEndFlg = true;
		}
	}
	else
	{
		// 有効であるならば
		/* 次のステージの選択が完了しているならこのシーンを削除 */
		if (this->pDataList_GameResource->GetNextStageSelectFlg())
		{
			/* このシーンの削除フラグを有効にする */
			this->bDeleteFlg = true;

			/* 次のステージ選択を未完了に戻す */
			this->pDataList_GameResource->SetNextStageSelectedFlg(false);
		}
	}
}

// 描画
void Scene_Shop::Draw()
{

}
