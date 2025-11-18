/* シーン"バトル画面"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"
// 関連クラス
#include "DataList_Battle.h"

// コンストラクタ
Scene_Battle::Scene_Battle() : Scene_Base("Scene_Battle", 0, false, false)
{
	/* 初期化 */
	this->iBattlePhase	= 0;	// バトルフェーズ

	/* データリスト"バトル用データ管理"を作成 */
	this->pDataList_Battle = std::make_shared<DataList_Battle>();
	gpDataListServer->AddDataList(this->pDataList_Battle);
}

// デストラクタ
Scene_Battle::~Scene_Battle()
{
	/* データリスト"バトル用データ管理"を削除 */
	gpDataListServer->DeleteDataList("DataList_Battle");
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
}

// 描画
void Scene_Battle::Draw()
{
	DrawString(16, 16, "Battle", GetColor(255, 255, 255));
	DrawBox(0, 0, 120, 170, GetColor(0, 0, 255), TRUE);
}
