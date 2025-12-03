/* シーン"バトル画面"の定義(更新部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"
// 関連クラス
#include "Scene_UI_Button.h"
#include "Card_Base.h"
#include "Card_Spell_Base.h"
#include "DataList_Battle.h"
#include "Character_Base.h"

/* 各フェーズごとの更新処理 */
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
	/* ドロー処理 */
	this->bReloadFlg = false;
	for (int i = 0; i < 5; i++)
	{
		/* デッキにカードが存在するか確認 */
		if (this->pDataList_Battle->GetDeckCardList().size() > 0)
		{
			// 存在する場合
			/* デッキの先頭のカードを手札に移動 */
			std::shared_ptr<Card_Base> pDrawCard = this->pDataList_Battle->GetDeckCardList().front();
			this->pDataList_Battle->AddHandCard(pDrawCard);
			this->pDataList_Battle->RemoveDeckCard(pDrawCard);
		}
		else if(this->pDataList_Battle->GetHandCardList().size() == 0)
		{
			// デッキにカード無しかつ、手札が0枚の場合
			/* リロードフラグを有効化する */
			this->bReloadFlg = true;			

			/* 捨て札のカードを手札に戻す */
			int TrashSize = static_cast<int>(this->pDataList_Battle->GetTrashCardList().size());
			for (int i = 0; i < TrashSize; i++)
			{
				std::vector<std::shared_ptr<Card_Base>> TrashCardList = this->pDataList_Battle->GetTrashCardList();
				std::shared_ptr<Card_Base> pCard = TrashCardList[0];
				if (pCard != nullptr)
				{
					this->pDataList_Battle->AddDeckCard(pCard);
					this->pDataList_Battle->RemoveTrashCard(pCard);
				}
			}
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
		/* "カードチェイン数確認"フェイズへ遷移 */
		this->iBattlePhase = BATTLE_PHASE_CARD_CHAIN_CHECK;
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

					/* バトルエリアのカードを削除 */
					this->pDataList_Battle->RemoveBattleAreaCard(iBattleAreaIndex);
				}

				/* ホールド中のカードをバトルエリアに配置 */
				this->pDataList_Battle->SetBattleAreaCard(iBattleAreaIndex, this->pDataList_Battle->GetHoldCard());

				/* 配置したカードの設定座標をバトルエリアの位置に設定 */
				this->pDataList_Battle->GetHoldCard()->SetSettingPos({ (SCREEN_SIZE_WIDE / 2) + (BATTLE_AREA_INTERVAL * (iBattleAreaIndex - 2)), (SCREEN_SIZE_HEIGHT / 2) });
			}
			else
			{
				// 接触していない場合
				/* ホールド中のカードを手札に設定する */
				this->pDataList_Battle->AddHandCard(this->pDataList_Battle->GetHoldCard());

				/* 戻したカードの設定座標を手札の位置に設定 */
				this->pDataList_Battle->GetHoldCard()->SetSettingPos({0, 0});
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
		/* "カードチェイン数確認"フェイズへ遷移 */
		this->iBattlePhase = BATTLE_PHASE_CARD_CHAIN_CHECK;
	}
}

