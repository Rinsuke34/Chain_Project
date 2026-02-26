/* シーン"ゲームオーバー画面"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_GameOver.h"
// 共通定義
#include "VariableDefine.h"
// 関連クラス
#include "DataList_GameResource.h"
#include "DataList_SaveData.h"
#include "Scene_UI_Button.h"

// コンストラクタ
Scene_GameOver::Scene_GameOver(bool Winflg) : Scene_Base("Scene_GameOver", 100, false, false)
{
	/* 初期化 */
	this->GameEndFlg		= false;		// ゲーム終了フラグ
	this->BackgroundAlpha	= 0;			// 背景のアルファ値
	this->iPhase			= PHASE_FADEIN;	// フェーズ
	this->UI_DecisionButton = nullptr;		// 決定ボタンのハンドル
	this->WinFlg			= Winflg;		// 勝利したかのフラグ

	/* データリスト取得 */
	// ゲームリソース管理用データリスト
	this->pDataList_GameResource = std::dynamic_pointer_cast<DataList_GameResource>(gpDataListServer->GetDataList("DataList_GameResource"));

	/* 取得経験値を算出＆セーブデータへ設定 */
	this->GetExp = this->pDataList_GameResource->GetHaveCoin();	// 取得経験値 = 所持コイン数
	std::shared_ptr<DataList_SaveData> pDataList_SaveData = std::dynamic_pointer_cast<DataList_SaveData>(gpDataListServer->GetDataList("DataList_SaveData"));
	pDataList_SaveData->SetHaveExp(pDataList_SaveData->GetHaveExp() + this->GetExp);
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
	/* フェーズに応じた更新処理 */
	switch (this->iPhase)
	{
		case PHASE_FADEIN:
			/* 背景のアルファ値を加算していく */
			this->BackgroundAlpha += 5;
			if (this->BackgroundAlpha >= 255 || (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
			{
				this->BackgroundAlpha = 255;

				/* フェーズをUI作成に移行 */
				this->iPhase = PHASE_UI_CREATE;
			}
			break;

		case PHASE_UI_CREATE:
			if (this->UI_DecisionButton == nullptr)
			{
				/* UIを有効化 */
				{
					/* "決定"ボタンの作成 */
					this->UI_DecisionButton = std::make_shared<Scene_UI_Button>("Battle_DecisionButton", this->iLayerOrder + 1);
					this->UI_DecisionButton->SetButtonText("タイトルへもどる");
					this->UI_DecisionButton->SetCenterPos({ SCREEN_SIZE_WIDE / 2, (SCREEN_SIZE_HEIGHT / 2) + 100 });
					this->UI_DecisionButton->SetFontHandle(giFont_DonguriDuel_32);
					gpSceneServer->AddSceneReservation(this->UI_DecisionButton);
				}
			}
			else
			{
				/* ゲーム終了フラグを確認 */
				if (this->GameEndFlg == false)
				{
					// 無効であるならば
					/* "決定"ボタンが入力されたならば */
					if (this->UI_DecisionButton->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
					{
						///* ワールドマップを有効にする */
						this->pDataList_GameResource->SetWoldMapActiveFlg(true);

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
			break;
	}
}

// 描画
void Scene_GameOver::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->BackgroundAlpha);
	
	/* 背景(ブラックアウト)の描写 */
	DrawBox(0, 0, SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT, GetColor(0, 0, 0), TRUE);

	/* 文字の描写 */
	std::string GameOverText = "";
	int iSizeX = 0;
	/* ゲームクリアであるか確認 */
	if (this->WinFlg)
	{
		// ゲームクリアの場合
		/* ゲームクリアの文字描写 */
		GameOverText = "ゲームクリア";
		iSizeX = 300;
		DrawStringToHandle(
			(SCREEN_SIZE_WIDE / 2)		- (iSizeX / 2),
			(SCREEN_SIZE_HEIGHT / 2)	- 200,
			GameOverText.c_str(),
			GetColor(255, 255, 0),
			giFont_DonguriDuel_64
		);
	}
	else
	{
		// ゲームオーバーの場合
		/* ゲームオーバーの文字描写 */
		GameOverText	= "ゲームオーバー";
		iSizeX			= 350;
		DrawStringToHandle(
			(SCREEN_SIZE_WIDE / 2) - (iSizeX / 2),
			(SCREEN_SIZE_HEIGHT / 2) - 200,
			GameOverText.c_str(),
			GetColor(255, 255, 255),
			giFont_DonguriDuel_64
		);
	}

	/* 取得した経験値を描写 */
	std::string GetExpText = "かくとく　" + std::to_string(this->GetExp) + "EXP";
	iSizeX = 210;
	DrawStringToHandle(
		(SCREEN_SIZE_WIDE / 2) - (iSizeX / 2),
		(SCREEN_SIZE_HEIGHT / 2),
		GetExpText.c_str(),
		GetColor(255, 255, 255),
		giFont_DonguriDuel_32
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
