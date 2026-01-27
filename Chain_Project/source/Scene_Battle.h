/* シーン"バトル画面"の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"
// 関連クラス
#include "Action_Effect.h"

/* 前方宣言 */
class Scene_UI_Button;
class DataList_Battle;
class Card_Base;
class DataList_GameResource;
class Drop_Item_Base;

// シーン"バトル画面"
class Scene_Battle : public Scene_Base
{
	public:
		Scene_Battle();				// コンストラクタ
		virtual ~Scene_Battle();	// デストラクタ

		/* 関数 */
		virtual void	Update()	override;						// 更新
		virtual void	Draw()		override;						// 描画
		virtual void	Character_Setup(int Level, bool BossFlg);	// キャラクターセットアップ

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
		static const int	BATTLE_PHASE_BATTLE_END_WIN			= 10;	// 戦闘終了(プレイヤー勝利)
		static const int	BATTLE_PHASE_BATTLE_END_GAMEOVER	= 11;	// 戦闘終了(プレイヤー敗北)
		static const int	BATTLE_PHASE_MAX					= 12;	// バトルフェーズ総数
		// 座標関係
		static const int	BATTLE_AREA_INTERVAL				= 300;	// バトルエリアの間隔
		static const int	BATTLE_AREA_POS_Y					= 600;	// バトルエリアのY座標
		static const int	HANDCARD_INTERVAL					= 170;	// 手札のカード間隔
		static const int	HANDCARD_POS_Y						= 900;	// 手札のY座標
		static const int	DECISIONBUTTON_POS_X				= 1750;	// 決定ボタンのX座標
		static const int	DECISIONBUTTON_POS_Y				= 640;	// 決定ボタンのY座標
		static const int	CHARACTER_INTERVAL					= 200;	// キャラクターの間隔
		static const int	CHARACTER_POS_Y						= 280;	// キャラクターのY座標
		static const int	BATTLE_AREA_HEIGHT					= 328;	// バトルエリアの高さ
		static const int	BATTLE_AREA_WIDE					= 258;	// バトルエリアの幅
		// アニメーション関連
		static const int EMPHASIS_ANIMATION_MAX					= 4;	// 強調表示アニメーションの最大数
		static const int EMPHASIS_ANIMATION_SPEED				= 8;	// 強調表示アニメーションの速度(変更までのフレーム数)
		static const int CHAIN_ANIMATION_MAX					= 40;	// 鎖のアニメーションの最大値

	private:
		/* 変数 */
		// データリスト
		std::shared_ptr<DataList_Battle>		pDataList_Battle;		// バトル用データリスト
		std::shared_ptr<DataList_GameResource>	pDataList_GameResource;	// ゲームリソース管理用データリスト
		// その他
		int												iBattlePhase;					// バトルフェーズ
		int												iBattlePhase_NowBattleAreaNo;	// 現在処理の実行中のバトルエリア
		bool											bReloadFlg;						// リロードフラグ
		int												iBattleActionDelay;				// バトルアクション間のディレイ時間
		int												BattleArea_Anim_ImageNo;		// バトルエリアのアニメーションの画像番号
		int												BattleArea_Anim_ChangeDelay;	// バトルエリアのアニメーションの変更までの待機時間
		std::vector<std::shared_ptr<Drop_Item_Base>>	DropItem_List;					// ドロップアイテム一覧
		int												Chain_Anim_Count[4];			// 鎖のアニメーションのカウント[0:ちょっと左上からちょっと右下 / 1:ちょっと右上からちょっと左下 / 2:左下から右上 / 3:右下から左上]
		// 画像
		std::shared_ptr<int> Image_BattleArea[2];										// バトルエリア[0:通常時, 1:カーソル接触時]
		std::shared_ptr<int> Image_BattleArea_Emphasis_Anim[EMPHASIS_ANIMATION_MAX];	// バトルエリア(カーソル接触時のアニメーション)[0～3]
		std::shared_ptr<int> Image_BackGround[3];										// 背景画像[0:キャラクター立ち位置の背景, 1:キャラクター立ち位置の足場, 2:カード置き場]
		std::shared_ptr<int> Image_BattleArea_No_Base;									// バトルエリアの番号の背景
		std::shared_ptr<int> Image_BattleArea_No_Number[5];								// バトルエリアの番号の数字[1～5]
		/* 関数 */
		// 各フェーズごとの更新処理
		void Update_EffectTurnStart();			// "ターン開始時"の効果発動
		void Update_DrawCard();					// カードドロー
		void Update_EnemyActionDecision();		// 敵の行動決定
		void Update_PlayerActionDecision();		// プレイヤーの行動決定
		void Update_CardChainCheck();			// カードのチェイン数確認
		void Update_EffectActionStart();		// "行動開始時"の効果発動
		void Update_BattleAction_Decision();	// 戦闘行動の決定
		void Update_BattleAction();				// 戦闘行動
		void Update_EffectTurnEnd();			// "ターン終了時"の効果発動
		void Update_StatusEffectAdvance();		// 状態変化のターン進行
		// 描写処理
		void Draw_BackGround();					// 背景描写
		void Draw_Character();					// キャラクター描写
		void Draw_Character_Move();				// キャラクターの行動内容描写
		void Draw_Chain();						// 鎖(チェイン時)描写
		void Draw_BattleArea();					// バトルエリア描写
		void Draw_HandCard();					// 手札の描写
		void Draw_HoldCard();					// ホールド中のカードを描写
		void Draw_TrashCard();					// 捨て札のカードの描写
		void Draw_DeckCard();					// 山札のカードの描写
		// UIのハンドル
		std::shared_ptr<Scene_UI_Button> UI_DecisionButton;	// 決定ボタン
		// その他
		void CardPosition_HandSetSettingPosting();							// 手札のカード設定座標の設定
		void Card_Update();													// カードの更新処理
		void CharacterPosition_Setup();										// キャラクターの座標の設定
		std::shared_ptr<Card_Base>	GetMouseInCard();						// マウスが重なっているカードを取得
		int	 GetMouseInBattleArea();										// マウスが重なっているバトルエリアを取得
		void Character_Death_Delete_Check();								// キャラクターが死亡していて、削除するかの確認処理
		void CheckLostCard();												// ロスト対象のカードを確認し、ロストカード一覧に入れる
		void ResetChain();													// チェイン数をリセット
		void CheckGameEnd();												// 戦闘が終了しているかの確認
		void Trash_UseCard(std::shared_ptr<Action_Effect_Base> pEffect);	// 効果を使用したカードのトラッシュ処理
		void Update_EmphasisAnim();											// バトルエリアのカーソル接触時のアニメーションを設定
		void Update_DropItem();												// ドロップアイテムの更新処理
		void Draw_DropItem();												// ドロップアイテムの描写処理
		void Create_DropItem();												// ドロップアイテムの作成処理
		void Update_Chain_Anim();											// 鎖のアニメーションの更新
};
