/* シーン"バトル画面"の定義(更新部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"
// 関連クラス
#include "DataList_GameResource.h"
#include "Scene_UI_Button.h"
#include "Scene_UI_ExplanationText.h"
#include "Scene_WoldMap.h"
#include "Scene_GameOver.h"
#include "Card_Base.h"
#include "Card_Spell_Base.h"
#include "DataList_Battle.h"
#include "Character_Base.h"
#include "Character_Player.h"
#include "Action_Effect.h"
#include "Drop_Item.h"
#include "DataList_SaveData.h"
// 共通定義
#include "VariableDefine.h"

/* 各フェーズごとの更新処理 */
// "戦闘開始時"の効果発動
void Scene_Battle::Update_EffectBattleStart()
{
	/* 各カードの"戦闘開始時"効果を実行 */
	for (auto& Deck : this->pDataList_Battle->GetDeckCardList())
	{
		Deck->Effect_BattleStart();
	}

	/* "ターン開始時"の効果発動フェイズへ遷移遷移 */
	this->iBattlePhase = BATTLE_PHASE_EFFECT_TRUN_START;
}

// "ターン開始時"の効果発動
void Scene_Battle::Update_EffectTurnStart()
{
	/* 各カードの"ターン開始時"効果を実行 */
	// デッキ
	for (int i = 0; i < this->pDataList_Battle->GetDeckCardList().size(); i++)
	{
		auto AllDeckCard = this->pDataList_Battle->GetDeckCardList()[i];
		if (AllDeckCard != nullptr)
		{
			/* 効果発動:ターン開始時 */
			AllDeckCard->Effect_StartTurn();
		}
	}
	// 手札
	for (int i = 0; i < this->pDataList_Battle->GetHandCardList().size(); i++)
	{
		auto AllDeckCard = this->pDataList_Battle->GetHandCardList()[i];
		if (AllDeckCard != nullptr)
		{
			/* 効果発動:ターン開始時 */
			AllDeckCard->Effect_StartTurn();
		}
	}
	// 捨て札
	for (int i = 0; i < this->pDataList_Battle->GetTrashCardList().size(); i++)
	{
		auto AllDeckCard = this->pDataList_Battle->GetTrashCardList()[i];
		if (AllDeckCard != nullptr)
		{
			/* 効果発動:ターン開始時 */
			AllDeckCard->Effect_StartTurn();
		}
	}

	/* ロストフラグが有効なカードをロストカードリストに設定 */
	CheckLostCard();

	/* "カードドロー"フェイズへ遷移 */
	this->iBattlePhase = BATTLE_PHASE_DRAW_CARD;
}

// カードドロー
void Scene_Battle::Update_DrawCard()
{
	this->bReloadFlg = false;
	int DrawCount = 5;

	/* プレイヤーが旅人であるなら、ドロー枚数を+１する */
	std::shared_ptr<DataList_SaveData> SaveData = std::dynamic_pointer_cast<DataList_SaveData>(gpDataListServer->GetDataList("DataList_SaveData"));
	if (SaveData->GetPlayerClassNo() == DataList_SaveData::CLASS_TRAVELER)
	{
		DrawCount += 1;
	}

	/* ドロー処理 */
	for (int i = 0; i < DrawCount; i++)
	{
		/* デッキにカードが存在するか確認 */
		if (this->pDataList_Battle->GetDeckCardList().size() > 0)
		{
			// 存在する場合
			/* デッキの先頭のカードを手札に移動 */
			std::shared_ptr<Card_Base> pDrawCard = this->pDataList_Battle->GetDeckCardList().front();
			this->pDataList_Battle->AddHandCard(pDrawCard);
			this->pDataList_Battle->RemoveDeckCard(pDrawCard);
			pDrawCard->SetCardState(Card_Base::CARDSTATE_HAND);
		}
		else if(this->pDataList_Battle->GetHandCardList().size() == 0)
		{
			// デッキにカード無しかつ、手札が0枚の場合
			/* リロードフラグを有効化する */
			this->bReloadFlg = true;

			/* 捨て札のカードを山札に戻す */
			std::vector<std::shared_ptr<Card_Base>> TrashCardList = this->pDataList_Battle->GetTrashCardList();
			for (auto& pCard : TrashCardList)
			{
				if (pCard != nullptr)
				{
					this->pDataList_Battle->AddDeckCard(pCard);
					this->pDataList_Battle->RemoveTrashCard(pCard);
				}
			}

			/* デッキリストのシャッフル */
			ShuffledDeck();
			break;
		}
	}

	/* "敵の行動決定"フェイズへ遷移 */
	this->iBattlePhase = BATTLE_PHASE_ENEMY_ACTION_DECISION;
}

// 敵の行動決定
void Scene_Battle::Update_EnemyActionDecision()
{
	/* 全てのキャラクターの行動決定処理を行う */
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		// 仲間キャラクター
		auto FriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
		if (FriendCharacter != nullptr)
		{
			FriendCharacter->Action();
		}
		// 敵キャラクター
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr)
		{
			EnemyCharacter->Action();
			EnemyCharacter->ShieldReset_EndTurn();
		}
	}

	/* "プレイヤーの行動決定"フェイズへ遷移 */
	this->iBattlePhase = BATTLE_PHASE_PLAYER_ACTION_DECISION;
}