// カードのチェイン数確認
void Scene_Battle::Update_CardChainCheck()
{
	/* 各バトルエリアにチェイン数を設定する */
	int CheckStartAreaNo	= -1;
	int CheinCount			= 0;
	for (int BattleAreaNo = 0; BattleAreaNo < DataList_Battle::BATTLE_AREA_MAX; BattleAreaNo++)
	{
		/* 確認開始エリア番号が無効であるか */
		if (CheckStartAreaNo == -1)
		{
			// 無効である場合
			/* カードが設定されているならその地点を確認開始箇所とする */
			if (this->pDataList_Battle->GetBattleAreaCardList(BattleAreaNo))
			{
				CheckStartAreaNo	= BattleAreaNo;
				CheinCount			= 0;
			}
		}
		else
		{
			// 有効である場合
			/* 前のバトルエリアのカードとチェインがつながっているか確認 */
			if (this->pDataList_Battle->GetChain_Suite_List(BattleAreaNo - 1).size() > 0)
			{
				// つながっている場合
				/* チェインカウントを加算する */
				CheinCount += this->pDataList_Battle->GetChain_Suite_List(BattleAreaNo - 1).size();

				/* 現在のバトルエリアのカードにチェイン数を設定 */
				this->pDataList_Battle->GetBattleAreaCardList(BattleAreaNo)->SetNowChainCount(CheinCount);
			}
			else
			{
				// つながっていない場合
				/* カードが設定されているならその地点を確認開始箇所とする */
				CheckStartAreaNo	= -1;
				CheinCount			= 0;
				if (this->pDataList_Battle->GetBattleAreaCardList(BattleAreaNo))
				{
					CheckStartAreaNo = BattleAreaNo;
				}
			}
		}
	}

	/* "行動開始時の効果発動"フェイズへ遷移 */
	this->iBattlePhase = BATTLE_PHASE_EFFECT_ACTION_START;
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

		/* 効果を優先順位順に並び変える */
		// スペル > 状態異常付与 > 回復 > シールド付与 > ダメージ
		{
			std::vector<std::shared_ptr<Card_Effect_Base>> EffectList = this->pDataList_Battle->GetEffectList(i);

			/* スペルの効果を抽出 */
			std::vector<std::shared_ptr<Card_Effect_Extra>> EffectList_Spell;
			for (auto& effect : EffectList)
			{
				// スペルの効果であるか確認
				if (std::shared_ptr<Card_Effect_Extra> spellEffect = std::dynamic_pointer_cast<Card_Effect_Extra>(effect))
				{
					EffectList_Spell.push_back(spellEffect);
				}
			}

			/* 状態異常付与系の効果を抽出 */
			std::vector<std::shared_ptr<Card_Effect_StatusAilment>> StatusEffectList_StatusAilment;
			for (const auto& effect : EffectList)
			{
				// 状態異常付与系の効果であるか確認
				if (std::shared_ptr<Card_Effect_StatusAilment> statusAilmentEffect = std::dynamic_pointer_cast<Card_Effect_StatusAilment>(effect))
				{
					StatusEffectList_StatusAilment.push_back(statusAilmentEffect);
				}
			}

			/* 回復系の効果を抽出 */
			std::vector<std::shared_ptr<Card_Effect_Heal>> EffectList_Heal;
			for (auto& effect : EffectList)
			{
				// 回復系の効果であるか確認
				if (std::shared_ptr<Card_Effect_Heal> healEffect = std::dynamic_pointer_cast<Card_Effect_Heal>(effect))
				{
					EffectList_Heal.push_back(healEffect);
				}
			}

			/* シールド付与系の効果を抽出 */
			std::vector<std::shared_ptr<Card_Effect_Defence>> EffectList_Shield;
			for (auto& effect : EffectList)
			{
				// シールド付与系の効果であるか確認
				if (std::shared_ptr<Card_Effect_Defence> shieldEffect = std::dynamic_pointer_cast<Card_Effect_Defence>(effect))
				{
					EffectList_Shield.push_back(shieldEffect);
				}
			}

			/* ダメージ系の効果を抽出 */
			std::vector<std::shared_ptr<Card_Effect_Attack>> EffectList_Damage;
			for (auto& effect : EffectList)
			{
				// ダメージ系の効果であるか確認
				if (std::shared_ptr<Card_Effect_Attack> damageEffect = std::dynamic_pointer_cast<Card_Effect_Attack>(effect))
				{
					EffectList_Damage.push_back(damageEffect);
				}
			}

			/* 優先順位順に効果を再設定する */
			EffectList.clear();
			for (auto& effect : EffectList_Spell)				{ EffectList.push_back(effect); }
			for (auto& effect : StatusEffectList_StatusAilment)	{ EffectList.push_back(effect); }
			for (auto& effect : EffectList_Heal)				{ EffectList.push_back(effect); }
			for (auto& effect : EffectList_Shield)				{ EffectList.push_back(effect); }
			for (auto& effect : EffectList_Damage)				{ EffectList.push_back(effect); }
			this->pDataList_Battle->SetEffectList(EffectList, i);
		}
	}

	/* "戦闘行動"フェイズへ遷移 */
	// ※バトルエリア1から順に処理を行うため、最初にバトルエリア1を設定しておく
	this->iBattlePhase_NowBattleAreaNo = DataList_Battle::BATTLE_AREA_1;
	this->iBattlePhase = BATTLE_PHASE_BATTLE_ACTION;
}

