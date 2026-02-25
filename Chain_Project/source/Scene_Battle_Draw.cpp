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
	// 描写頂点を算出
	static const int TILE_POINT_MAX		= 32;
	static const int TILE_UP_WIDE		= 64;
	static const int TILE_UNDER_WIDE	= 66;
	int UpPosX[TILE_POINT_MAX],UnderPosX[TILE_POINT_MAX];
	for (int i = 0; i < TILE_POINT_MAX; i++)
	{
		UpPosX[i]		= (SCREEN_SIZE_WIDE / 2) - ((TILE_POINT_MAX / 2) * TILE_UP_WIDE)		+ (TILE_UP_WIDE * i);
		UnderPosX[i]	= (SCREEN_SIZE_WIDE / 2) - ((TILE_POINT_MAX / 2) * TILE_UNDER_WIDE)	+ (TILE_UNDER_WIDE * i);
	}
	// 描写
	for (int i = 0; i < TILE_POINT_MAX - 1; i++)
	{
		DrawModiGraph(
			UpPosX[i],			250,
			UpPosX[i + 1],		250,
			UnderPosX[i + 1],	300,
			UnderPosX[i],		300,
			*(this->Image_BackGround[1]),
			TRUE
		);
	}

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
			(SCREEN_SIZE_WIDE / 2)	- (64 / 2) + (BATTLE_AREA_INTERVAL * (i - 2))	- 20,
			BATTLE_AREA_POS_Y		- (BATTLE_AREA_HEIGHT / 2) - 50 - (64 / 2)		- 20,
			(SCREEN_SIZE_WIDE / 2)	+ (64 / 2) + (BATTLE_AREA_INTERVAL * (i - 2))	+ 20,
			BATTLE_AREA_POS_Y		- (BATTLE_AREA_HEIGHT / 2) - 50 + (64 / 2)		+ 20,
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

	/* カードの強調表示アニメーションを描写 */
	if (this->ActionCardInAreaNo != -1)
	{
		int EmphasisAnimCount = (this->ActionCard_Emphasis_AnimCount / 5);

		// 上
		DrawExtendGraph(
			this->ActionCardArea_Anim_CenterPos.iX + 25,
			this->ActionCardArea_Anim_CenterPos.iY - EmphasisAnimCount - (BATTLE_AREA_HEIGHT / 2) + 20,
			this->ActionCardArea_Anim_CenterPos.iX - 25,
			this->ActionCardArea_Anim_CenterPos.iY - EmphasisAnimCount - (BATTLE_AREA_HEIGHT / 2) - 7,
			*(this->Image_BattleArea_ActionCard_Emphasis), TRUE);

		// 下
		DrawExtendGraph(
			this->ActionCardArea_Anim_CenterPos.iX - 25,
			this->ActionCardArea_Anim_CenterPos.iY + EmphasisAnimCount + (BATTLE_AREA_HEIGHT / 2) - 20,
			this->ActionCardArea_Anim_CenterPos.iX + 25,
			this->ActionCardArea_Anim_CenterPos.iY + EmphasisAnimCount + (BATTLE_AREA_HEIGHT / 2) + 7,
			*(this->Image_BattleArea_ActionCard_Emphasis), TRUE);

		// 右
		DrawModiGraph(
			this->ActionCardArea_Anim_CenterPos.iX + (BATTLE_AREA_WIDE / 2) - 20 + EmphasisAnimCount,		this->ActionCardArea_Anim_CenterPos.iY + 25,
			this->ActionCardArea_Anim_CenterPos.iX + (BATTLE_AREA_WIDE / 2) - 20 + EmphasisAnimCount,		this->ActionCardArea_Anim_CenterPos.iY - 25,
			this->ActionCardArea_Anim_CenterPos.iX + (BATTLE_AREA_WIDE / 2) + 7 + EmphasisAnimCount,		this->ActionCardArea_Anim_CenterPos.iY - 25,
			this->ActionCardArea_Anim_CenterPos.iX + (BATTLE_AREA_WIDE / 2) + 7 + EmphasisAnimCount,		this->ActionCardArea_Anim_CenterPos.iY + 25,
			*(this->Image_BattleArea_ActionCard_Emphasis), TRUE
		);

		// 左
		DrawModiGraph(
			this->ActionCardArea_Anim_CenterPos.iX - (BATTLE_AREA_WIDE / 2) + 20 - EmphasisAnimCount,		this->ActionCardArea_Anim_CenterPos.iY + 25,
			this->ActionCardArea_Anim_CenterPos.iX - (BATTLE_AREA_WIDE / 2) + 20 - EmphasisAnimCount,		this->ActionCardArea_Anim_CenterPos.iY - 25,
			this->ActionCardArea_Anim_CenterPos.iX - (BATTLE_AREA_WIDE / 2) - 7 - EmphasisAnimCount,		this->ActionCardArea_Anim_CenterPos.iY - 25,
			this->ActionCardArea_Anim_CenterPos.iX - (BATTLE_AREA_WIDE / 2) - 7 - EmphasisAnimCount,		this->ActionCardArea_Anim_CenterPos.iY + 25,
			*(this->Image_BattleArea_ActionCard_Emphasis), TRUE
		);
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

/* 山札、トラッシュの枚数描写 */
void Scene_Battle::Draw_Number()
{
	// 枠のオフセット
	const int offsets[8][2] = {
		{ -3, -3 }, {  0, -3 },	{  3, -3 },
		{ -3,  0 },				{  3,  0 },
		{ -3,  3 }, {  0,  3 }, {  3,  3 }
	};

	/* 山札の枚数の描写 */
	if (this->NumberDrawFlg_Deck)
	{
		std::string DeckText = std::to_string(this->pDataList_Battle->GetDeckCardList().size()) + "枚";
		int x = SCREEN_SIZE_WIDE - 50 - 20;
		int y = SCREEN_SIZE_HEIGHT / 2 + 100 - 10;

		// 黒枠を描写
		for (int i = 0; i < 8; ++i) {
			DrawStringToHandle(
				x + offsets[i][0],
				y + offsets[i][1],
				DeckText.c_str(),
				GetColor(0, 0, 0),
				giFont_JF_Dot_MPlus10_20
			);
		}
		// 本体（白色）を描写
		DrawStringToHandle(
			x,
			y,
			DeckText.c_str(),
			GetColor(255, 255, 255),
			giFont_JF_Dot_MPlus10_20
		);
	}

	/* 捨て札の枚数の描写 */
	if (this->NumberDrawFlg_Trash)
	{
		std::string TrashText = std::to_string(this->pDataList_Battle->GetTrashCardList().size()) + "枚";
		int x = 100 - 20;
		int y = SCREEN_SIZE_HEIGHT / 2 + 100 - 10;

		// 黒枠を描写
		for (int i = 0; i < 8; ++i) {
			DrawStringToHandle(
				x + offsets[i][0],
				y + offsets[i][1],
				TrashText.c_str(),
				GetColor(0, 0, 0),
				giFont_JF_Dot_MPlus10_20
			);
		}
		// 本体（白色）を描写
		DrawStringToHandle(
			x,
			y,
			TrashText.c_str(),
			GetColor(255, 255, 255),
			giFont_JF_Dot_MPlus10_20
		);
	}
}

// チェイン数の描写
void Scene_Battle::Draw_ChainNo()
{
	// 枠のオフセット
	const int offsets[8][2] = {
		{ -2, -2 }, {  0, -2 },	{  2, -2 },
		{ -2,  0 },				{  2,  0 },
		{ -2,  2 }, {  0,  2 }, {  2,  2 }
	};

	/* 各バトルエリアのチェイン数を描写 */
	for (int i = 0; i < DataList_Battle::BATTLE_AREA_MAX; i++)
	{
		/* 設定されたカードを取得 */
		auto BattleCard = this->pDataList_Battle->GetBattleAreaCardList(i);

		/* カードが設定されているか確認 */
		if (BattleCard != nullptr)
		{
			/* チェイン数を取得 */
			int ChainNo = BattleCard->GetNowChainCount();

			/* チェイン数が0より大きいなら描写する */
			if (ChainNo > 0)
			{
				/* 枠を描写 */
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128); // 半透明モードを設定(0-255, 128=50%透明)
				DrawBox(
					BattleCard->GetNowPos().iX - 65, BattleCard->GetNowPos().iY - 5,
					BattleCard->GetNowPos().iX + 65, BattleCard->GetNowPos().iY + 25,
					GetColor(0, 0, 0), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードを元に戻す

				std::string ChainText = std::to_string(ChainNo) + "チェイン";
				int x = BattleCard->GetNowPos().iX - 50;
				int y = BattleCard->GetNowPos().iY;

				/* チェイン数が実際の値と異なる場合、再分を描写 */
				int ActualChainNo = BattleCard->MyChainCountGet_Buff();
				if (ChainNo != ActualChainNo)
				{
					// チェイン数の差分を描写
					std::string ChainDiffText = ((ActualChainNo - ChainNo) > 0 ? "+" : "") + std::to_string(ActualChainNo - ChainNo) + "チェイン";
					int diffX = x + 20;	// チェイン数の差分はチェイン数の右側に描写
					int diffY = y + 20;	// チェイン数の差分はチェイン数の右側に描写

					/* 枠を描写 */
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128); // 半透明モードを設定(0-255, 128=50%透明)
					DrawBox(
						diffX - 10, diffY - 5,
						diffX + 110, diffY + 25,
						GetColor(0, 0, 0), TRUE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードを元に戻す

					// 黒枠を描写
					for (int j = 0; j < 8; ++j) {
						DrawStringToHandle(
							diffX + offsets[j][0],
							diffY + offsets[j][1],
							ChainDiffText.c_str(),
							GetColor(0, 0, 0),
							giFont_JF_Dot_MPlus10_20
						);
					}
					// 本体（白色）を描写
					DrawStringToHandle(
						diffX,
						diffY,
						ChainDiffText.c_str(),
						GetColor((ActualChainNo - ChainNo) > 0 ? 0 : 255, (ActualChainNo - ChainNo) > 0 ? 255 : 0, 0), // 増加なら緑、減少なら赤
						giFont_JF_Dot_MPlus10_20
					);
				}

				// 黒枠を描写
				for (int j = 0; j < 8; ++j) {
					DrawStringToHandle(
						x + offsets[j][0],
						y + offsets[j][1],
						ChainText.c_str(),
						GetColor(0, 0, 0),
						giFont_JF_Dot_MPlus10_20
					);
				}
				// 本体（白色）を描写
				DrawStringToHandle(
					x,
					y,
					ChainText.c_str(),
					GetColor(255, 255, 0),
					giFont_JF_Dot_MPlus10_20
				);
			}
		}
	}
}