// プレイヤーの行動決定
void Scene_Battle::Update_PlayerActionDecision()
{
	/* リロードフラグが有効ならばこのフェーズをスキップする */
	if (this->bReloadFlg)
	{
		/* "行動開始時の効果発動"フェイズへ遷移 */
		this->iBattlePhase = BATTLE_PHASE_EFFECT_ACTION_START;
		return;
	}

	/* カードをホールド中であるか確認 */
	if (this->pDataList_Battle->GetHoldCard() != nullptr)
	{
		// ホールド中である場合
		/* マウスの左ボタンがホールド状態であるか確認 */
		if (gstKeyboardInputData.igInput[INPUT_HOLD] & MOUSE_INPUT_LEFT)
		{
			// ホールド状態である場合
			/* ホールド中のカードの位置をマウスの位置に更新 */
			this->pDataList_Battle->GetHoldCard()->SetNowPos
			({
				gstKeyboardInputData.iMouseX,
				gstKeyboardInputData.iMouseY
			});

			/* ホールド中のカードの状態を"ピックアップ中"に設定 */
			this->pDataList_Battle->GetHoldCard()->SetCardState(Card_Base::CARDSTATE_PICKED);
		}
		else
		{
			// 左ボタンがホールド状態でない場合
			/* バトルエリアに接触しているか確認 */
			int iBattleAreaIndex = GetMouseInBattleArea();
			if (iBattleAreaIndex != -1)
			{
				// 接触している場合
				/* 対象のバトルエリアにカードが設定されているか確認 */
				if (this->pDataList_Battle->GetBattleAreaCardList(iBattleAreaIndex) != nullptr)
				{
					// カードが設定されている場合
					/* バトルエリアのカードを手札に戻す */
					this->pDataList_Battle->AddHandCard(this->pDataList_Battle->GetBattleAreaCardList(iBattleAreaIndex));

					/* 戻したカードの設定座標を手札の位置に設定 */
					this->pDataList_Battle->GetBattleAreaCardList(iBattleAreaIndex)->SetSettingPos({ 0, 0 });

					/* 戻したカードの状態を"手札"に設定する */
					this->pDataList_Battle->GetBattleAreaCardList(iBattleAreaIndex)->SetCardState(Card_Base::CARDSTATE_HAND);

					/* バトルエリアのカードを削除 */
					this->pDataList_Battle->RemoveBattleAreaCard(iBattleAreaIndex);
				}

				/* ホールド中のカードをバトルエリアに配置 */
				this->pDataList_Battle->SetBattleAreaCard(iBattleAreaIndex, this->pDataList_Battle->GetHoldCard());

				/* 配置したカードの設定座標をバトルエリアの位置に設定 */
				this->pDataList_Battle->GetHoldCard()->SetSettingPos({ (SCREEN_SIZE_WIDE / 2) + (BATTLE_AREA_INTERVAL * (iBattleAreaIndex - 2)), BATTLE_AREA_POS_Y });

				/* 配置したカードの状態を"設定中"にする */
				this->pDataList_Battle->GetHoldCard()->SetCardState(Card_Base::CARDSTATE_SETTING);
			}
			else
			{
				// 接触していない場合
				/* ホールド中のカードを手札に設定する */
				this->pDataList_Battle->AddHandCard(this->pDataList_Battle->GetHoldCard());

				/* 戻したカードの設定座標を手札の位置に設定 */
				this->pDataList_Battle->GetHoldCard()->SetSettingPos({0, 0});

				/* ホールド中のカードの状態を"手札"に設定する */
				this->pDataList_Battle->GetHoldCard()->SetCardState(Card_Base::CARDSTATE_HAND);
			}

			/* ホールド中のカードをホールド解除する */
			this->pDataList_Battle->SetHoldCard(nullptr);

			/* カードのチェイン状態を確認する */
			this->pDataList_Battle->CheckChain();
		}
	}
	else
	{
		// ホールド中でない場合
		/* カーソルが重なっているカードを取得 */
		std::shared_ptr<Card_Base> pInCard = GetMouseInCard();
		if (pInCard != nullptr)
		{
			// 重なっているカードがある場合
			/* マウスの左ボタンがトリガーされているか確認 */
			if (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT)
			{
				// トリガーされている場合
				/* 取得したカードをホールド状態にする */
				this->pDataList_Battle->SetHoldCard(pInCard);

				/* ホールド状態のカードを手札とバトルエリアから削除する */
				this->pDataList_Battle->RemoveHandCard(pInCard);
				for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
				{
					if (this->pDataList_Battle->GetBattleAreaCardList(i) == pInCard)
					{
						this->pDataList_Battle->RemoveBattleAreaCard(i);
						break;
					}
				}
			}
		}
	}

	/* "決定"ボタンが入力されたならば */
	if (this->UI_DecisionButton->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
	{
		/* "行動開始時の効果発動"フェイズへ遷移 */
		this->iBattlePhase = BATTLE_PHASE_EFFECT_ACTION_START;
	}
}

// カードのチェイン数確認
void Scene_Battle::Update_CardChainCheck()
{
	/* 各バトルエリアにチェイン数を設定する */
	int ChainCount = 0;

	for (int BattleAreaNo = 0; BattleAreaNo < DataList_Battle::BATTLE_AREA_MAX; BattleAreaNo++)
	{
		/* 現在のバトルエリアのカードを取得 */
		std::shared_ptr<Card_Base> CurrentCard = this->pDataList_Battle->GetBattleAreaCardList(BattleAreaNo);

		/* カードが存在しない場合はチェイン数をリセット */
		if (CurrentCard == nullptr)
		{
			ChainCount = 0;
			continue;
		}

		/* 現在のカードにチェイン数を設定 */
		CurrentCard->SetNowChainCount(ChainCount);

		/* 次のバトルエリアとのチェイン判定 */
		if (BattleAreaNo < DataList_Battle::BATTLE_AREA_MAX - 1)
		{
			/* 次のエリアとのチェイン状態を確認 */
			std::vector<std::string> ChainSuiteList = this->pDataList_Battle->GetChain_Suite_List(BattleAreaNo);
			if (ChainSuiteList.size() > 0)
			{
				// チェインが繋がっている場合、次のカードのためにカウントを加算
				ChainCount += static_cast<int>(ChainSuiteList.size());
			}
			else
			{
				// チェインが途切れた場合、カウントをリセット
				ChainCount = 0;
			}
		}
	}
}

// "行動開始時"の効果発動
void Scene_Battle::Update_EffectActionStart()
{
	/* 各カードの"行動開始時"効果を実行 */
	// バトルエリア
	for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	{
		auto AllDeckCard = this->pDataList_Battle->GetBattleAreaCardList(i);
		if (AllDeckCard != nullptr)
		{
			/* 効果発動:ターン開始時 */
			AllDeckCard->Effect_StartAction();
		}
	}

	/* ロストフラグが有効なカードをロストカードリストに設定 */
	CheckLostCard();

	/* "戦闘行動の決定"フェイズへ遷移 */
	this->iBattlePhase = BATTLE_PHASE_BATTLE_ACTION_DECISION;
}

// 戦闘行動の決定
void Scene_Battle::Update_BattleAction_Decision()
{
	/* バトルエリア1～5の順で処理を行う */
	for (int i = DataList_Battle::BATTLE_AREA_1; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	{
		/* バトルエリアにカードが設定されているか確認 */
		if (this->pDataList_Battle->GetBattleAreaCardList(i) != nullptr)
		{
			// カードが設定されている場合
			/* カードの戦闘行動を実行 */
			this->pDataList_Battle->GetBattleAreaCardList(i)->BattleAction();
		}

		/* 効果を優先順位に並び変える */
		std::vector<std::shared_ptr<Action_Effect_Base>> ActionEffectList = this->pDataList_Battle->GetActionEffectList();
		std::stable_sort(ActionEffectList.begin(), ActionEffectList.end(),
			[](const std::shared_ptr<Action_Effect_Base>& a, const std::shared_ptr<Action_Effect_Base>& b) {
				int pa = a ? a->Priority : INT_MIN;
				int pb = b ? b->Priority : INT_MIN;
				return pa > pb;
			});
		this->pDataList_Battle->SetActionEffectList(ActionEffectList);
	}

	/* "戦闘行動"フェイズへ遷移 */
	// ※バトルエリア1から順に処理を行うため、最初にバトルエリア1を設定しておく
	this->iBattlePhase_NowBattleAreaNo	= DataList_Battle::BATTLE_AREA_1;
	this->iBattlePhase					= BATTLE_PHASE_BATTLE_ACTION;
	this->iBattleActionDelay			= 30;
}

// 戦闘行動
void Scene_Battle::Update_BattleAction()
{
	/* 戦闘行動間のディレイ時間が完了しているか確認 */
	if (this->iBattleActionDelay > 0)
	{
		// 完了していない場合
		/* ディレイ時間を減少 */
		// ※倍速フラグが有効な場合は通常の半分の時間で行動する
		this->iBattleActionDelay -= this->BattleSpeedDoubleFlg ? 2 : 1;
		return;
	}

	/* 行動内容を取得 */
	std::vector<std::shared_ptr<Action_Effect_Base>> EffectList = this->pDataList_Battle->GetActionEffectList();

	/* 行動処理が完了しているか確認 */
	if (EffectList.size() == 0)
	{
		// 完了している場合
		/* 戦闘行動の削除フェイズへ遷移 */
		this->iBattlePhase = BATTLE_PHASE_BATTLE_ACTION_DELETE;

		return;
	}

	/* 与効果の内容を取得 */
	std::shared_ptr<Action_Effect_Base> pEffect = EffectList.front();

	/* 実行者が生存しているか確認 */
	std::shared_ptr<Character_Base>	EffectUser = pEffect->EffectUser;
	if (EffectUser != nullptr)
	{
		// 生存している場合
		/* 効果の内容に応じた処理を実行 */
		pEffect->ExecuteEffect();
		pEffect->Sound_Effect_Play();

		/* 効果処理後のディレイ時間を設定 */
		this->iBattleActionDelay = BATTLEACTION_DELAY;
		
		/* カードによる行動であるならそのカードの設定されているエリア番号を取得 */
		// ついでに現在座標を少し高い位置に変化させる
		int CardAreaNo = -1;
		if (pEffect->EffectCard != nullptr)
		{
			for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
			{
				if (this->pDataList_Battle->GetBattleAreaCardList(i) == pEffect->EffectCard)
				{
					CardAreaNo = i;
					break;
				}
			}

			Struct_2D::POSITION CardNowPos = pEffect->EffectCard->GetNowPos();
			pEffect->EffectCard->SetNowPos({ CardNowPos.iX, CardNowPos.iY - 30 });
		}
		this->ActionCardInAreaNo = CardAreaNo;

		/* 再実行フラグが有効ならディレイ後もう一度実行する */
		if ((pEffect->Restart_State == Action_Effect_Base::RESTART_RESTART) ||
			(pEffect->Restart_State == Action_Effect_Base::RESTART_RESTART_MAINSKIP))
		{
			return;
		}

		/* その行動内容を実行者の行動内容から削除 */
		EffectUser->Delete_Action_Effect(pEffect);

		/* 行動内容を使用済み行動内容リストに設定 */
		this->UsedActionEffectList.push_back(pEffect);
	}

	/* 行動内容を削除する */
	this->pDataList_Battle->RemoveEffect(pEffect);

	/* キャラクターが死亡しているか確認 */
	Character_Death_Delete_Check();
}

// 戦闘行動の削除
void Scene_Battle::Update_BattleAction_Delete()
{
	/* 使用した効果に紐づいたカードをトラッシュ */
	for (auto& Effect : UsedActionEffectList)
	{
		/* 使用した効果に紐づいたカードをトラッシュ */
		Trash_UseCard(Effect);
	}

	/* カードの設定されたエリア番号を初期化 */
	this->ActionCardInAreaNo = -1;

	/* 使用済み行動内容リストを初期化 */
	this->UsedActionEffectList.clear();

	/* "ターン終了時"の効果発動フェイズへ遷移 */
	this->iBattlePhase = BATTLE_PHASE_EFFECT_TRUN_END;
}

// "ターン終了時"の効果発動
void Scene_Battle::Update_EffectTurnEnd()
{
	/* 各カードの"ターン終了時"効果を実行 */
	// デッキ
	for (int i = 0; i < this->pDataList_Battle->GetDeckCardList().size(); i++)
	{
		auto AllDeckCard = this->pDataList_Battle->GetDeckCardList()[i];
		if (AllDeckCard != nullptr)
		{
			/* 効果発動:ターン開始時 */
			AllDeckCard->Effect_EndTurn();
		}
	}
	// 手札
	for (int i = 0; i < this->pDataList_Battle->GetHandCardList().size(); i++)
	{
		auto AllDeckCard = this->pDataList_Battle->GetHandCardList()[i];
		if (AllDeckCard != nullptr)
		{
			/* 効果発動:ターン開始時 */
			AllDeckCard->Effect_EndTurn();
		}
	}
	// 捨て札
	for (int i = 0; i < this->pDataList_Battle->GetTrashCardList().size(); i++)
	{
		auto AllDeckCard = this->pDataList_Battle->GetTrashCardList()[i];
		if (AllDeckCard != nullptr)
		{
			/* 効果発動:ターン開始時 */
			AllDeckCard->Effect_EndTurn();
		}
	}

	/* ロストフラグが有効なカードをロストカードリストに設定 */
	CheckLostCard();

	/* チェイン数をリセット */
	ResetChain();

	/* "状態変化のターン進行"フェイズへ遷移 */
	this->iBattlePhase = BATTLE_PHASE_STATUS_EFFECT_ADVANCE;
}

// 状態変化のターン進行
void Scene_Battle::Update_StatusEffectAdvance()
{
	/* 各キャラクターの状態変化のターンを進行 */
	// ※プレイヤー陣営のシールドのリセットもここで実施
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		// 仲間
		auto FriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
		if (FriendCharacter != nullptr)
		{
			FriendCharacter->Update_Buff_Debuff();
			FriendCharacter->ShieldReset_EndTurn();
		}
		// 敵
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr)
		{
			EnemyCharacter->Update_Buff_Debuff();
		}
	}

	/* キャラクターが死亡しているか確認 */
	Character_Death_Delete_Check();

	/* チェイン情報を初期化する */
	this->pDataList_Battle->ResetChain();

	/* "ターン開始時の効果効果発動"フェイズへ遷移 */
	this->iBattlePhase = BATTLE_PHASE_EFFECT_TRUN_START;
}

