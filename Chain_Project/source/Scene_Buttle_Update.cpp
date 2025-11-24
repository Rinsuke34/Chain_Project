/* シーン"バトル画面"の定義(更新部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"
// 関連クラス
#include "Scene_UI_Button.h"
#include "Card_Base.h"
#include "DataList_Battle.h"
#include "Character_Base.h"

/* 各フェーズごとの更新処理 */
// "ターン開始時"の効果発動
void Scene_Battle::Update_EffectTurnStart()
{
	/* 各カードの"ターン開始時"効果を実行 */
	// バトルエリア
	for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	{
		auto BattleAreaCard = this->pDataList_Battle->GetBattleAreaCardList(i);
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_StartAction();
		}
	}
	// 山札
	for (auto& BattleAreaCard : this->pDataList_Battle->GetDeckCardList())
	{
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_StartTurn();
		}
	}
	// 手札
	for (auto& BattleAreaCard : this->pDataList_Battle->GetHandCardList())
	{
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_StartTurn();
		}
	}
	// 捨て札
	for (auto& BattleAreaCard : this->pDataList_Battle->GetTrashCardList())
	{
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_StartTurn();
		}
	}

	/* "カードドロー"フェイズへ遷移 */
	this->iBattlePhase = BATTLE_PHASE_DRAW_CARD;
}

// カードドロー
void Scene_Battle::Update_DrawCard()
{
	/* ドロー処理 */
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
				this->pDataList_Battle->GetHoldCard()->SetSettingPos({ (SCREEN_SIZE_WIDE / 2) - (BATTLE_AREA_INTERVAL * (iBattleAreaIndex - 2)), (SCREEN_SIZE_HEIGHT / 2) });
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
		auto BattleAreaCard = this->pDataList_Battle->GetBattleAreaCardList(i);
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_StartAction();
		}
	}
	// 山札
	for (auto& BattleAreaCard : this->pDataList_Battle->GetDeckCardList())
	{
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_StartAction();
		}
	}
	// 手札
	for (auto& BattleAreaCard : this->pDataList_Battle->GetHandCardList())
	{
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_StartAction();
		}
	}
	// 捨て札
	for (auto& BattleAreaCard : this->pDataList_Battle->GetTrashCardList())
	{
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_StartAction();
		}
	}

	/* "戦闘行動"フェイズへ遷移 */
	// ※バトルエリア1から順に処理を行うため、最初にバトルエリア1を設定しておく
	this->iBattlePhase = BATTLE_PHASE_BATTLE_ACTION;
	this->iBattlePhase_NowBattleAreaNo = DataList_Battle::BATTLE_AREA_1;
}

// 戦闘行動
void Scene_Battle::Update_BattleAction()
{
	///* バトルエリア1～5の順で処理を行う */
	//switch (this->iBattlePhase_NowBattleAreaNo)
	//{
	//	case DataList_Battle::BATTLE_AREA_1:
	//		this->iBattlePhase_NowBattleAreaNo = DataList_Battle::BATTLE_AREA_2;
	//		break;

	//	case DataList_Battle::BATTLE_AREA_2:
	//		this->iBattlePhase_NowBattleAreaNo = DataList_Battle::BATTLE_AREA_2;
	//		break;

	//	case DataList_Battle::BATTLE_AREA_3:
	//		this->iBattlePhase_NowBattleAreaNo = DataList_Battle::BATTLE_AREA_2;
	//		break;

	//	case DataList_Battle::BATTLE_AREA_4:
	//		this->iBattlePhase_NowBattleAreaNo = DataList_Battle::BATTLE_AREA_2;
	//		break;

	//	case DataList_Battle::BATTLE_AREA_5:
	//		this->iBattlePhase_NowBattleAreaNo = DataList_Battle::BATTLE_AREA_2;
	//		break;
	//}

	///* バトルエリアの1～5の順で処理を行う */
	//for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	//{
	//	/* バトルエリアにカードが設定されているか確認 */
	//	if (this->pDataList_Battle->GetBattleAreaCardList(i) != nullptr)
	//	{
	//		// カードが設定されている場合
	//		/* カードの戦闘行動を実行 */
	//		this->pDataList_Battle->GetBattleAreaCardList(i)->BattleAction();

	//		/* カードを捨て札リストに追加 */
	//		this->pDataList_Battle->AddTrashCard(this->pDataList_Battle->GetBattleAreaCardList(i));

	//		/* バトルエリアからカードを削除 */
	//		this->pDataList_Battle->RemoveBattleAreaCard(i);
	//	}
	//}

	/* 次のバトルエリアの処理を実施 */
	if (this->iBattlePhase_NowBattleAreaNo < DataList_Battle::BATTLE_AREA_5)
	{
		// 次のバトルエリアが存在する場合
		this->iBattlePhase_NowBattleAreaNo++;
	}
	else
	{
		// 次のバトルエリアが存在しない場合
		this->iBattlePhase_NowBattleAreaNo = DataList_Battle::BATTLE_AREA_1;

		/* 全てのバトルエリアの処理が終了したため、"ターン終了時"の効果発動フェイズへ遷移 */
		this->iBattlePhase = BATTLE_PHASE_EFFECT_TRUN_END;
		return;
	}
}

// "ターン終了時"の効果発動
void Scene_Battle::Update_EffectTurnEnd()
{
	/* 各カードの"ターン終了時"効果を実行 */
	// バトルエリア
	for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	{
		auto BattleAreaCard = this->pDataList_Battle->GetBattleAreaCardList(i);
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_EndTurn();
		}
	}
	// 山札
	for (auto& BattleAreaCard : this->pDataList_Battle->GetDeckCardList())
	{
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_EndTurn();
		}
	}
	// 手札
	for (auto& BattleAreaCard : this->pDataList_Battle->GetHandCardList())
	{
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_EndTurn();
		}
	}
	// 捨て札
	for (auto& BattleAreaCard : this->pDataList_Battle->GetTrashCardList())
	{
		if (BattleAreaCard != nullptr)
		{
			BattleAreaCard->Effect_EndTurn();
		}
	}

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
			(SCREEN_SIZE_WIDE / 2)	- (SizeX / 2) - (BATTLE_AREA_INTERVAL * i),
			BATTLE_AREA_POS_Y		- (SizeY / 2),
			(SCREEN_SIZE_WIDE / 2)	+ (SizeX / 2) - (BATTLE_AREA_INTERVAL * i),
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
