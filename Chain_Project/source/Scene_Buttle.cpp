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

// テスト用クラス
#include "Card_Arms_WoodenShield.h"
#include "Card_Arms_WoodenSword.h"
#include "Character_Player.h"
#include "Character_Npc_Test.h"

// コンストラクタ
Scene_Battle::Scene_Battle() : Scene_Base("Scene_Battle", 0, false, false)
{
	/* 初期化 */
	this->iBattlePhase					= 0;	// バトルフェーズ
	this->iBattlePhase_NowBattleAreaNo	= 0;	// 現在処理の実行中のバトルエリア

	/* データリスト"バトル用データ管理"を作成 */
	this->pDataList_Battle = std::make_shared<DataList_Battle>();
	gpDataListServer->AddDataList(this->pDataList_Battle);

	/* 画像読み込み */
	{
		// 画像管理データリスト
		std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

		// バトルエリア
		std::string ImageFilePath = "UI/Battle/UI_BattleArea";
		this->Image_BattleArea[0] = pDataList_Image->iGetImageHandle(ImageFilePath);
		ImageFilePath = "UI/Battle/UI_BattleArea_Over";
		this->Image_BattleArea[1] = pDataList_Image->iGetImageHandle(ImageFilePath);
	}

	/* UI作成 */
	{
		/* "決定"ボタンの作成 */
		this->UI_DecisionButton = std::make_shared<Scene_UI_Button>("Battle_DecisionButton", 1);
		this->UI_DecisionButton->SetButtonText("けってい");
		this->UI_DecisionButton->SetCenterPos({ DECISIONBUTTON_POS_X, DECISIONBUTTON_POS_Y });
		this->UI_DecisionButton->SetFontHandle(giFont_DonguriDuel_32);
		gpSceneServer->AddSceneReservation(this->UI_DecisionButton);
	}

	/* テスト用のカード、キャラクター生成処理 */
	// ※ テスト用なので直打ち
	// カード設定
	std::vector<std::shared_ptr<Card_Base>> DeckCardList;
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenSword>());
	DeckCardList.push_back(std::make_shared<Card_Arms_WoodenShield>());
	this->pDataList_Battle->SetDeckCardList(DeckCardList);
	// 画像更新
	for (auto& Deck : DeckCardList)
	{
		Deck->UpdateImage();
	}
	// キャラクター設定
	this->pDataList_Battle->SetFriendCharacter(0, nullptr);
	this->pDataList_Battle->SetFriendCharacter(1, std::make_shared<Character_Player>());
	this->pDataList_Battle->SetFriendCharacter(2, nullptr);
	this->pDataList_Battle->SetEnemyCharacter(0, std::make_shared<Character_Npc_Test>());
	this->pDataList_Battle->SetEnemyCharacter(1, std::make_shared<Character_Npc_Test>());
	this->pDataList_Battle->SetEnemyCharacter(2, std::make_shared<Character_Npc_Test>());

}

// デストラクタ
Scene_Battle::~Scene_Battle()
{
	/* データリスト"バトル用データ管理"を削除 */
	gpDataListServer->DeleteDataList("DataList_Battle");

	/* 紐づいたUIを削除 */
	this->UI_DecisionButton->SetDeleteFlg(true);
	this->UI_DecisionButton = nullptr;
}

// 更新
void Scene_Battle::Update()
{
	/* バトルフェーズに応じた更新処理を行う */
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
	}

	/* 手札のカードの設定座標の設定 */
	CardPosition_HandSetSettingPosting();

	/* カードの座標補間 */
	CardPosition_Interpolation();

	// キャラクターの座標の設定
	CharacterPosition_Setup();
}

// 描画
void Scene_Battle::Draw()
{
	/* 背景描写 */
	Draw_BackGround();

	/* キャラクター描写 */
	Draw_Character();

	/* バトルエリア描写 */
	Draw_BattleArea();

	/* 手札の描写 */
	Draw_HandCard();

	/* ホールド中のカードを描写 */
	Draw_HoldCard();
}