/* その他 */
// 手札のカード設定座標の設定
void Scene_Battle::CardPosition_HandSetSettingPosting()
{
	/* 手札の総数を取得 */
	int HandCardCount = static_cast<int>(this->pDataList_Battle->GetHandCardList().size());

	/* 手札のカードの設定座標を算出し、設定する */
	for (int i = 0; i < HandCardCount; i++)
	{
		/* 設定座標を算出 */
		Struct_2D::POSITION SettingPos =
		{
			(SCREEN_SIZE_WIDE / 2) - ((HANDCARD_INTERVAL * (HandCardCount - 1)) / 2) + (HANDCARD_INTERVAL * i),
			HANDCARD_POS_Y
		};

		/* カードに設定座標を設定 */
		this->pDataList_Battle->GetHandCardList()[i]->SetSettingPos(SettingPos);
	}
}

// デッキのカード設定座標の設定
void Scene_Battle::CardPosition_DeckSetSettingPosting()
{
	/* デッキのカードの初期化処理時に実施 */
	for (auto& Deck : this->pDataList_GameResource->GetDeckCardList())
	{
		Deck->SetCardState(Card_Base::CARDSTATE_DECK);
		Deck->Update();
	}
}

// カードの更新処理
void Scene_Battle::Card_Update()
{
	/* 山札のカードの更新処理 */
	for (const auto& DeckCard : this->pDataList_Battle->GetDeckCardList())
	{
		DeckCard->Update();
	}

	/* 捨て札のカードの更新処理 */
	for (const auto& TrashCard : this->pDataList_Battle->GetTrashCardList())
	{
		TrashCard->Update();
	}

	/* ピックアップ中カードの更新処理 */
	if (this->pDataList_Battle->GetHoldCard() != nullptr)
	{
		this->pDataList_Battle->GetHoldCard()->Update();
	}

	/* バトルエリアのカードの更新処理 */
	for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	{
		auto BattleCard = this->pDataList_Battle->GetBattleAreaCardList(i);
		if (BattleCard != nullptr)
		{
			BattleCard->Update();
		}
	}

	/* 手札のカードの更新処理 */
	for (const auto& HandCard : this->pDataList_Battle->GetHandCardList())
	{
		HandCard->Update();
	}
}

