/* シーン"ゲームオーバー画面"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_GameOver.h"
// 共通定義
#include "VariableDefine.h"
// 関連クラス
#include "DataList_GameResource.h"
#include "Scene_UI_Button.h"

// コンストラクタ
Scene_GameOver::Scene_GameOver() : Scene_Base("Scene_GameOver", 10, false, false)
{
	/* 初期化 */
	this->GameEndFlg = false;		// ゲーム終了フラグ

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
Scene_GameOver::~Scene_GameOver()
{
	/* 紐づいたUIを削除 */
	this->UI_DecisionButton->SetDeleteFlg(true);
	this->UI_DecisionButton = nullptr;

	/* データリスト"ゲームリソース管理"を削除 */
	gpDataListServer->DeleteDataList("DataList_GameResource");
}

// 更新
void Scene_GameOver::Update()
{
	/* ゲーム終了フラグを確認 */
	if (this->GameEndFlg == false)
	{
		// 無効であるならば
		/* "決定"ボタンが入力されたならば */
		if (this->UI_DecisionButton->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
		{
			///* ワールドマップを有効にする */
			//this->pDataList_GameResource->SetWoldMapActiveFlg(true);

			/* ゲーム終了フラグを有効化 */
			this->GameEndFlg = true;
		}
	}
	else
	{
		// 有効であるならば

		/* ゲーム終了フラグを有効にする */
		this->pDataList_GameResource->SetGameEndFlg(true);
	}
}

// 描画
void Scene_GameOver::Draw()
{

}
