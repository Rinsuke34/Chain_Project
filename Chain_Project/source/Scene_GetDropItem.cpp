/* シーン"ドロップアイテム取得"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_GetDropItem.h"
// 関連クラス
#include "DataList_GameResource.h"
#include "DataList_Image.h"
#include "Card_Base.h"
#include "Scene_UI_Button.h"
#include "Scene_UI_ExplanationText.h"
// 共通定義
#include "FunctionDefine.h"
#include "VariableDefine.h"

// コンストラクタ
Scene_GetDropItem::Scene_GetDropItem() : Scene_Base("Scene_GetDropItem", 100, false, false)
{
	/* 初期化 */
	this->SceneGetDropItemDrawPos	= { 0, DROPITEM_DRAWPOS_Y_LOW };	// ワールドマップの描写座標
	this->OldActiveFlg				= false;							// 以前のドロップアイテム確認シーン有効フラグ
	this->Phase						= PHASE_CARD_LINE_UP;				// フェーズ
	this->PhaseTimer				= PHASE_TIMER_MAX_MOVE;				// フェーズ用タイマー
	this->TableCardIndex			= 0;								// カードを表にするフェーズ用カードインデックス

	/* 画像取得 */
	{
		/* 画像管理データリストを取得 */
		std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

		/* 背景の枠の画像を取得 */
		std::string ImageFilePath = "UI/Button/Button_Frame_Corner";
		this->Image_Frame_Corner = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Button/Button_Frame_Line";
		this->Image_Frame_Line = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Button/Button_Frame_Inside";
		this->Image_Frame_Inside = pDataList_Image->iGetImageHandle(ImageFilePath);
	}

	/* データリスト */
	// ゲームリソース管理用データリスト
	this->pDataList_GameResource = std::dynamic_pointer_cast<DataList_GameResource>(gpDataListServer->GetDataList("DataList_GameResource"));
}

// デストラクタ
Scene_GetDropItem::~Scene_GetDropItem()
{
	/* 紐づいたUIを削除 */
	if (this->UI_DecisionButton)
	{
		this->UI_DecisionButton->SetDeleteFlg(true);
		this->UI_DecisionButton = nullptr;
	}
	if (this->UI_ExplanationText)
	{
		this->UI_ExplanationText->SetDeleteFlg(true);
		this->UI_ExplanationText = nullptr;
	}
}

// 更新
void Scene_GetDropItem::Update()
{
	/* フェーズの更新 */
	Update_Phase();

	/* 描写座標の更新 */
	Update_DrawPos();

	/* カードの更新 */
	Update_Card();

	/* カード座標の設定 */
	CardPosition_Setup();

	/* 終了確認 */
	EndCheck();

	/* ドロップアイテム確認シーンが無効→有効と変化しているか確認 */
	ActiveCheck();

	/* 説明文設定 */
	Update_Explanation();
}

// セットアップ
void Scene_GetDropItem::Setup()
{
	/* 現在設定されているカードリストを初期化 */
	this->GetCardList.clear();
	
	/* ドロップしたカードリストを設定&セットアップ処理 */
	for (auto& Card : this->pDataList_GameResource->GetDropCardList())
	{
		/* リストへ設定 */
		this->GetCardList.push_back(Card);

		/* セットアップ処理 */
		Card->UpdateImage();
		Struct_2D::POSITION StartPos = { 226, 30 };
		Card->SetNowPos(StartPos);
		Card->SetCardState(Card_Base::CARDSTATE_GETLIST_BACK);
	}

	/* リソース上のドロップしたカードを削除 */
	this->pDataList_GameResource->ClearDropCardList();
	
	/* 描写座標を初期化 */
	this->SceneGetDropItemDrawPos.iY = DROPITEM_DRAWPOS_Y_LOW;

	/* 取得カードの初期配置をここで行う（Setup 内で確実に設定） */
	CardPosition_Setup();

	/* "決定"ボタンの作成 */
	this->UI_DecisionButton = std::make_shared<Scene_UI_Button>("Battle_DecisionButton", this->iLayerOrder + 1);
	this->UI_DecisionButton->SetButtonText("けってい");
	this->UI_DecisionButton->SetCenterPos({ 1750, SceneGetDropItemDrawPos.iY + (DROPITEM_DRAW_HEIGHT / 2) });
	this->UI_DecisionButton->SetFontHandle(giFont_DonguriDuel_32);
	gpSceneServer->AddSceneReservation(this->UI_DecisionButton);

	/* 初期化処理 */
	this->Phase				= PHASE_CARD_LINE_UP;
	this->PhaseTimer		= PHASE_TIMER_MAX_MOVE;
	this->TableCardIndex	= 0;
}