// キャラクターの座標設定
void Scene_Battle::Character_SetPosition()
{
	/* 仲間キャラクターの座標設定＆更新 */
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		auto FriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
		if (FriendCharacter != nullptr)
		{
			Struct_2D::POSITION Pos =
			{
				CHARACTER_INTERVAL * (DataList_Battle::POSITION_MAX - i),
				CHARACTER_POS_Y
			};

			FriendCharacter->SetBasePos(Pos);
		}
	}

	/* 敵キャラクターの座標設定 */
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr)
		{
			Struct_2D::POSITION Pos =
			{
				SCREEN_SIZE_WIDE - (CHARACTER_INTERVAL * (DataList_Battle::POSITION_MAX - i)),
				CHARACTER_POS_Y
			};

			EnemyCharacter->SetBasePos(Pos);
		}
	}
}

// マウスが重なっているカードを取得
std::shared_ptr<Card_Base> Scene_Battle::GetMouseInCard()
{
	// 戻り値
	// std::shared_ptr<Card_Base> <- マウスが重なっているカードのポインタ(ないならnullptr)

	/* 確認用のカードリストを作成 */
	std::vector<std::shared_ptr<Card_Base>> CardList;

	// バトルエリアのカードを追加
	for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	{
		if (this->pDataList_Battle->GetBattleAreaCardList(i) != nullptr)
		{
			CardList.push_back(this->pDataList_Battle->GetBattleAreaCardList(i));
		}
	}

	// 手札のカードを追加
	for (const auto& HandCard : this->pDataList_Battle->GetHandCardList())
	{
		CardList.push_back(HandCard);
	}
	
	/* マウスの位置を定義 */
	Struct_2D::POSITION MousePosition = {
		gstKeyboardInputData.iMouseX,
		gstKeyboardInputData.iMouseY
	};

	/* カードリストを走査して、マウスが重なっているカードを取得 */
	for (const auto& Card : CardList)
	{
		/* ホールド中のカードであるなら対象外とする */
		if (Card == this->pDataList_Battle->GetHoldCard())
		{
			// ホールド中のカードであるなら次のカードへ
			continue;
		}

		/* カードの範囲を定義 */
		Struct_2D::RANGE CardRange = {
			Card->GetNowPos().iX - (Card_Base::CARD_WIDTH / 2),
			Card->GetNowPos().iY - (Card_Base::CARD_HEIGHT / 2),
			Card->GetNowPos().iX + (Card_Base::CARD_WIDTH / 2),
			Card->GetNowPos().iY + (Card_Base::CARD_HEIGHT / 2)
		};

		/* マウスがカードの範囲内にあるか確認 */
		if (PUBLIC_PROCESS::bPositionIn2DRangeCheck(MousePosition, CardRange))
		{
			// マウスがカードの範囲内にある場合、そのカードを返す
			return Card;
		}
	}

	// マウスが重なっているカードがない場合、nullptrを返す
	return nullptr;
}

