/* シーン"バトル画面"の定義(描写部分) */

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <memory>
// ヘッダファイル
#include "Scene_Battle.h"
// 共通定義
#include "FunctionDefine.h"
#include "VariableDefine.h"
// 関連クラス
#include "Datalist_Image.h"
#include "DataList_Battle.h"
#include "Card_Base.h"
#include "Character_Base.h"
#include "Character_Player.h"
#include "Drop_Item.h"

// 背景描写
void Scene_Battle::Draw_BackGround()
{
	/* キャラクター背景 */
	DrawModiGraph(
		0,					0,
		SCREEN_SIZE_WIDE,	0,
		SCREEN_SIZE_WIDE,	250,
		0,					250,
		*(this->Image_BackGround[0]),
		TRUE
	);

	/* キャラクター足場 */
	DrawModiGraph(
		0, 250,
		SCREEN_SIZE_WIDE, 250,
		SCREEN_SIZE_WIDE, 300,
		0, 300,
		*(this->Image_BackGround[1]),
		TRUE
	);

	/* カード置き場 */
	DrawModiGraph(
		0, 300,
		SCREEN_SIZE_WIDE, 300,
		SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT,
		0, SCREEN_SIZE_HEIGHT,
		*(this->Image_BackGround[2]),
		TRUE
	);
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

// キャラクターの行動内容描写
void Scene_Battle::Draw_Character_Move()
{
	/* 仲間キャラクターの行動内容描写 */
	for (int i = 0; i < 3; i++)
	{
		auto FriendCharacter = this->pDataList_Battle->GetFriendCharacter(i);
		if (FriendCharacter != nullptr)
		{
			/* プレイヤーであるなら対象外とする */
			if (std::dynamic_pointer_cast<Character_Player>(FriendCharacter) != nullptr)
			{
				continue;
			}

			FriendCharacter->Draw_Action_Effect();
		}
	}

	/* 敵キャラクターの行動内容描写 */
	for (int i = 0; i < 3; i++)
	{
		auto EnemyCharacter = this->pDataList_Battle->GetEnemyCharacter(i);
		if (EnemyCharacter != nullptr)
		{
			EnemyCharacter->Draw_Action_Effect();
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
				Struct_2D::POSITION StartPos, EndPos;
				// 3:右下から左上
				StartPos	= BattleAreaPos_Right;
				EndPos		= BattleAreaPos_Left;
				StartPos.iY	+= 80;
				EndPos.iY	-= 80;
				StartPos.iX -= 60;
				EndPos.iX	+= 60;
				DRAW_FUNCTION::DrawChain_Anim(StartPos, EndPos, this->Chain_Anim_Count[3]);
				// 2:左下から右上
				StartPos	= BattleAreaPos_Left;
				EndPos		= BattleAreaPos_Right;
				StartPos.iY += 80;
				EndPos.iY	-= 80;
				StartPos.iX += 60;
				EndPos.iX	-= 60;
				DRAW_FUNCTION::DrawChain_Anim(StartPos, EndPos, this->Chain_Anim_Count[2]);
				// 1:ちょっと右上からちょっと左下
				StartPos	= BattleAreaPos_Right;
				EndPos		= BattleAreaPos_Left;
				StartPos.iY	-= 40;
				EndPos.iY	+= 40;
				DRAW_FUNCTION::DrawChain_Anim(StartPos, EndPos, this->Chain_Anim_Count[1]);
				// 0:ちょっと左上からちょっと右下
				StartPos	= BattleAreaPos_Left;
				EndPos		= BattleAreaPos_Right;
				StartPos.iY	-= 40;
				EndPos.iY	+= 40;
				DRAW_FUNCTION::DrawChain_Anim(StartPos, EndPos, this->Chain_Anim_Count[0]);

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
	/* バトルエリアを描写 */
	// ※カーソルが重なっているバトルエリアは別の画像を使用する
	int iSelectAreaNo = GetMouseInBattleArea();
	for (int i = 0; i < 5; i++)
	{
		/* フレーム */
		DrawExtendGraph(
			(SCREEN_SIZE_WIDE / 2)	- (BATTLE_AREA_WIDE / 2) + (BATTLE_AREA_INTERVAL * (i - 2)),
			BATTLE_AREA_POS_Y		- (BATTLE_AREA_HEIGHT / 2),
			(SCREEN_SIZE_WIDE / 2)	+ (BATTLE_AREA_WIDE / 2) + (BATTLE_AREA_INTERVAL * (i - 2)),
			BATTLE_AREA_POS_Y		+ (BATTLE_AREA_HEIGHT / 2),
			*(this->Image_BattleArea[(iSelectAreaNo == i) ? 1 : 0]), TRUE);

		/* 番号の背景 */
		DrawExtendGraph(
			(SCREEN_SIZE_WIDE / 2)	- (200 / 2) + (BATTLE_AREA_INTERVAL * (i - 2)),
			BATTLE_AREA_POS_Y		- (BATTLE_AREA_HEIGHT / 2) - (100),
			(SCREEN_SIZE_WIDE / 2)	+ (200 / 2) + (BATTLE_AREA_INTERVAL * (i - 2)),
			BATTLE_AREA_POS_Y		- (BATTLE_AREA_HEIGHT / 2),
			*(this->Image_BattleArea_No_Base), TRUE);

		/* 番号 */
		DrawExtendGraph(
			(SCREEN_SIZE_WIDE / 2)	- (64 / 2) + (BATTLE_AREA_INTERVAL * (i - 2)),
			BATTLE_AREA_POS_Y		- (BATTLE_AREA_HEIGHT / 2) - 50 - (64 / 2),
			(SCREEN_SIZE_WIDE / 2)	+ (64 / 2) + (BATTLE_AREA_INTERVAL * (i - 2)),
			BATTLE_AREA_POS_Y		- (BATTLE_AREA_HEIGHT / 2) - 50 + (64 / 2),
			*(this->Image_BattleArea_No_Number[i]), TRUE);

		/* 選択中であるならアニメーションを描写 */
		if (iSelectAreaNo == i)
		{
			/* アニメーション */
			DrawExtendGraph(
				(SCREEN_SIZE_WIDE / 2) - (BATTLE_AREA_WIDE / 2) + (BATTLE_AREA_INTERVAL * (i - 2)),
				BATTLE_AREA_POS_Y - (BATTLE_AREA_HEIGHT / 2),
				(SCREEN_SIZE_WIDE / 2) + (BATTLE_AREA_WIDE / 2) + (BATTLE_AREA_INTERVAL * (i - 2)),
				BATTLE_AREA_POS_Y + (BATTLE_AREA_HEIGHT / 2),
				*(this->Image_BattleArea_Emphasis_Anim[BattleArea_Anim_ImageNo]), TRUE);
		}
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

// ドロップアイテムの描写処理
void Scene_Battle::Draw_DropItem()
{
	for (auto& DropItem : this->DropItem_List)
	{
		DropItem->Draw();
	}
}

// 捨て札のカードの描写
void Scene_Battle::Draw_TrashCard()
{
	for(auto& TrashCard : this->pDataList_Battle->GetTrashCardList())
	{
		TrashCard->Draw();
	}
}

// 山札のカードの描写
void Scene_Battle::Draw_DeckCard()
{
	for(auto& DeckCard : this->pDataList_Battle->GetDeckCardList())
	{
		DeckCard->Draw();
	}
}