// 描画
void Scene_GetDropItem::Draw()
{
	/* 背景描写 */
	BackGround_Drow();

	/* 取得カードの描写 */
	Draw_GetCard();
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

// 背景描写
void Scene_GetDropItem::BackGround_Drow()
{
	/* 背景、フレームの描写 */
	DRAW_FUNCTION::DrawFrame_Image(
		{ DROPITEM_DRAW_WIDTH / 2, (DROPITEM_DRAW_HEIGHT / 2) + SceneGetDropItemDrawPos.iY },
		{ DROPITEM_DRAW_WIDTH - (DROPITEM_FRAME_THICKNESS * 2), DROPITEM_DRAW_HEIGHT - (DROPITEM_FRAME_THICKNESS * 2)},
		DROPITEM_FRAME_THICKNESS,
		*(this->Image_Frame_Corner),
		*(this->Image_Frame_Line),
		*(this->Image_Frame_Inside)
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
			this->UI_DecisionButton->SetDeleteFlg(true);
			this->UI_DecisionButton = nullptr;
		}
	}

	/* 現在のシーン状態を設定 */
	this->OldActiveFlg = this->pDataList_GameResource->GetDropItemCheckFlg();
}

// カード位置セットアップ
void Scene_GetDropItem::CardPosition_Setup()
{
	/* 取得したカードの総数を取得 */
	int CardCount = static_cast<int>(this->GetCardList.size());

	/* 取得したカードの設定座標を算出し、設定する */
	for (int i = 0; i < CardCount; i++)
	{
		/* そのインデックスのカードが裏面(削除選択)状態であるなら設定座標を少し下げる */
		int CorrectionY = 0;
		if (this->GetCardList[i]->GetCardState() == Card_Base::CARDSTATE_GETLIST_FRONT_DELETE)
		{
			CorrectionY = 20;
		}

		/* 設定座標を算出 */
		Struct_2D::POSITION SettingPos =
		{
			(DROPITEM_DRAW_WIDTH / 2) - ((GETCARD_INTERVAL * (CardCount - 1)) / 2) + (GETCARD_INTERVAL * i),
			GETCARD_POS_Y + this->SceneGetDropItemDrawPos.iY + CorrectionY
		};

		/* カードに設定座標を設定 */
		this->GetCardList[i]->SetSettingPos(SettingPos);
	}
}

// 取得カード描写
void Scene_GetDropItem::Draw_GetCard()
{
	for (auto& Card : this->GetCardList)
	{
		Card->Draw();
	}
}

// カードの更新
void Scene_GetDropItem::Update_Card()
{
	for (auto& Card : this->GetCardList)
	{
		Card->Update();
	}
}