// マウスが重なっているバトルエリアを取得
int Scene_Battle::GetMouseInBattleArea()
{
	// 戻り値
	// int <- マウスが重なっているバトルエリアのインデックス(ないなら-1)

	/* 戦闘終了しているなら処理を行わない */
	if (this->iBattlePhase == BATTLE_PHASE_BATTLE_END_WIN ||
		this->iBattlePhase == BATTLE_PHASE_BATTLE_END_GAMEOVER)
	{
		return -1;
	}

	/* バトルエリアのサイズを取得 */
	int SizeX, SizeY;
	GetGraphSize(*(this->Image_BattleArea[0]), &SizeX, &SizeY);

	/* バトルエリアを走査して、マウスが重なっているバトルエリアを取得 */
	for (int i = -2; i <= 2; i++)
	{
		/* バトルエリアの範囲を定義 */
		Struct_2D::RANGE BattleAreaRange = {
			(SCREEN_SIZE_WIDE / 2)	- (SizeX / 2) + (BATTLE_AREA_INTERVAL * i),
			BATTLE_AREA_POS_Y		- (SizeY / 2),
			(SCREEN_SIZE_WIDE / 2)	+ (SizeX / 2) + (BATTLE_AREA_INTERVAL * i),
			BATTLE_AREA_POS_Y		+ (SizeY / 2)
		};
		/* マウスの位置を定義 */
		Struct_2D::POSITION MousePosition = {
			gstKeyboardInputData.iMouseX,
			gstKeyboardInputData.iMouseY
		};
		/* マウスがバトルエリアの範囲内にあるか確認 */
		if (PUBLIC_PROCESS::bPositionIn2DRangeCheck(MousePosition, BattleAreaRange))
		{
			// マウスがバトルエリアの範囲内にある場合、そのバトルエリアのインデックスを返す
			return i + 2; // インデックスは0～4なので、+2して調整
		}
	}

	// マウスが重なっているバトルエリアがない場合、-1を返す
	return -1;
}

