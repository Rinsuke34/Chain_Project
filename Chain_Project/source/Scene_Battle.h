/* シーン"バトル画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class DataList_Battle;
class Card_Base;

// シーン"バトル画面"
class Scene_Battle : public Scene_Base
{
	public:
		Scene_Battle();				// コンストラクタ
		virtual ~Scene_Battle();	// デストラクタ

		/* 関数 */
		virtual void	Update()			override;	// 更新
		virtual void	Draw()				override;	// 描画

		/* 定数 */
		// バトルフェーズ
		static const int	BATTLE_PHASE_EFFECT_TRUN_START		= 0;	// "ターン開始時"の効果発動
		static const int	BATTLE_PHASE_DRAW_CARD				= 1;	// カードドロー
		static const int	BATTLE_PHASE_ENEMY_ACTION_DECISION	= 2;	// 敵の行動決定
		static const int	BATTLE_PHASE_PLAYER_ACTION_DECISION	= 3;	// プレイヤーの行動決定
		static const int	BATTLE_PHASE_CARD_CHAIN_CHECK		= 4;	// カードのチェイン数確認
		static const int	BATTLE_PHASE_EFFECT_ACTION_START	= 5;	// "行動開始時"の効果発動
		static const int	BATTLE_PHASE_BATTLE_ACTION			= 6;	// 戦闘行動
		static const int	BATTLE_PHASE_EFFECT_TRUN_END		= 7;	// "ターン終了時"の効果発動
		static const int	BATTLE_PHASE_STATUS_EFFECT_ADVANCE	= 8;	// 状態変化のターン進行
		static const int	BATTLE_PHASE_MAX					= 9;	// バトルフェーズ総数
		// 座標関係
		static const int	BATTLE_AREA_INTERVAL				= 250;	// バトルエリアの間隔
		static const int	BATTLE_AREA_HEIGHT					= 540;	// バトルエリアのY座標
		static const int	HANDCARD_INTERVAL					= 125;	// 手札のカード間隔
		static const int	HANDCARD_HEIGHT						= 900;	// 手札のY座標

	private:
		/* 変数 */
		// データリスト
		std::shared_ptr<DataList_Battle> pDataList_Battle;		// バトル用データリスト
		// その他
		int iBattlePhase;										// バトルフェーズ

		/* 関数 */
		// 画像
		std::shared_ptr<int> Image_BattleArea[2];	// バトルエリア[0:通常時, 1:カーソル接触時]
		// 各フェーズごとの更新処理
		void Update_EffectTurnStart();		// "ターン開始時"の効果発動
		void Update_DrawCard();				// カードドロー
		void Update_EnemyActionDecision();	// 敵の行動決定
		void Update_PlayerActionDecision();	// プレイヤーの行動決定
		void Update_CardChainCheck();		// カードのチェイン数確認
		void Update_EffectActionStart();	// "行動開始時"の効果発動
		void Update_BattleAction();			// 戦闘行動
		void Update_EffectTurnEnd();		// "ターン終了時"の効果発動
		void Update_StatusEffectAdvance();	// 状態変化のターン進行
		// 描写処理
		void Draw_BackGround();				// 背景描写
		void Draw_BattleArea();				// バトルエリア描写
		void Draw_HandCard();				// 手札の描写
		void Draw_HoldCard();				// ホールド中のカードを描写
		// その他
		void CardPosition_HandSetSettingPosting();			// 手札のカード設定座標の設定
		void CardPosition_Interpolation();					// カードの座標補間
		std::shared_ptr<Card_Base>	GetMouseInCard();		// マウスが重なっているカードを取得
		int							GetMouseInBattleArea();	// マウスが重なっているバトルエリアを取得
};
