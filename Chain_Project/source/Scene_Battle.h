/* シーン"バトル画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"
#include "StructDefine.h"

/* 前方宣言 */
class Scene_UI_Button;
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
		static const int	BATTLE_PHASE_BATTLE_ACTION_DECISION = 6;	// 戦闘行動の決定
		static const int	BATTLE_PHASE_BATTLE_ACTION			= 7;	// 戦闘行動
		static const int	BATTLE_PHASE_EFFECT_TRUN_END		= 8;	// "ターン終了時"の効果発動
		static const int	BATTLE_PHASE_STATUS_EFFECT_ADVANCE	= 9;	// 状態変化のターン進行
		static const int	BATTLE_PHASE_MAX					= 10;	// バトルフェーズ総数
		// 座標関係
		static const int	BATTLE_AREA_INTERVAL				= 300;	// バトルエリアの間隔
		static const int	BATTLE_AREA_POS_Y					= 540;	// バトルエリアのY座標
		static const int	HANDCARD_INTERVAL					= 170;	// 手札のカード間隔
		static const int	HANDCARD_POS_Y						= 900;	// 手札のY座標
		static const int	DECISIONBUTTON_POS_X				= 1720;	// 決定ボタンのX座標
		static const int	DECISIONBUTTON_POS_Y				= 640;	// 決定ボタンのY座標
		static const int	CHARACTER_INTERVAL					= 200;	// キャラクターの間隔
		static const int	CHARACTER_POS_Y						= 260;	// キャラクターのY座標

	private:
		/* 変数 */
		// データリスト
		std::shared_ptr<DataList_Battle> pDataList_Battle;		// バトル用データリスト
		// その他
		int iBattlePhase;										// バトルフェーズ
		int iBattlePhase_NowBattleAreaNo;						// 現在処理の実行中のバトルエリア
		bool bReloadFlg;										// リロードフラグ
		int iBattleActionDelay;									// バトルアクション間のディレイ時間
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
		void Update_BattleAction_Decision();// 戦闘行動の決定
		void Update_BattleAction();			// 戦闘行動
		void Update_EffectTurnEnd();		// "ターン終了時"の効果発動
		void Update_StatusEffectAdvance();	// 状態変化のターン進行
		// 描写処理
		void Draw_BackGround();				// 背景描写
		void Draw_Character();				// キャラクター描写
		void Draw_Chain();					// 鎖(チェイン時)描写
		void Draw_BattleArea();				// バトルエリア描写
		void Draw_HandCard();				// 手札の描写
		void Draw_HoldCard();				// ホールド中のカードを描写
		// UIのハンドル
		std::shared_ptr<Scene_UI_Button> UI_DecisionButton;	// 決定ボタン
		// その他
		void CardPosition_HandSetSettingPosting();									// 手札のカード設定座標の設定
		void CardPosition_Interpolation();											// カードの座標補間
		void CharacterPosition_Setup();												// キャラクターの座標の設定
		std::shared_ptr<Card_Base>	GetMouseInCard();								// マウスが重なっているカードを取得
		int	 GetMouseInBattleArea();												// マウスが重なっているバトルエリアを取得
		void UseCardEffect(std::shared_ptr<Card_Effect_Base> Effect, int AreaNo);	// カード効果の使用
		void Character_Death_Check();												// キャラクターが死亡しているか確認
		void CheckLostCard();														// ロスト対象のカードを確認し、ロストカード一覧に入れる
};