// キャラクターが死亡していて、削除するかの確認処理
void Scene_Battle::Character_Death_Delete_Check()
{
	/* 仲間キャラクターの死亡確認 */
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		auto FriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
		if (FriendCharacter != nullptr)
		{
			if (FriendCharacter->GetHP_Now() <= 0)
			{
				// 死亡している場合
				/* 削除するかの確認 */
				if (FriendCharacter->GetDeathDeleteFlg())
				{
					// 削除する場合
					/* nullptrに設定する */
					this->pDataList_Battle->SetFriendCharacter(i, nullptr);
				}

				/* 該当のキャラクターに紐づいた行動内容を削除する */
				for (auto& Effect : FriendCharacter->GetActionEffectList())
				{
					this->pDataList_Battle->RemoveEffect(Effect);
				}
			}
		}
	}

	/* 敵キャラクターの死亡確認 */
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr)
		{
			if (EnemyCharacter->GetHP_Now() <= 0)
			{
				// 死亡している場合
				/* 削除するかの確認 */
				if (EnemyCharacter->GetDeathDeleteFlg())
				{
					// 削除する場合
					/* nullptrに設定する */
					this->pDataList_Battle->SetEnemyCharacter(i, nullptr);
				}

				/* 該当のキャラクターに紐づいた行動内容を削除する */
				for (auto& Effect : EnemyCharacter->GetActionEffectList())
				{
					this->pDataList_Battle->RemoveEffect(Effect);
				}
			}
		}
	}
}

// ロスト対象のカードを確認し、ロストカード一覧に入れる
void Scene_Battle::CheckLostCard()
{
	/* ロストフラグが有効なカードをロストカードリストに移動する処理 */
	std::vector<std::shared_ptr<Card_Base>> AllDeckCardList = this->pDataList_Battle->GetAllDeckCardList();
	std::vector<std::shared_ptr<Card_Base>> LostCardList;

	/* ロストフラグが有効なカードを洗い出す */
	for (const auto& card : AllDeckCardList)
	{
		if (card && card->GetLostFlg())
		{
			LostCardList.push_back(card);
		}
	}

	/* ロストフラグが有効なカードをリストに登録＆ロスト状態に設定 */
	for (const auto& card : LostCardList)
	{
		this->pDataList_Battle->AddLostCard(card);
		card->SetCardState(Card_Base::CARDSTATE_LOST);
	}

	/* 該当のカードをデッキ、手札、捨て札リストから削除 */
	for (const auto& card : LostCardList)
	{
		this->pDataList_Battle->RemoveAllCard(card);      // AllDeckCardListから削除
		this->pDataList_Battle->RemoveDeckCard(card);     // DeckCardListから削除
		this->pDataList_Battle->RemoveHandCard(card);     // HandCardListから削除
		this->pDataList_Battle->RemoveTrashCard(card);    // TrashCardListから削除
	}
}

// チェイン数をリセット
void Scene_Battle::ResetChain()
{
	/* すべてのカードのチェイン数をリセット */
	std::vector<std::shared_ptr<Card_Base>> AllDeckCardList = this->pDataList_Battle->GetAllDeckCardList();
	for (const auto& card : AllDeckCardList)
	{
		if (card)
		{
			card->SetNowChainCount(0);
		}
	}
}

// 戦闘が終了しているかの確認
void Scene_Battle::CheckGameEnd()
{
	/* プレイヤーキャラクターが存在しているか確認 */
	bool bPlayerCharacterExist = false;
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		/* プレイヤーキャラクターであるかキャストして確認 */
		auto FriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
		if (FriendCharacter != nullptr)
		{
			auto CastedPlayerCharacter = std::dynamic_pointer_cast<Character_Player>(FriendCharacter);
			if (CastedPlayerCharacter != nullptr)
			{
				// プレイヤーキャラクターが存在している場合、処理を抜ける
				bPlayerCharacterExist = true;
				break;
			}
		}
	}
	if (!bPlayerCharacterExist)
	{
		// プレイヤーキャラクターが存在していない場合
		/* 戦闘終了(プレイヤー敗北)フェイズへ遷移 */
		this->iBattlePhase = BATTLE_PHASE_BATTLE_END_GAMEOVER;

		/* シーン"ゲームオーバー"を作成 */
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_GameOver>());

		return;
	}

	/* 体力が1以上の敵キャラクターが存在するか確認 */
	bool bEnemyCharacterExist = false;
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr)
		{
			if (EnemyCharacter->GetHP_Now() > 0)
			{
				// 体力が1以上の敵キャラクターが存在している場合、処理を抜ける
				bEnemyCharacterExist = true;
				break;
			}
		}
	}

	if (!bEnemyCharacterExist)
	{
		// 敵キャラクターが存在していない場合
		/* ドロップアイテムが盤面上に残っているなら処理を抜ける(ドロップアイテムが消滅するまで待つ) */
		if (this->DropItem_List.size() > 0)
		{
			return;
		}

		/* 戦闘終了(プレイヤー勝利)フェイズへ遷移 */
		this->iBattlePhase = BATTLE_PHASE_BATTLE_END_WIN;

		/* ドロップアイテム確認シーンを有効化 */
		this->pDataList_GameResource->SetDropItemCheckFlg(true);

		return;
	}
}

// 効果を使用したカードのトラッシュ処理
void Scene_Battle::Trash_UseCard(std::shared_ptr<Action_Effect_Base> pEffect)
{
	// 引数
	// pEffect <- 効果の内容

	/* カードが設定されているか確認 */
	if (pEffect->EffectCard == nullptr)
	{
		// 設定されていない場合、処理を終了
		return;
	}

	/* 効果の紐づいたカードをバトルエリア1～5から探す */
	this->iBattlePhase_NowBattleAreaNo = -1;
	for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	{
		if (this->pDataList_Battle->GetBattleAreaCardList(i) == pEffect->EffectCard)
		{
			// 見つかった場合、バトルエリア番号を保存して処理を抜ける
			this->iBattlePhase_NowBattleAreaNo = i;
			break;
		}
	}

	/* 見つかったか確認 */
	if (this->iBattlePhase_NowBattleAreaNo != -1)
	{
		/* 紐づいたカードをトラッシュする */
		this->pDataList_Battle->GetBattleAreaCardList(this->iBattlePhase_NowBattleAreaNo)->SetCardState(Card_Base::CARDSTATE_TRASH);
		this->pDataList_Battle->AddTrashCard(this->pDataList_Battle->GetBattleAreaCardList(this->iBattlePhase_NowBattleAreaNo));
		this->pDataList_Battle->RemoveBattleAreaCard(this->iBattlePhase_NowBattleAreaNo);
	}
}

