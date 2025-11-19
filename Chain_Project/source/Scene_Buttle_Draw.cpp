/* シーン"バトル画面"の定義(描写部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"
// 関連クラス
#include "DataList_Battle.h"
#include "Card_Base.h"
#include "Character_Base.h"

// 背景描写
void Scene_Battle::Draw_BackGround()
{

}

// キャラクター描写
void Scene_Battle::Draw_Character()
{
	/* 仲間キャラクターを描写 */
	for (int i = 0; i < 3; i++)
	{
		auto FriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
		if (FriendCharacter != nullptr)
		{
			FriendCharacter->Draw();
		}
	}

	/* 敵キャラクターを描写 */
	for (int i = 0; i < 3; i++)
	{
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr)
		{
			EnemyCharacter->Draw();
		}
	}
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
			(SCREEN_SIZE_WIDE / 2)	- (SizeX / 2) - (BATTLE_AREA_INTERVAL * i),
			BATTLE_AREA_POS_Y		- (SizeY / 2),
			(SCREEN_SIZE_WIDE / 2)	+ (SizeX / 2) - (BATTLE_AREA_INTERVAL * i),
			BATTLE_AREA_POS_Y		+ (SizeY / 2),
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
