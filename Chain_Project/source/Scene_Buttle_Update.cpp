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

}

// カードドロー
void Scene_Battle::Update_DrawCard()
{

}

// 敵の行動決定
void Scene_Battle::Update_EnemyActionDecision()
{

}

// プレイヤーの行動決定
void Scene_Battle::Update_PlayerActionDecision()
{
	/* カードをホールド中であるか確認 */
	if (this->pDataList_Battle->GetHoldCard() != nullptr)
	{
		// ホールド中である場合
		/*  */
	}
	else
	{
		// ホールド中でない場合
		/* カーソルが重なっているカードを取得 */
		if (GetMouseInCard() != nullptr)
		{
			// 重なっているカードがある場合
			/* マウスの左ボタンがトリガーされているか確認 */
			if (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT)
			{
				// トリガーされている場合
				/* 取得したカードをホールド状態にする */
				this->pDataList_Battle->SetHoldCard(GetMouseInCard());
			}
		}
	}
}

// カードのチェイン数確認
void Scene_Battle::Update_CardChainCheck()
{

}

// "行動開始時"の効果発動
void Scene_Battle::Update_EffectActionStart()
{

}

// 戦闘行動
void Scene_Battle::Update_BattleAction()
{

}

// "ターン終了時"の効果発動
void Scene_Battle::Update_EffectTurnEnd()
{

}

// 状態変化のターン進行
void Scene_Battle::Update_StatusEffectAdvance()
{

}

/* その他 */
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
	for (const auto& card : this->pDataList_Battle->GetHandCardList())
	{
		CardList.push_back(card);
	}
	
	/* マウスの位置を定義 */
	Struct_2D::POSITION MousePosition = {
		gstKeyboardInputData.iMouseX,
		gstKeyboardInputData.iMouseY
	};

	/* カードリストを走査して、マウスが重なっているカードを取得 */
	for (const auto& card : CardList)
	{
		/* ホールド中のカードであるなら対象外とする */
		if (card == this->pDataList_Battle->GetHoldCard())
		{
			// ホールド中のカードであるなら次のカードへ
			continue;
		}

		/* カードの範囲を定義 */
		Struct_2D::RANGE CardRange = {
			card->GetNowPos().iX,
			card->GetNowPos().iX + Card_Base::CARD_WIDTH,
			card->GetNowPos().iY,
			card->GetNowPos().iY + Card_Base::CARD_HEIGHT
		};

		/* マウスがカードの範囲内にあるか確認 */
		if (PUBLIC_PROCESS::bPositionIn2DRangeCheck(MousePosition, CardRange))
		{
			// マウスがカードの範囲内にある場合、そのカードを返す
			return card;
		}
	}

	// マウスが重なっているカードがない場合、nullptrを返す
	return nullptr;
}

// マウスが重なっているバトルエリアを取得
int Scene_Battle::GetMouseInBattleArea()
{

}