// バトルエリアのカーソル接触時のアニメーションを設定
void Scene_Battle::Update_EmphasisAnim()
{
	/* バトルエリアにカーソルが接触しているか確認 */
	if (GetMouseInBattleArea() != -1)
	{
		// 接触している場合
		/* アニメーションの状態を進める */
		this->BattleArea_Anim_ChangeDelay--;
		if (this->BattleArea_Anim_ChangeDelay <= 0)
		{
			// 変更待機時間が0以下になった場合、アニメーションを進める
			this->BattleArea_Anim_ImageNo++;
			if (this->BattleArea_Anim_ImageNo >= EMPHASIS_ANIMATION_MAX)
			{
				// 最大数を超えた場合、最初の状態に戻す
				this->BattleArea_Anim_ImageNo = 0;
			}
			// 変更待機時間をリセット
			this->BattleArea_Anim_ChangeDelay = EMPHASIS_ANIMATION_SPEED;
		}
	}
	else
	{
		// 接触していない場合
		/* アニメーションの状態をリセットする */
		this->BattleArea_Anim_ImageNo		= 0;
		this->BattleArea_Anim_ChangeDelay	= EMPHASIS_ANIMATION_SPEED;
	}
}

// ドロップアイテムの更新処理
void Scene_Battle::Update_DropItem()
{
	/* 更新処理 */
	for (auto& DropItem : this->DropItem_List)
	{
		DropItem->Update();
	}

	/* 削除フラグが有効なら削除する */
	this->DropItem_List.erase(
		std::remove_if(
			this->DropItem_List.begin(),
			this->DropItem_List.end(),
			[](const std::shared_ptr<Drop_Item_Base>& DropItem) {
				return DropItem->GetDeleteFlg();
			}
		),
		this->DropItem_List.end()
	);
}

// ドロップアイテムの作成処理
void Scene_Battle::Create_DropItem()
{
	/* HPが0以下の敵キャラクターがいるか確認 */
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr)
		{
			if (EnemyCharacter->GetHP_Now() <= 0)
			{
				// 死亡している場合
				/* 所持コインがあるならコインを生成 */
				int Coin = EnemyCharacter->GetDropCoin();
				if (Coin > 0)
				{
					std::shared_ptr<Drop_Item_Base> NewCoin = std::make_shared<Drop_Item_Coin>();
					NewCoin->SetPosition(EnemyCharacter->GetBasePos());
					this->DropItem_List.push_back(NewCoin);

					/* ドロップコイン数を-1する */
					EnemyCharacter->SetDropCoin(Coin - 1);
				}

				/* 所持カードがあるならカードを生成 */
				auto DropCard = EnemyCharacter->GetDropCardList();
				if (DropCard.size() > 0)
				{
					std::shared_ptr<Drop_Item_Card> NewCard = std::make_shared<Drop_Item_Card>();
					NewCard->SetCard(DropCard.back());
					NewCard->SetPosition(EnemyCharacter->GetBasePos());
					this->DropItem_List.push_back(NewCard);

					/* ドロップカードリストから削除 */
					DropCard.pop_back();
					EnemyCharacter->SetDropCardList(DropCard);
				}
			}
		}
	}
}

// 鎖のアニメーションの更新
void Scene_Battle::Update_Chain_Anim()
{
	for (int i = 0; i < 4; i++)
	{
		this->Chain_Anim_Count[i] += (i + 1);
		if(this->Chain_Anim_Count[i] >= CHAIN_ANIMATION_MAX)
		{
			this->Chain_Anim_Count[i] -= CHAIN_ANIMATION_MAX;
		}
	}
}

