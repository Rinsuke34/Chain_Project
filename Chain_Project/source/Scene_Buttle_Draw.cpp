/* シーン"バトル画面"の定義(描写部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Battle.h"
// 共通定義
#include "FunctionDefine.h"
// 関連クラス
#include "Datalist_Image.h"
#include "DataList_Battle.h"
#include "Card_Base.h"
#include "Character_Base.h"

// 背景描写
void Scene_Battle::Draw_BackGround()
{
	/* 仮 */
	DrawBox(0, 0, SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT, GetColor(0, 128, 0), TRUE);

	/* テスト */
	DrawLine(0, 300, SCREEN_SIZE_WIDE, 300, GetColor(255, 255, 255), 5);
	DrawLine(0, 200, SCREEN_SIZE_WIDE, 200, GetColor(255, 255, 255), 5);

	DrawFormatString(50, 50, GetColor(0, 0, 0), "デッキ枚数 : %llu", static_cast<unsigned long long>(this->pDataList_Battle->GetDeckCardList().size()));
	DrawFormatString(50, 100, GetColor(0, 0, 0), "トラッシュ枚数 : %llu", static_cast<unsigned long long>(this->pDataList_Battle->GetTrashCardList().size()));
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

// 鎖(チェイン時)描写
void Scene_Battle::Draw_Chain()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* チェインが発生しているならそのスートを描写する */
	for (int i = 0; i < DataList_Battle::CHAIN_SUTE_AREA_MAX; i++)
	{
		std::vector<std::string> ChainSuiteList = this->pDataList_Battle->GetChain_Suite_List(i);
		if (ChainSuiteList.size() > 0)
		{
			// チェインが発生している場合
			for (int j = 0; j < ChainSuiteList.size(); j++)
			{
				/* スート画像を取得 */
				std::string ImageFilePath = "Card_Suit/" + ChainSuiteList[j];
				std::shared_ptr<int> SuteImage = pDataList_Image->iGetImageHandle(ImageFilePath);
				ImageFilePath = "Card_Commoon/Chain_SuitFrame";
				std::shared_ptr<int> SuteFrame = pDataList_Image->iGetImageHandle(ImageFilePath);

				/* 左右のバトルエリアの中心座標を取得 */
				Struct_2D::POSITION BattleAreaPos_Left = {
					(SCREEN_SIZE_WIDE / 2) + BATTLE_AREA_INTERVAL * ( i - 2 ),
					BATTLE_AREA_POS_Y
				};
				Struct_2D::POSITION BattleAreaPos_Right = {
					(SCREEN_SIZE_WIDE / 2) + BATTLE_AREA_INTERVAL * ( i - 1 ),
					BATTLE_AREA_POS_Y
				};

				/* 鎖を描写 */
				DRAW_FUNCTION::DrawChain(BattleAreaPos_Left, BattleAreaPos_Right);
				
				/* スート画像のサイズを取得 */
				int SizeX, SizeY;
				GetGraphSize(*(SuteFrame), &SizeX, &SizeY);

				/* スートを描写 */
				DrawModiGraph(
					(BattleAreaPos_Right.iX + BattleAreaPos_Left.iX) / 2 - (SizeX / 2), (BattleAreaPos_Right.iY + BattleAreaPos_Left.iY) / 2 - (SizeY / 2),
					(BattleAreaPos_Right.iX + BattleAreaPos_Left.iX) / 2 + (SizeX / 2), (BattleAreaPos_Right.iY + BattleAreaPos_Left.iY) / 2 - (SizeY / 2),
					(BattleAreaPos_Right.iX + BattleAreaPos_Left.iX) / 2 + (SizeX / 2), (BattleAreaPos_Right.iY + BattleAreaPos_Left.iY) / 2 + (SizeY / 2),
					(BattleAreaPos_Right.iX + BattleAreaPos_Left.iX) / 2 - (SizeX / 2), (BattleAreaPos_Right.iY + BattleAreaPos_Left.iY) / 2 + (SizeY / 2),
					*(SuteFrame),
					TRUE
				);

				DrawModiGraph(
					(BattleAreaPos_Right.iX + BattleAreaPos_Left.iX) / 2 - (SizeX / 2), (BattleAreaPos_Right.iY + BattleAreaPos_Left.iY) / 2 - (SizeY / 2),
					(BattleAreaPos_Right.iX + BattleAreaPos_Left.iX) / 2 + (SizeX / 2), (BattleAreaPos_Right.iY + BattleAreaPos_Left.iY) / 2 - (SizeY / 2),
					(BattleAreaPos_Right.iX + BattleAreaPos_Left.iX) / 2 + (SizeX / 2), (BattleAreaPos_Right.iY + BattleAreaPos_Left.iY) / 2 + (SizeY / 2),
					(BattleAreaPos_Right.iX + BattleAreaPos_Left.iX) / 2 - (SizeX / 2), (BattleAreaPos_Right.iY + BattleAreaPos_Left.iY) / 2 + (SizeY / 2),
					*(SuteImage),
					TRUE
				);
			}
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
			(SCREEN_SIZE_WIDE / 2)	- (SizeX / 2) + (BATTLE_AREA_INTERVAL * i),
			BATTLE_AREA_POS_Y		- (SizeY / 2),
			(SCREEN_SIZE_WIDE / 2)	+ (SizeX / 2) + (BATTLE_AREA_INTERVAL * i),
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