// 戦闘行動
void Scene_Battle::Update_BattleAction()
{
	/* 戦闘行動間のディレイ時間が完了しているか確認 */
	if (this->iBattleActionDelay > 0)
	{
		// 完了していない場合
		/* ディレイ時間を減少 */
		this->iBattleActionDelay--;
		return;
	}

	/* 現在のバトルエリアの与効果を取得 */
	std::vector<std::shared_ptr<Card_Effect_Base>> EffectList = this->pDataList_Battle->GetEffectList(this->iBattlePhase_NowBattleAreaNo);

	/* このバトルエリアの処理が完了しているか確認 */
	if (EffectList.size() == 0)
	{
		// 完了している場合
		/* バトルエリアにカードが設定されているか確認 */
		if (this->pDataList_Battle->GetBattleAreaCardList(this->iBattlePhase_NowBattleAreaNo) != nullptr)
		{
			// 設定されているなら
			/* カードを捨て札リストに追加 */
			this->pDataList_Battle->AddTrashCard(this->pDataList_Battle->GetBattleAreaCardList(this->iBattlePhase_NowBattleAreaNo));

			/* バトルエリアからカードを削除 */
			this->pDataList_Battle->RemoveBattleAreaCard(this->iBattlePhase_NowBattleAreaNo);
		}

		/* 次のバトルエリアの処理を実施 */
		if (this->iBattlePhase_NowBattleAreaNo < DataList_Battle::BATTLE_AREA_5)
		{
			// 次のバトルエリアが存在する場合
			this->iBattlePhase_NowBattleAreaNo++;
			return;
		}
		else
		{
			// 次のバトルエリアが存在しない場合
			/* 全てのバトルエリアの処理が終了したため、"ターン終了時"の効果発動フェイズへ遷移 */
			this->iBattlePhase = BATTLE_PHASE_EFFECT_TRUN_END;
			return;
		}
	}

	/* 与効果の内容を取得＆リスト上からの削除 */
	std::shared_ptr<Card_Effect_Base> pEffect = EffectList.front();
	this->pDataList_Battle->RemoveEffect(pEffect, this->iBattlePhase_NowBattleAreaNo);

	/* 効果の内容に応じた処理を実行 */
	UseCardEffect(pEffect, this->iBattlePhase_NowBattleAreaNo);

	/* キャラクターが死亡しているか確認 */
	Character_Death_Check();

	/* 効果処理後のディレイ時間を設定 */
	this->iBattleActionDelay = 30;
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

	/* 全キャラクターのシールドをリセット */
	for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
	{
		// 仲間
		auto FriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
		if (FriendCharacter != nullptr)
		{
			FriendCharacter->ShieldReset_EndTurn();
		}
		// 敵
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr)
		{
			EnemyCharacter->ShieldReset_EndTurn();
		}
	}

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

// カードの座標補間
void Scene_Battle::CardPosition_Interpolation()
{
	/* バトルエリアのカードの座標補間 */
	for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	{
		auto BattleCard = this->pDataList_Battle->GetBattleAreaCardList(i);
		if (BattleCard != nullptr)
		{
			BattleCard->Position_Interpolation();
		}
	}

	/* 手札のカードの座標補間 */
	for (const auto& HandCard : this->pDataList_Battle->GetHandCardList())
	{
		HandCard->Position_Interpolation();
	}
}