// 説明文設定
void Scene_Battle::Update_Explanation()
{
	/* 各種フラグの初期化 */
	this->NumberDrawFlg_Deck	= false;   // 山札の枚数描写フラグを初期化
	this->NumberDrawFlg_Trash	= false;   // 捨て札の枚数描写フラグを初期化

	/* 優先順位は"行動内容の説明文 > カードの説明文"とする */
	bool ExplanationDrowFlg = false;

	/* すべてのカードにカーソルが重なっているか確認 */
	std::vector<std::shared_ptr<Card_Base>> AllDeckCardList = this->pDataList_Battle->GetAllDeckCardList();
	for (const auto& card : AllDeckCardList)
	{
		if (card)
		{
			/* カーソルが重なっているか */
			if (card->MouseInCard())
			{
				// 重なっている場合
				/* 説明文を描写すべき状態であるかを確認 */
				int CardState = card->GetCardState();
				if (CardState == Card_Base::CARDSTATE_HAND ||
					CardState == Card_Base::CARDSTATE_SETTING)
				{
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
				}
				// トラッシュ、デッキのカードである場合は、説明の代わりに枚数を表示する
				else if (CardState == Card_Base::CARDSTATE_TRASH)
				{
					/* 捨て札の枚数描写フラグを有効化 */
					this->NumberDrawFlg_Trash = true;
				}
				else if (CardState == Card_Base::CARDSTATE_DECK)
				{
					/* 山札の枚数描写フラグを有効化 */
					this->NumberDrawFlg_Deck = true;
				}
			}
		}
	}

	/* すべてのキャラクターにカーソルが重なっているか確認 */
	std::vector<std::shared_ptr<Character_Base>> AllCharacterList;
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		std::shared_ptr GetCharacter = this->pDataList_Battle->GetFriendCharacter(i);
		if (GetCharacter)	{ AllCharacterList.push_back(GetCharacter); }
		GetCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (GetCharacter)	{ AllCharacterList.push_back(GetCharacter); }
	}
	for (auto& Character : AllCharacterList)
	{
		/* カーソルが重なっているか */
		if (Character->MouseInCharacter())
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
			std::string ExplanationText = "/cysこうどう/ce";
			std::vector<std::shared_ptr<Action_Effect_Base>> ActionEffectList = Character->GetActionEffectList();
			for (int i = 0; i < ActionEffectList.size(); i++)
			{
				ExplanationText += "/n";
				ExplanationText += ActionEffectList[i]->ExplanationText;
			}
			this->UI_ExplanationText->SetExplanationText(ExplanationText);

			/* 設定座標を設定する */
			this->UI_ExplanationText->SetBasePos(Character->GetBasePos());

			/* 下方向に描写するよう設定 */
			this->UI_ExplanationText->SetUpwardDisplayFlg(false);

			/* 描写フラグを有効化する */
			ExplanationDrowFlg = true;
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

// 行動中のカードの強調表示アニメーションの更新
void Scene_Battle::Update_ActionCardArea()
{
	/* 行動中のカードの強調表示アニメーションの中心座標更新 */
	if (this->ActionCardInAreaNo == -1)
	{
		// 行動中のカードがバトルエリアにない場合
		/* 待機場所に設定 */
		this->ActionCardArea_Anim_CenterPos = { -1, BATTLE_AREA_POS_Y };
	}
	else
	{
		// 行動中のカードがバトルエリアにある場合
		/* 現在のX座標が-1であるか */
		bool bFirstSetFlg = (this->ActionCardArea_Anim_CenterPos.iX == -1);

		/* 現在行動を行っているバトルエリアの中心座標を取得 */
		Struct_2D::POSITION TargetCenterPos =
		{
			(SCREEN_SIZE_WIDE / 2) + (BATTLE_AREA_INTERVAL * (this->ActionCardInAreaNo - 2)),
			BATTLE_AREA_POS_Y
		};
		
		/* 強調表示アニメーションの中心座標を更新 */
		if (bFirstSetFlg)
		{
			// 最初の設定である場合、中心座標をターゲット座標に設定する
			this->ActionCardArea_Anim_CenterPos = TargetCenterPos;
		}
		else
		{
			// 最初の設定でない場合、中心座標をターゲット座標に徐々に近づける
			this->ActionCardArea_Anim_CenterPos.iX += (TargetCenterPos.iX - this->ActionCardArea_Anim_CenterPos.iX) / 3;
			this->ActionCardArea_Anim_CenterPos.iY += (TargetCenterPos.iY - this->ActionCardArea_Anim_CenterPos.iY) / 3;
		}
	}

	/* 戦闘行動カード強調表示アニメーションのカウント */
	this->ActionCard_Emphasis_AnimCount++;
	if (this->ActionCard_Emphasis_AnimCount >= 15)
	{
		this->ActionCard_Emphasis_AnimCount = 0;
	}
}

// デッキのシャッフル処理
void Scene_Battle::ShuffledDeck()
{
	/* デッキ内容を取得 */
	std::vector<std::shared_ptr<Card_Base>> DeckCardList = this->pDataList_GameResource->GetDeckCardList();

	/* ランダムになるようシャッフルする */
	std::vector<std::shared_ptr<Card_Base>> ShuffledDeckCardList;
	while (DeckCardList.size() > 0)
	{
		// ランダムなインデックスを取得
		int RandomIndex = GetRand(DeckCardList.size() - 1);
		// シャッフル後のデッキリストに追加
		ShuffledDeckCardList.push_back(DeckCardList[RandomIndex]);
		// 元のデッキリストから削除
		DeckCardList.erase(DeckCardList.begin() + RandomIndex);
	}
	DeckCardList = ShuffledDeckCardList;
	
	/* シャッフルしたデッキ内容を設定 */
	this->pDataList_Battle->SetDeckCardList(DeckCardList);
	
	/* デッキのカード設定座標の設定 */
	CardPosition_DeckSetSettingPosting();
}

// 戦闘スピード倍速ボタンの更新処理
void Scene_Battle::Update_DoubleSpeedButton()
{
	/* "戦闘スピード倍速"ボタンが入力されたならば */
	if (this->UI_SpeedDoubleButton->GetMouseOverFlg() && (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT))
	{
		/* 現在倍速フラグが有効であるか */
		if (this->BattleSpeedDoubleFlg)
		{
			// 倍速フラグが有効である場合
			/* 倍速フラグを無効にする */
			this->BattleSpeedDoubleFlg = false;

			/* テキストを変更する */
			this->UI_SpeedDoubleButton->SetButtonText("とうそくモード");
		}
		else
		{
			// 倍速フラグが無効である場合
			/* 倍速フラグを有効にする */
			this->BattleSpeedDoubleFlg = true;

			/* テキストを変更する */
			this->UI_SpeedDoubleButton->SetButtonText("ばいそくモード");
		}
	}
}
