/* シーン"バトル画面"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"
// 共通定義
#include "VariableDefine.h"
// 関連クラス
#include "Scene_UI_Button.h"
#include "Card_Base.h"
#include "DataList_Battle.h"
#include "DataList_Image.h"
#include "DataList_GameResource.h"

// コンストラクタ
Scene_Battle::Scene_Battle() : Scene_Base("Scene_Battle", 50, false, false)
{
	/* 初期化 */
	this->iBattlePhase					= 0;		// バトルフェーズ
	this->iBattlePhase_NowBattleAreaNo	= 0;		// 現在処理の実行中のバトルエリア
	this->bReloadFlg					= false;	// ドロー失敗フラグ
	this->iBattleActionDelay			= 0;		// バトルアクション間のディレイ時間
	this->BattleArea_Anim_ImageNo		= 0;		// バトルエリアのアニメーションの画像番号
	this->BattleArea_Anim_ChangeDelay	= 0;		// バトルエリアのアニメーションの変更までの待機時間
	for (int i = 0; i < 4; i++)
	{
		this->Chain_Anim_Count[i]		= 0;		// 鎖のアニメーションのカウント
	}

	/* データリスト"バトル用データ管理"を作成 */
	this->pDataList_Battle = std::make_shared<DataList_Battle>();
	gpDataListServer->AddDataList(this->pDataList_Battle);

	/* データリスト取得 */
	// ゲームリソース管理用データリスト
	this->pDataList_GameResource = std::dynamic_pointer_cast<DataList_GameResource>(gpDataListServer->GetDataList("DataList_GameResource"));

	/* 画像読み込み */
	{
		// 画像管理データリスト
		std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

		// バトルエリア
		std::string ImageFilePath = "UI/Battle/UI_BattleArea";
		this->Image_BattleArea[0] = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Battle/UI_BattleArea_Over";
		this->Image_BattleArea[1] = pDataList_Image->iGetImageHandle(ImageFilePath);

		// バトルエリア(カーソル接触時のアニメーション)[0～3]
		ImageFilePath = "UI/Battle/Emphasis_Anim/UI_BattleArea_Emphasis_1";
		this->Image_BattleArea_Emphasis_Anim[0] = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Battle/Emphasis_Anim/UI_BattleArea_Emphasis_2";
		this->Image_BattleArea_Emphasis_Anim[1] = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Battle/Emphasis_Anim/UI_BattleArea_Emphasis_3";
		this->Image_BattleArea_Emphasis_Anim[2] = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Battle/Emphasis_Anim/UI_BattleArea_Emphasis_4";
		this->Image_BattleArea_Emphasis_Anim[3] = pDataList_Image->iGetImageHandle(ImageFilePath);

		// 背景画像[0:キャラクター立ち位置の背景, 1:キャラクター立ち位置の足場, 2:カード置き場]
		ImageFilePath = "BackGround/Battle_Cave_BackGround";
		this->Image_BackGround[0] = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "BackGround/Battle_Cave_Floor";
		this->Image_BackGround[1] = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "BackGround/Table";
		this->Image_BackGround[2] = pDataList_Image->iGetImageHandle(ImageFilePath);

		/* バトルエリアの番号の背景 */
		ImageFilePath = "UI/Battle/BattleArea_No_Base";
		this->Image_BattleArea_No_Base = pDataList_Image->iGetImageHandle(ImageFilePath);

		/* バトルエリアの番号の数字[1～5] */
		ImageFilePath = "UI/Battle/BattleArea_No_1";
		this->Image_BattleArea_No_Number[0]= pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Battle/BattleArea_No_2";
		this->Image_BattleArea_No_Number[1] = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Battle/BattleArea_No_3";
		this->Image_BattleArea_No_Number[2] = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Battle/BattleArea_No_4";
		this->Image_BattleArea_No_Number[3] = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Battle/BattleArea_No_5";
		this->Image_BattleArea_No_Number[4] = pDataList_Image->iGetImageHandle(ImageFilePath);
	}

	/* UI作成 */
	{
		/* "決定"ボタンの作成 */
		this->UI_DecisionButton = std::make_shared<Scene_UI_Button>("Battle_DecisionButton", this->iLayerOrder + 1);
		this->UI_DecisionButton->SetButtonText("けってい");
		this->UI_DecisionButton->SetCenterPos({ DECISIONBUTTON_POS_X, DECISIONBUTTON_POS_Y });
		this->UI_DecisionButton->SetFontHandle(giFont_DonguriDuel_32);
		gpSceneServer->AddSceneReservation(this->UI_DecisionButton);
	}

	/* カード設定 */
	// ゲームリソースに登録されたデッキを設定する
	std::vector<std::shared_ptr<Card_Base>> DeckCardList = this->pDataList_GameResource->GetDeckCardList();
	// データリストにデッキを設定
	this->pDataList_Battle->SetAllDeckCardList(DeckCardList);
	this->pDataList_Battle->SetDeckCardList(DeckCardList);
	// セットアップ処理
	for (auto& Deck : DeckCardList)
	{
		Deck->UpdateImage();
		Deck->SetUp_DataList();
		Deck->SetCardState(Card_Base::CARDSTATE_DECK);
	}
}