// キャラクターの座標の設定
void Scene_Battle::CharacterPosition_Setup()
{
	/* 仲間キャラクターの座標設定 */
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

// カード効果の使用
void Scene_Battle::UseCardEffect(std::shared_ptr<Card_Effect_Base> Effect, int AreaNo)
{
	// 引数
	// std::shared_ptr<Card_Effect_Base>	Effect	<- 使用するカード効果のポインタ
	// int									AreaNo  <- 効果を使用するバトルエリアのインデックス

	/* 与効果の種類に応じた処理を実行 */
	// 攻撃系効果
	if (std::shared_ptr<Card_Effect_Attack> AttackEffect = std::dynamic_pointer_cast<Card_Effect_Attack>(Effect))
	{
		/* 効果の対象キャラクターを取得 */
		if (AttackEffect->Target_Camp == Character_Base::CAMP_ENEMY)
		{
			// 敵キャラクターが対象である場合
			/* 全体攻撃であるか確認 */
			if (AttackEffect->AllRange)
			{
				// 全体攻撃である場合
				/* 全ての敵キャラクターにダメージ処理を実行 */
				for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
				{
					std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
					if (TargetEnemyCharacter != nullptr)
					{
						// 対象の敵キャラクターが存在する場合
						/* ダメージ処理を実行 */
						TargetEnemyCharacter->Damage(AttackEffect->DamageAmount);
					}
				}

				/* 攻撃リアクションを設定 */
				if (AttackEffect->EffectUser)
				{
					AttackEffect->EffectUser->Action_Attack();
				}
			}
			else
			{
				// 単体攻撃である場合
				/* 対象の立ち位置の敵を取得 */
				std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(AttackEffect->Target_Position);
				if (TargetEnemyCharacter != nullptr)
				{
					// 対象の敵キャラクターが存在する場合
					/* ダメージ処理を実行 */
					TargetEnemyCharacter->Damage(AttackEffect->DamageAmount);

					/* 攻撃リアクションを設定 */
					if (AttackEffect->EffectUser)
					{
						AttackEffect->EffectUser->Action_Attack();
					}
				}
			}
		}
		else
		{
			// 仲間キャラクターが対象である場合
			/* 全体攻撃であるか確認 */
			if (AttackEffect->AllRange)
			{
				// 全体攻撃である場合
				/* 全ての仲間キャラクターにダメージ処理を実行 */
				for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
				{
					std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
					if (TargetFriendCharacter != nullptr)
					{
						// 対象の仲間キャラクターが存在する場合
						/* ダメージ処理を実行 */
						TargetFriendCharacter->Damage(AttackEffect->DamageAmount);
					}
				}

				/* 攻撃リアクションを設定 */
				if (AttackEffect->EffectUser)
				{
					AttackEffect->EffectUser->Action_Attack();
				}
			}
			else
			{
				// 単体攻撃である場合
				/* 対象の立ち位置の仲間を取得 */
				std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(AttackEffect->Target_Position);
				if (TargetFriendCharacter != nullptr)
				{
					// 対象の仲間キャラクターが存在する場合
					/* ダメージ処理を実行 */
					TargetFriendCharacter->Damage(AttackEffect->DamageAmount);

					/* 攻撃リアクションを設定 */
					if (AttackEffect->EffectUser)
					{
						AttackEffect->EffectUser->Action_Attack();
					}
				}
			}
		}
	}
	// 防御系効果
	else if (std::shared_ptr<Card_Effect_Defence> DefenceEffect = std::dynamic_pointer_cast<Card_Effect_Defence>(Effect))
	{
		/* 効果の対象キャラクターを取得 */
		if (DefenceEffect->Target_Camp == Character_Base::CAMP_ENEMY)
		{
			// 敵キャラクターが対象である場合
			/* 全体に付与されるか確認 */
			if (DefenceEffect->AllRange)
			{
				// 全体に付与される場合
				/* 全ての敵キャラクターにシールド付与処理を実行 */
				for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
				{
					std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
					if (TargetEnemyCharacter != nullptr)
					{
						// 対象の敵キャラクターが存在する場合
						/* シールド付与処理を実行 */
						TargetEnemyCharacter->AddShield(DefenceEffect->ShieldAmount);
					}
				}

				/* バフ付与リアクションを設定 */
				if (DefenceEffect->EffectUser)
				{
					DefenceEffect->EffectUser->Action_AddBuff();
				}
			}
			else
			{
				// 単体に付与される場合
				/* 対象の立ち位置の敵を取得 */
				std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(DefenceEffect->Target_Position);
				if (TargetEnemyCharacter != nullptr)
				{
					// 対象の敵キャラクターが存在する場合
					/* シールド付与処理を実行 */
					TargetEnemyCharacter->AddShield(DefenceEffect->ShieldAmount);

					/* バフ付与リアクションを設定 */
					if (DefenceEffect->EffectUser)
					{
						DefenceEffect->EffectUser->Action_AddBuff();
					}
				}
			}
		}
		else
		{
			// 仲間キャラクターが対象である場合
			/* 全体に付与されるか確認 */
			if(DefenceEffect->AllRange)
			{
				// 全体に付与される場合
				/* 全ての仲間キャラクターにシールド付与処理を実行 */
				for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
				{
					std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
					if (TargetFriendCharacter != nullptr)
					{
						// 対象の仲間キャラクターが存在する場合
						/* シールド付与処理を実行 */
						TargetFriendCharacter->AddShield(DefenceEffect->ShieldAmount);
					}
				}

				/* バフ付与リアクションを設定 */
				if (DefenceEffect->EffectUser)
				{
					DefenceEffect->EffectUser->Action_AddBuff();
				}
			}
			else
			{
				// 単体に付与される場合
				/* 対象の立ち位置の仲間を取得 */
				std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(DefenceEffect->Target_Position);
				if (TargetFriendCharacter != nullptr)
				{
					// 対象の仲間キャラクターが存在する場合
					/* シールド付与処理を実行 */
					TargetFriendCharacter->AddShield(DefenceEffect->ShieldAmount);

					/* バフ付与リアクションを設定 */
					if (DefenceEffect->EffectUser)
					{
						DefenceEffect->EffectUser->Action_AddBuff();
					}

				}
			}
		}
	}
	// 回復系効果
	else if (std::shared_ptr<Card_Effect_Heal> HealEffect = std::dynamic_pointer_cast<Card_Effect_Heal>(Effect))
	{
		/* 効果の対象キャラクターを取得 */
		if (HealEffect->Target_Camp == Character_Base::CAMP_ENEMY)
		{
			// 敵キャラクターが対象である場合
			/* 全体回復であるか確認 */
			if (HealEffect->AllRange)
			{
				// 全体回復である場合
				/* 全ての敵キャラクターに回復処理を実行 */
				for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
				{
					std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
					if (TargetEnemyCharacter != nullptr)
					{
						// 対象の敵キャラクターが存在する場合
						/* 回復処理を実行 */
						TargetEnemyCharacter->Heal(HealEffect->HealAmount);
					}
				}

				/* バフ付与リアクションを設定 */
				if (HealEffect->EffectUser)
				{
					HealEffect->EffectUser->Action_AddBuff();
				}
			}
			else
			{
				// 単体回復である場合
				/* 対象の立ち位置の敵を取得 */
				std::shared_ptr<Character_Base> TargetEnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(HealEffect->Target_Position);
				if (TargetEnemyCharacter != nullptr)
				{
					// 対象の敵キャラクターが存在する場合
					/* 回復処理を実行 */
					TargetEnemyCharacter->Heal(HealEffect->HealAmount);

					/* バフ付与リアクションを設定 */
					if (HealEffect->EffectUser)
					{
						HealEffect->EffectUser->Action_AddBuff();
					}
				}
			}
		}
		else
		{
			// 仲間キャラクターが対象である場合
			/* 全体回復であるか確認 */
			if (HealEffect->AllRange)
			{
				// 全体回復である場合
				/* 全ての仲間キャラクターに回復処理を実行 */
				for (int i = 0; i < DataList_Battle::POSITION_MAX; i++)
				{
					std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
					if (TargetFriendCharacter != nullptr)
					{
						// 対象の仲間キャラクターが存在する場合
						/* 回復処理を実行 */
						TargetFriendCharacter->Heal(HealEffect->HealAmount);
					}
				}
				/* バフ付与リアクションを設定 */
				if (HealEffect->EffectUser)
				{
					HealEffect->EffectUser->Action_AddBuff();
				}
			}
			else
			{
				// 単体回復である場合
				/* 対象の立ち位置の仲間を取得 */
				std::shared_ptr<Character_Base> TargetFriendCharacter = this->pDataList_Battle->GetFriendCharacter(HealEffect->Target_Position);
				if (TargetFriendCharacter != nullptr)
				{
					// 対象の仲間キャラクターが存在する場合
					/* 回復処理を実行 */
					TargetFriendCharacter->Heal(HealEffect->HealAmount);

					/* バフ付与リアクションを設定 */
					if (HealEffect->EffectUser)
					{
						HealEffect->EffectUser->Action_AddBuff();
					}
				}
			}
		}
	}
	// カード効果(特殊効果)
	else if (std::shared_ptr<Card_Effect_Extra> SpellEffect = std::dynamic_pointer_cast<Card_Effect_Extra>(Effect))
	{
		/* カード効果(特殊効果)を実行 */
		SpellEffect->ExEffectCard->Card_Effect_Extra_Process();
	}
	// 状態異常付与系処理
}

// キャラクターが死亡しているか確認
void Scene_Battle::Character_Death_Check()
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
				/* nullptrに設定する */
				this->pDataList_Battle->SetFriendCharacter(i, nullptr);
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
				/* nullptrに設定する */
				this->pDataList_Battle->SetEnemyCharacter(i, nullptr);
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

	/* ロストフラグが有効なカードをリストに登録 */
	for (const auto& card : LostCardList)
	{
		this->pDataList_Battle->AddLostCard(card);
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
