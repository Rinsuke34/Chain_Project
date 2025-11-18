/* シーン"バトル画面"の定義(更新部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"
// 関連クラス
#include "Card_Base.h"
#include "DataList_Battle.h"

/* 各フェーズごとの更新処理 */
// "ターン開始時"の効果発動
void Scene_Battle::Update_EffectTurnStart()
{
	this->iBattlePhase = BATTLE_PHASE_DRAW_CARD;
}

// カードドロー
void Scene_Battle::Update_DrawCard()
{
	this->iBattlePhase = BATTLE_PHASE_ENEMY_ACTION_DECISION;
}

// 敵の行動決定
void Scene_Battle::Update_EnemyActionDecision()
{
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
				/* ホールド中のカードをバトルエリアに配置 */
				this->pDataList_Battle->SetBattleAreaCard(iBattleAreaIndex, this->pDataList_Battle->GetHoldCard());

				/* 配置したカードの設定座標をバトルエリアの位置に設定 */
				this->pDataList_Battle->GetHoldCard()->SetSettingPos({(SCREEN_SIZE_WIDE / 2) - (BATTLE_AREA_INTERVAL * (iBattleAreaIndex - 2)), (SCREEN_SIZE_HEIGHT / 2)});
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

//	this->iBattlePhase = BATTLE_PHASE_CARD_CHAIN_CHECK;
}

// カードのチェイン数確認
void Scene_Battle::Update_CardChainCheck()
{
	this->iBattlePhase = BATTLE_PHASE_EFFECT_ACTION_START;
}

// "行動開始時"の効果発動
void Scene_Battle::Update_EffectActionStart()
{
	this->iBattlePhase = BATTLE_PHASE_BATTLE_ACTION;
}

// 戦闘行動
void Scene_Battle::Update_BattleAction()
{
	this->iBattlePhase = BATTLE_PHASE_EFFECT_TRUN_END;
}

// "ターン終了時"の効果発動
void Scene_Battle::Update_EffectTurnEnd()
{
	this->iBattlePhase = BATTLE_PHASE_STATUS_EFFECT_ADVANCE;
}

// 状態変化のターン進行
void Scene_Battle::Update_StatusEffectAdvance()
{

}

/* その他 */
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
			(SCREEN_SIZE_WIDE	/ 2) - (SizeX / 2) - (BATTLE_AREA_INTERVAL * i),
			(SCREEN_SIZE_HEIGHT / 2) - (SizeY / 2),
			(SCREEN_SIZE_WIDE	/ 2) + (SizeX / 2) - (BATTLE_AREA_INTERVAL * i),
			(SCREEN_SIZE_HEIGHT	/ 2) + (SizeY / 2)
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
