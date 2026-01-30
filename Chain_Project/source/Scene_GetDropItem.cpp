/* シーン"ドロップアイテム取得"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_GetDropItem.h"
// 関連クラス
#include "DataList_GameResource.h"
#include "DataList_Image.h"
#include "Card_Base.h"
#include "Scene_UI_Button.h"
#include "Scene_UI_Button.h"
// 共通定義
#include "FunctionDefine.h"
#include "VariableDefine.h"

// コンストラクタ
Scene_GetDropItem::Scene_GetDropItem() : Scene_Base("Scene_GetDropItem", 9, false, false)
{
	/* 初期化 */
	this->SceneGetDropItemDrawPos	= { 0, DROPITEM_DRAWPOS_Y_LOW };	// ワールドマップの描写座標
	this->OldActiveFlg				= false;							// 以前のドロップアイテム確認シーン有効フラグ
	// 画像
	this->Image_SceneGetDropItem	= MakeScreen(DROPITEM_DRAW_WIDTH, DROPITEM_DRAW_HEIGHT, TRUE);	// ワールドマップの画像

	/* データリスト */
	// ゲームリソース管理用データリスト
	this->pDataList_GameResource = std::dynamic_pointer_cast<DataList_GameResource>(gpDataListServer->GetDataList("DataList_GameResource"));
}

