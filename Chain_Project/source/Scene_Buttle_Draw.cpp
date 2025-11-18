/* シーン"バトル画面"の定義(描写部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Card_Base.h"

// 背景描写
void Scene_Battle::Draw_BackGround()
{

}

// バトルエリア描写
void Scene_Battle::Draw_BattleArea()
{
	/* バトルエリアのサイズを取得 */
	int SizeX, SizeY;
	GetGraphSize(*(this->Image_BattleArea[0]), &SizeX, &SizeY);

	/* バトルエリアを描写 */
	// ※-2～2までループさせて、中央から左右に2つずつ描写する
	// ※カーソルが重なっているバトルエリアは別の画像を使用する
	int iSelectAreaNo = GetMouseInBattleArea();
	for (int i = -2; i <= 2; i++)
	{
		DrawExtendGraph(
			(SCREEN_SIZE_WIDE / 2) - (BATTLE_AREA_INTERVAL * i) - (SizeX / 2),
			(SCREEN_SIZE_HEIGHT / 2) - (SizeY / 2),
			(SCREEN_SIZE_WIDE / 2) - (BATTLE_AREA_INTERVAL * i) + (SizeX / 2),
			(SCREEN_SIZE_HEIGHT / 2) + (SizeY / 2),
			*(this->Image_BattleArea[(iSelectAreaNo == i + 2) ? 1 : 0]), TRUE);
	}

	/* カードの描写 */
	for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	{
		auto BattleCard = this->pDataList_Battle->GetBattleAreaCardList(i);
		if (BattleCard != nullptr)
		{
			BattleCard->Draw();
		}
	}
}

// 手札描写
void Scene_Battle::Draw_HandCard()
{
	/* カードの描写 */
	std::vector<std::shared_ptr<Card_Base>> HandCardList = this->pDataList_Battle->GetHandCardList();
	for (const auto& HandCard : HandCardList)
	{
		HandCard->Draw();
	}
}

// ホールド中のカードを描写
void Scene_Battle::Draw_HoldCard()
{
	/* カードの描写 */
	if (this->pDataList_Battle->GetHoldCard() != nullptr)
	{
		this->pDataList_Battle->GetHoldCard()->Draw();
	}
}