// デストラクタ
Scene_Battle::~Scene_Battle()
{
	/* カードのバフをリセット */
	for (auto& Card : this->pDataList_Battle->GetAllDeckCardList())
	{
		Card->Reset_Buff();
	}

	/* データリスト"バトル用データ管理"を削除 */
	gpDataListServer->DeleteDataList("DataList_Battle");

	/* 紐づいたUIを削除 */
	this->UI_DecisionButton->SetDeleteFlg(true);
	this->UI_DecisionButton = nullptr;

	/* 次のステージ選択を未完了に戻す */
	this->pDataList_GameResource->SetNextStageSelectedFlg(false);
}

// 更新
void Scene_Battle::Update()
{
	/* バトルフェーズに応じた更新処理を行う */
	bool BattleEndFlg = false;
	switch (this->iBattlePhase)
	{
		// "ターン開始時"の効果発動
		case BATTLE_PHASE_EFFECT_TRUN_START:
			Update_EffectTurnStart();
			break;

		// カードドロー
		case BATTLE_PHASE_DRAW_CARD:
			Update_DrawCard();
			break;

		// 敵の行動決定
		case BATTLE_PHASE_ENEMY_ACTION_DECISION:
			Update_EnemyActionDecision();
			break;

		// プレイヤーの行動決定
		case BATTLE_PHASE_PLAYER_ACTION_DECISION:
			Update_PlayerActionDecision();
			break;

		// カードのチェイン数確認
		case BATTLE_PHASE_CARD_CHAIN_CHECK:
			Update_CardChainCheck();
			break;

		// "行動開始時"の効果発動
		case BATTLE_PHASE_EFFECT_ACTION_START:
			Update_EffectActionStart();
			break;

		// 戦闘行動の決定
		case BATTLE_PHASE_BATTLE_ACTION_DECISION:
			Update_BattleAction_Decision();
			break;

		// 戦闘行動
		case BATTLE_PHASE_BATTLE_ACTION:
			Update_BattleAction();
			break;

		// "ターン終了時"の効果発動
		case BATTLE_PHASE_EFFECT_TRUN_END:
			Update_EffectTurnEnd();
			break;

		// 状態変化のターン進行
		case BATTLE_PHASE_STATUS_EFFECT_ADVANCE:
			Update_StatusEffectAdvance();
			break;

		// 戦闘終了
		case BATTLE_PHASE_BATTLE_END_WIN:
		case BATTLE_PHASE_BATTLE_END_GAMEOVER:
			BattleEndFlg = true;
			break;
	}

	/* 戦闘中であるかにより処理を変更 */
	if (BattleEndFlg == false)
	{
		// 戦闘中の場合
		/* 戦闘終了確認 */
		CheckGameEnd();
	}
	else
	{
		// 戦闘終了の場合
		/* ドロップアイテム確認シーンが無効になっているか確認 */
		if (!this->pDataList_GameResource->GetDropItemCheckFlg() &&
			!this->pDataList_GameResource->GetNextStageSelectFlg())
		{
			// ワールドマップが無効であるなら、有効にする
			if (!this->pDataList_GameResource->GetWoldMapActiveFlg())
			{
				this->pDataList_GameResource->SetWoldMapActiveFlg(true);
			}
		}

		/* 次のステージの選択が完了しているならこのシーンを削除 */
		if (this->pDataList_GameResource->GetNextStageSelectFlg())
		{
			/* このシーンの削除フラグを有効にする */
			this->bDeleteFlg = true;
		}
	}

	/* カードの設定座標の設定 */
	CardPosition_HandSetSettingPosting();

	/* カードやアクション内容等の更新処理 */
	Card_Update();
	CharacterPosition_Setup();
	Update_DropItem();

	/* バトルエリアのカーソル接触時のアニメーションを設定 */
	Update_EmphasisAnim();

	/* ドロップアイテムの作成処理 */
	Create_DropItem();

	/* キャラクターが死亡しているか確認 */
	Character_Death_Delete_Check();

	/* 鎖のアニメーション更新 */
	Update_Chain_Anim();
}

// 描画
void Scene_Battle::Draw()
{
	/* 背景描写 */
	Draw_BackGround();

	/* キャラクター描写 */
	Draw_Character();

	/* キャラクターの行動内容描写 */
	Draw_Character_Move();

	/* 鎖(チェイン時)描写 */
	Draw_Chain();

	/* バトルエリア描写 */
	Draw_BattleArea();

	/* 山札のカードの描写 */
	Draw_DeckCard();

	/* 捨て札のカードの描写 */
	Draw_TrashCard();

	/* 手札の描写 */
	Draw_HandCard();

	/* ホールド中のカードを描写 */
	Draw_HoldCard();

	/* ドロップアイテムの描写 */
	Draw_DropItem();
}