// 更新
void Scene_GetDropItem::Update()
{
	/* 描写座標の更新 */
	Update_DrawPos();

	/* "決定"ボタンが入力されたのならドロップアイテム確認シーンを無効化 */
	if (this->UI_DecisionButton != nullptr)
	{
		if (this->UI_DecisionButton->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
		{
			this->pDataList_GameResource->SetDropItemCheckFlg(false);
		}
	}

	/* ドロップアイテム確認シーンが無効→有効と変化しているか確認 */
	ActiveCheck();

	/* 画像の更新 */
	Update_Image();
}

// セットアップ
void Scene_GetDropItem::Setup()
{
	/* 現在設定されているカードリストを初期化 */
	this->GetCardList.clear();
	
	/* ドロップしたカードリストを設定 */
	for (auto& Card : this->pDataList_GameResource->GetDropCardList())
	{
		this->GetCardList.push_back(Card);
	}

	/* 描写座標を初期化 */
	this->SceneGetDropItemDrawPos.iY = DROPITEM_DRAWPOS_Y_LOW;

	/* "決定"ボタンの作成 */
	this->UI_DecisionButton = std::make_shared<Scene_UI_Button>("Battle_DecisionButton", 10);
	this->UI_DecisionButton->SetButtonText("けってい");
	this->UI_DecisionButton->SetCenterPos({ 1750, SceneGetDropItemDrawPos.iY + (DROPITEM_DRAW_HEIGHT / 2) });
	this->UI_DecisionButton->SetFontHandle(giFont_DonguriDuel_32);
	gpSceneServer->AddSceneReservation(this->UI_DecisionButton);
}

// 描画
void Scene_GetDropItem::Draw()
{
	/* ドロップアイテム取得画面の描画 */
	SceneGetDropItem_Drow();
}

// 描写座標の更新
void Scene_GetDropItem::Update_DrawPos()
{
	/* ドロップアイテム確認シーンが有効であるか確認 */
	if (this->pDataList_GameResource->GetDropItemCheckFlg())
	{
		// 有効である場合
		/* 最大Y座標になるまで描写座標を上昇させる */
		if (this->SceneGetDropItemDrawPos.iY > DROPITEM_DRAWPOS_Y_MAX)
		{
			this->SceneGetDropItemDrawPos.iY -= 20;
			if (this->SceneGetDropItemDrawPos.iY < DROPITEM_DRAWPOS_Y_MAX)
			{
				this->SceneGetDropItemDrawPos.iY = DROPITEM_DRAWPOS_Y_MAX;
			}
		}

		/* "決定"ボタンの座標更新 */
		if (this->UI_DecisionButton != nullptr)
		{
			this->UI_DecisionButton->SetCenterPos({ 1750, SceneGetDropItemDrawPos.iY + (DROPITEM_DRAW_HEIGHT / 2) });
		}
	}
	else
	{
		// 無効である場合
		/* 最低Y座標になるまで描写座標を下降させる */
		if (this->SceneGetDropItemDrawPos.iY < DROPITEM_DRAWPOS_Y_LOW)
		{
			this->SceneGetDropItemDrawPos.iY += 20;
			if (this->SceneGetDropItemDrawPos.iY > DROPITEM_DRAWPOS_Y_LOW)
			{
				this->SceneGetDropItemDrawPos.iY = DROPITEM_DRAWPOS_Y_LOW;
			}
		}
	}
}

// ドロップアイテム取得画面の描画
void Scene_GetDropItem::SceneGetDropItem_Drow()
{
	DrawGraph(this->SceneGetDropItemDrawPos.iX, this->SceneGetDropItemDrawPos.iY, this->Image_SceneGetDropItem, TRUE);
}

// 画像の更新
void Scene_GetDropItem::Update_Image()
{
	/* 描写先をドロップアイテム取得画面に設定 */
	SetDrawScreen(this->Image_SceneGetDropItem);

	/* 画像をクリア */
	ClearDrawScreen();

	/* 背景描写 */
	BackGround_Drow();

	/* 描写先を裏画面に戻す */
	SetDrawScreen(DX_SCREEN_BACK);
}

// 背景描写
void Scene_GetDropItem::BackGround_Drow()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 背景の枠の画像を取得 */
	std::string ImageFilePath = "UI/Button/Button_Frame_Corner_Over";
	std::shared_ptr<int> Image_Frame_Corner = pDataList_Image->iGetImageHandle(ImageFilePath);
	ImageFilePath = "UI/Button/Button_Frame_Line_Over";
	std::shared_ptr<int> Image_Frame_Line = pDataList_Image->iGetImageHandle(ImageFilePath);
	ImageFilePath = "UI/Button/Button_Frame_Inside_Over";
	std::shared_ptr<int> Image_Frame_Inside = pDataList_Image->iGetImageHandle(ImageFilePath);

	/* 背景、フレームの描写 */
	DRAW_FUNCTION::DrawFrame_Image(
		{ DROPITEM_DRAW_WIDTH / 2, DROPITEM_DRAW_HEIGHT / 2 },
		{ DROPITEM_DRAW_WIDTH - (DROPITEM_FRAME_THICKNESS * 2), DROPITEM_DRAW_HEIGHT - (DROPITEM_FRAME_THICKNESS * 2) },
		DROPITEM_FRAME_THICKNESS,
		*(Image_Frame_Corner),
		*(Image_Frame_Line),
		*(Image_Frame_Inside)
	);
}

// ドロップアイテム確認シーンが無効→有効と変化しているか確認
void Scene_GetDropItem::ActiveCheck()
{
	/* 前回と現在で無効→有効となっているか確認 */
	if (!this->OldActiveFlg && this->pDataList_GameResource->GetDropItemCheckFlg())
	{
		// 無効→有効に変化している場合
		/* セットアップ処理を行う */
		Setup();
	}

	/* 前回と現在で有効→無効となっているか確認 */
	if (this->OldActiveFlg && !this->pDataList_GameResource->GetDropItemCheckFlg())
	{
		// 有効→無効に変化している場合
		/* 紐づいたUIが有効であるなら削除する */
		if (this->UI_DecisionButton != nullptr)
		{
//			this->UI_DecisionButton->SetDeleteFlg(true);
//			this->UI_DecisionButton = nullptr;
		}
	}

	/* 現在のシーン状態を設定 */
	this->OldActiveFlg = this->pDataList_GameResource->GetDropItemCheckFlg();
}