// フェーズの更新
void Scene_GetDropItem::Update_Phase()
{
	/* フェーズに応じた更新処理 */
	switch (this->Phase)
	{
		// カード整列フェーズ
		case PHASE_CARD_LINE_UP:
			/* タイマーを更新 */
			if (this->PhaseTimer > 0)
			{
				// タイマーが残っている場合
				this->PhaseTimer--;
			}
			else
			{
				// タイマーが無くなった場合
				/* 次のフェーズへ */
				this->Phase			= PHASE_CARD_TABLE;
			}
			break;

		// カードを表にするフェーズ
		case PHASE_CARD_TABLE:
			/* 左クリックが大縄れたなら全てのカードを表にし、次のフェーズへ */
			if (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT)
			{
				/* 全てのカードを表にする */
				for (auto& Card : this->GetCardList)
				{
					Card->SetCardState(Card_Base::CARDSTATE_GETLIST_FRONT);
				}
				/* 次のフェーズへ */
				this->Phase = PHASE_WAIT_DECISION;
				break;
			}

			/* タイマーを更新 */
			if (this->PhaseTimer > 0)
			{
				// タイマーが残っている場合
				this->PhaseTimer--;
			}
			else
			{
				// タイマーが無くなった場合
				/* タイマーをリセット */
				this->PhaseTimer = PHASE_TIMER_MAX_OPEN;

				/* カードを表にする */
				if (this->TableCardIndex < static_cast<int>(this->GetCardList.size()))
				{
					this->GetCardList[this->TableCardIndex]->SetCardState(Card_Base::CARDSTATE_GETLIST_FRONT);
				}

				/* 次のカードへ */
				this->TableCardIndex++;
				if (this->TableCardIndex >= static_cast<int>(this->GetCardList.size()))
				{
					// 全てのカードを表にした場合
					/* 次のフェーズへ */
					this->Phase = PHASE_WAIT_DECISION;
				}
			}
			break;

		// 決定待機フェーズ
		case PHASE_WAIT_DECISION:
			/* 左クリックが押されたか確認 */
			if (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT)
			{
				/* カーソルが接触しているカードがあるか確認 */
				for (auto& Card : this->GetCardList)
				{
					if (Card->MouseInCard())
					{
						/* そのカードの状態を選択と非選択状態を切り替える */
						if (Card->GetCardState() == Card_Base::CARDSTATE_GETLIST_FRONT_DELETE)
						{
							/* そのカードを通常状態にする */
							Card->SetCardState(Card_Base::CARDSTATE_GETLIST_FRONT);
						}
						else
						{
							/* そのカードを削除選択状態にする */
							Card->SetCardState(Card_Base::CARDSTATE_GETLIST_FRONT_DELETE);
						}
						break;
					}
				}
			}
			break;
	}
}

// 終了確認
void Scene_GetDropItem::EndCheck()
{
	/* "決定"ボタンが入力されたか確認 */
	if (this->UI_DecisionButton != nullptr)
	{
		if (this->UI_DecisionButton->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
		{
			/* ドロップアイテム確認シーンを無効化 */
			this->pDataList_GameResource->SetDropItemCheckFlg(false);

			/* デッキのカードリストを取得 */
			std::vector<std::shared_ptr<Card_Base>> DeckCardList = this->pDataList_GameResource->GetDeckCardList();

			/* 取得カードの内、削除状態ではないカードをデッキに追加する */
			for (auto& Card : this->GetCardList)
			{
				if (Card->GetCardState() != Card_Base::CARDSTATE_GETLIST_FRONT_DELETE)
				{
					DeckCardList.push_back(Card);
				}
			}

			/* デッキのカードリストを設定 */
			this->pDataList_GameResource->SetDeckCardList(DeckCardList);

			/* 現在設定されているカードリストを初期化 */
			this->GetCardList.clear();
		}
	}
}

// 説明文設定
void Scene_GetDropItem::Update_Explanation()
{
	/* すべてのカードにカーソルが重なっているか確認 */
	bool ExplanationDrowFlg = false;
	for (const auto& card : this->GetCardList)
	{
		if (card)
		{
			/* カーソルが重なっているか */
			if (card->MouseInCard())
			{
				// 重なっている場合
				/* 現在説明UIがnullであるなら作成する */
				if (!this->UI_ExplanationText)
				{
					/* 説明UIを作成する */
					this->UI_ExplanationText = std::make_shared<Scene_UI_ExplanationText>(this->iLayerOrder + 1);
					gpSceneServer->AddSceneReservation(this->UI_ExplanationText);
				}

				/* 説明文を設定する */
				this->UI_ExplanationText->SetExplanationText(card->GetExplanationText());

				/* 設定座標を設定する */
				Struct_2D::POSITION ExplanationPos = card->GetNowPos();
				ExplanationPos.iY -= (Card_Base::CARD_HEIGHT / 2);
				this->UI_ExplanationText->SetBasePos(ExplanationPos);

				/* 上方向に描写するよう設定 */
				this->UI_ExplanationText->SetUpwardDisplayFlg(true);

				/* 描写フラグを有効化する */
				ExplanationDrowFlg = true;
				return;
			}
		}
	}

	/* 描写フラグが無効であるなら説明UIを削除する */
	if (!ExplanationDrowFlg)
	{
		if (this->UI_ExplanationText)
		{
			this->UI_ExplanationText->SetDeleteFlg(true);
			this->UI_ExplanationText = nullptr;
		}
	}
}
