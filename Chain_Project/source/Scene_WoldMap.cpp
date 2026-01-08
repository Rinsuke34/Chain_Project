/* シーン"ワールドマップ画面"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_WoldMap.h"
// 標準ライブラリ
#include <iostream>
#include <fstream>
// 共通定義
#include "FunctionDefine.h"
// 関連クラス
#include "Scene_GameOver.h"
#include "DataList_Image.h"
#include "DataList_GameResource.h"
#include "WoldMap_Node_Base.h"
#include "Card_Base.h"
#include "Card_NextArea.h"
#include "WoldMap_Node_Enemy.h"
#include "WoldMap_Node_Shop.h"
#include "WoldMap_Node_Boss.h"

// コンストラクタ
Scene_WoldMap::Scene_WoldMap() : Scene_Base("Scene_WoldMap", 10, false, false)
{
	/* 初期化 */
	this->WoldMapNodeList.clear();												// ワールドマップノードリスト
	this->WoldMapDrawPos	= { WOLDMAP_DRAW_START_POS_X, WOLDMAP_DRAW_POS_Y };	// ワールドマップの描写座標
	this->NowNode			= nullptr;											// 現在のノード
	this->GameOverCreateFlg	= false;											// ゲームオーバーシーン作成フラグ
	// 画像
	this->Image_WoldMap	= MakeScreen(WOLDMAP_IMAGE_WIDTH, WOLDMAP_IMAGE_HEIGHT, TRUE);	// ワールドマップの画像

	/* データリスト取得 */
	// ゲームリソース管理用データリスト
	this->pDataList_GameResource = std::dynamic_pointer_cast<DataList_GameResource>(gpDataListServer->GetDataList("DataList_GameResource"));

	/* マップデータの読み込み */
	Load_MapData();

	/* 現在の地点を取得 */
	CheckNowNode();

	/* 移動先エリアカードの作成 */
	NextAreaCard_Create();
}

// デストラクタ
Scene_WoldMap::~Scene_WoldMap()
{
	/* 画像を削除 */
	DeleteGraph(this->Image_WoldMap);
}

// 更新
void Scene_WoldMap::Update()
{
	/* ステージクリアの確認 */
	CheckStageEnd();

	/* ワールドマップの描写座標更新 */
	Update_DrawPos();

	/* 各ノードの更新処理 */
	Node_Update();

	/* 各ノードの中心座標を設定 */
	Node_SetCenterPosition();

	/* 画像の更新 */
	Update_Image();

	/* 移動先エリアカードの更新 */
	NextAreaCard_Update();

	/* カード選択 */
	Select_Card();
}

// 描画
void Scene_WoldMap::Draw()
{
	/* ワールドマップの描写 */
	WoldMap_Draw();

	/* 移動先エリアカードの描画 */
	NextAreaCard_Draw();
}

// 現在のノードを確認
void Scene_WoldMap::CheckNowNode()
{
	/* 現在地点のノードを取得 */
	for (auto& Node : this->WoldMapNodeList)
	{
		if (Node->GetNodeState() == WoldMap_Node_Base::NODE_STATE_PLAYER_POS)
		{
			/* 現在地点のノードを設定 */
			this->NowNode = Node;
			Node_SetResource();

			break;
		}
	}
}

// 移動先エリアカードの作成
void Scene_WoldMap::NextAreaCard_Create()
{
	/* 移動先エリアカードリストをクリア */
	this->NextAreaCardList.clear();

	for(auto& MoveNode : this->NowNode->GetMoveNodeList())
	{
		/* 移動先エリアカードを作成 */
		std::shared_ptr<Card_NextArea> NewCard = std::make_shared<Card_NextArea>(MoveNode->GetNodeType());
		NewCard->UpdateImage();
		NewCard->SetNextAreaNode(MoveNode);

		/* 作成したカードをリストに追加 */
		this->NextAreaCardList.push_back(NewCard);
	}
}

// 各ノードの更新処理
void Scene_WoldMap::Node_Update()
{
	/* ワールドマップのノードを更新 */
	for (auto& Node : this->WoldMapNodeList)
	{
		Node->Update();
	}
}

// 各ノードの描画処理
void Scene_WoldMap::Node_Draw()
{
	/* ワールドマップのノードを描写 */
	for (auto& Node : this->WoldMapNodeList)
	{
		Node->Draw();
	}
}

// 各ノードの中心座標を設定
void Scene_WoldMap::Node_SetCenterPosition()
{
	/* ワールドマップノードの中心座標を設定 */
	for (auto& Node : this->WoldMapNodeList)
	{
		/* 対象のノードの中心座標を算出 */
		Struct_2D::POSITION SetPosition = { 0, 0 };

		SetPosition.iX = Node->GetPosition_Map().iX * WOLDMAP_NODE_INTERVAL_X + (WOLDMAP_IMAGE_WIDTH / 2);
		SetPosition.iY = Node->GetPosition_Map().iY * WOLDMAP_NODE_INTERVAL_Y;

		/* 対象のノードの中心座標を設定 */
		Node->SetPosition_Now(SetPosition);
	}
}

// 各ノードからつながる道を描写
void Scene_WoldMap::Road_Draw()
{
	/* 各ノードからつながる道を描写 */
	for (auto& Node : this->WoldMapNodeList)
	{
		/* 移動可能なノードを取得 */
		std::vector<std::shared_ptr<WoldMap_Node_Base>> MoveNodeList = Node->GetMoveNodeList();

		/* 道を描写 */
		for (auto& MoveNode : MoveNodeList)
		{
			/* 道の開始位置、終了位置を算出 */
			Struct_2D::POSITION StartPos	= Node->GetPosition_Now();
			Struct_2D::POSITION EndPos		= {
				MoveNode->GetPosition_Now().iX,
				MoveNode->GetPosition_Now().iY
			};

			/* 道を描写 */
			DRAW_FUNCTION::DrawRoad(StartPos, EndPos);
		}
	}
}

// 画像の更新
void Scene_WoldMap::Update_Image()
{
	/* 描写先をワールドマップの画像に設定 */
	SetDrawScreen(this->Image_WoldMap);

	/* 画像をクリア */
	ClearDrawScreen();

	/* 背景描写 */
	BackGround_Draw();

	/* 各ノードからつながる道を描写 */
	Road_Draw();

	/* 各ノードの描画処理 */
	Node_Draw();

	/* 描写先を裏画面に戻す */
	SetDrawScreen(DX_SCREEN_BACK);
}

// 描写座標の更新
void Scene_WoldMap::Update_DrawPos()
{
	/* ワールドマップが有効であるか確認 */
	if (this->pDataList_GameResource->GetWoldMapActiveFlg())
	{
		// 有効である場合
		/* 描写座標まで移動が完了しているか確認 */
		if (this->WoldMapDrawPos.iX > WOLDMAP_DRAW_POS_X)
		{
			// 完了していない場合
			/* 描写座標を移動量分移動させる */
			this->WoldMapDrawPos.iX -= WOLDMAP_DRAW_POS_MOVE_SPEED;

			/* 全移動先カードのワールドマップの移動完了フラグを無効に設定する */
			for (auto& Card : this->NextAreaCardList)
			{
				Card->SetWoldMapMoveEndFlg(false);
			}
		}
		else
		{
			// 完了している場合
			/* 描写座標に固定する */
			this->WoldMapDrawPos.iX = WOLDMAP_DRAW_POS_X;

			/* 全移動先カードのワールドマップの移動完了フラグを有効に設定する */
			for (auto& Card : this->NextAreaCardList)
			{
				Card->SetWoldMapMoveEndFlg(true);
			}
		}
	}
	else
	{
		// 無効である場合
		/* 画面外まで移動が完了しているか確認 */
		if (this->WoldMapDrawPos.iX < WOLDMAP_DRAW_POS_X + SCREEN_SIZE_WIDE)
		{
			// 完了していない場合
			/* 描写座標を移動量分移動させる */
			this->WoldMapDrawPos.iX += WOLDMAP_DRAW_POS_MOVE_SPEED;
		}
		else
		{
			// 完了している場合
			/* 描写座標に固定する */
			this->WoldMapDrawPos.iX = WOLDMAP_DRAW_POS_X + SCREEN_SIZE_WIDE;
		}
	}
}

// ワールドマップの描写
void Scene_WoldMap::WoldMap_Draw()
{
	/* ワールドマップの描写 */
	DrawGraph(this->WoldMapDrawPos.iX, this->WoldMapDrawPos.iY, this->Image_WoldMap, TRUE);
}

// 移動先エリアカードの更新
void Scene_WoldMap::NextAreaCard_Update()
{
	/* ワールドマップが無効であるなら処理を行わない */
	if (!this->pDataList_GameResource->GetWoldMapActiveFlg())
	{
		return;
	}

	/* 移動先エリアカードの総数を取得 */
	int NextAreaCardCount = static_cast<int>(this->NextAreaCardList.size());

	/* カードの位置設定処理 */
	for (int i = 0; i < NextAreaCardCount; i++)
	{
		/* 設定座標の算出 */
		Struct_2D::POSITION SetPosition = {
			NEXTAREA_CARD_CENTER_X + (i - (NextAreaCardCount - 1) / 2.0f) * NEXTAREA_CARD_INTERVAL,
			NEXTAREA_CARD_POS_Y
		};

		/* 設定座標の設定 */
		this->NextAreaCardList[i]->SetSettingPos(SetPosition);

		/* 更新処理 */
		this->NextAreaCardList[i]->Update();
	}
}

// 移動先エリアカードの描画
void Scene_WoldMap::NextAreaCard_Draw()
{
	/* ワールドマップが無効であるなら処理を行わない */
	if (!this->pDataList_GameResource->GetWoldMapActiveFlg())
	{
		return;
	}

	/* カードの描写処理 */
	for (auto& Card : this->NextAreaCardList)
	{
		Card->Draw();
	}
}

// 背景描写
void Scene_WoldMap::BackGround_Draw()
{
	/* 画像管理データリストを取得 */
	std::shared_ptr<DataList_Image> pDataList_Image = std::dynamic_pointer_cast<DataList_Image>(gpDataListServer->GetDataList("DataList_Image"));

	/* 背景の枠の画像を取得 */
	std::string ImageFilePath = "UI/Button/Button_Frame_Corner_Over";
	std::shared_ptr<int> Image_Frame_Corner = pDataList_Image->iGetImageHandle(ImageFilePath);
	ImageFilePath = "UI/Button/Button_Frame_Line_Over";
	std::shared_ptr<int> Image_Frame_Line	= pDataList_Image->iGetImageHandle(ImageFilePath);
	ImageFilePath = "UI/Button/Button_Frame_Inside_Over";
	std::shared_ptr<int> Image_Frame_Inside	= pDataList_Image->iGetImageHandle(ImageFilePath);

	/* 背景、フレームの描写 */
	DRAW_FUNCTION::DrawFrame_Image(
		{ WOLDMAP_IMAGE_WIDTH / 2, WOLDMAP_IMAGE_HEIGHT / 2 },
		{ WOLDMAP_IMAGE_WIDTH - (WOLDMAP_FRAME_THICKNESS * 2), WOLDMAP_IMAGE_HEIGHT - (WOLDMAP_FRAME_THICKNESS * 2) },
		WOLDMAP_FRAME_THICKNESS,
		*(Image_Frame_Corner),
		*(Image_Frame_Line),
		*(Image_Frame_Inside)
	);
}

// カード選択
void Scene_WoldMap::Select_Card()
{
	/* ワールドマップが無効であるなら処理を行わない */
	if (!this->pDataList_GameResource->GetWoldMapActiveFlg())
	{
		return;
	}

	/* 左クリックが行われたのかの確認 */
	if (gstKeyboardInputData.igInput[INPUT_TRG] & MOUSE_INPUT_LEFT)
	{
		// 左クリックが行われた場合
		for (auto& Card : this->NextAreaCardList)
		{
			if(Card->MouseInCard())
			{
				// カードにカーソルが重なっている場合
				/* 現在のノードの状態を"クリア済み"に設定する */
				this->NowNode->SetNodeState(WoldMap_Node_Base::NODE_STATE_CLEARED);

				/* 移動先ノードの状態をプレイヤー現在位置に設定 */
				std::shared_ptr<WoldMap_Node_Base> MoveNode = Card->GetNextAreaNode();

				if (MoveNode != nullptr)
				{
					/* 現在のノードを更新 */
					this->NowNode = MoveNode;

					/* 変更後のノードを"プレイヤー現在位置"に設定する */
					MoveNode->SetNodeState(WoldMap_Node_Base::NODE_STATE_PLAYER_POS);

					/* 現在の地点を再取得 */
					CheckNowNode();

					/* 移動先エリアカードの再作成 */
					NextAreaCard_Create();

					/* ゲームリソース管理に現在のノード情報を設定 */
					Node_SetResource();

					/* ワールドマップを無効状態に設定する */
					this->pDataList_GameResource->SetWoldMapActiveFlg(false);

					/* ステージ終了フラグを有効にする */
					this->pDataList_GameResource->SetStageEndFlg(true);

					/* 次のステージの選択を完了状態に設定する */
					this->pDataList_GameResource->SetNextStageSelectedFlg(true);
				}

				/* 処理終了 */
				break;
			}
		}
	}
}

// マップデータの読み込み
void Scene_WoldMap::Load_MapData()
{
	/* マップデータのJSONファイルを開く */
	std::ifstream FileName("resource/SetupData/MapData.json");

	nlohmann::json MapData;
	FileName >> MapData;

	for (const auto& item : MapData)
	{
		/* 概要の飲み込み */
		int NodeType = item["Type"];
		Struct_2D::POSITION MapPos = {
			item["Position"][0],
			item["Position"][1]
		};
		std::vector<Struct_2D::POSITION> MoveNodePosList;
		if (item.contains("MovePosition"))
		{
			for (const auto& moveItem : item["MovePosition"])
			{
				Struct_2D::POSITION MovePos = {
					moveItem[0],
					moveItem[1]
				};
				MoveNodePosList.push_back(MovePos);
			}
		}
		int State = item["State"];
		int NodeLevel = item["NodeLevel"];

		/* ノードの作成 */
		std::shared_ptr<WoldMap_Node_Base> NewNode = nullptr;
		switch (NodeType)
		{
			// エネミーノード
			case WoldMap_Node_Base::NODE_TYPE_ENEMY:
				NewNode = std::make_shared<WoldMap_Node_Enemy>();
				break;

			// ショップノード
			case WoldMap_Node_Base::NODE_TYPE_SHOP:
				NewNode = std::make_shared<WoldMap_Node_Shop>();
				break;

			// ボスノード
			case WoldMap_Node_Base::NODE_TYPE_BOSS:
				NewNode = std::make_shared<WoldMap_Node_Boss>();
				break;
		}

		/* ノードの各種設定 */
		if (NewNode != nullptr)
		{
			NewNode->SetPosition_Map(MapPos);
			NewNode->SetMoveNodePosList(MoveNodePosList);
			NewNode->SetNodeState(State);
			NewNode->SetNodeLevel(NodeLevel);
			this->WoldMapNodeList.push_back(NewNode);
		}
	}

	/* 各ノードの移動先ノードを設定 */
	for (auto& Node : this->WoldMapNodeList)
	{
		/* ノードの移動先ノード座標リストを取得 */
		std::vector<Struct_2D::POSITION> MoveNodePosList = Node->GetMoveNodePosList();

		/* 移動先座標と一致しているノードを探し、発見できたら移動先ノードとして登録する */
		for (const auto& MovePos : MoveNodePosList)
		{
			for (auto& TargetNode : this->WoldMapNodeList)
			{
				if (TargetNode->GetPosition_Map().iX == MovePos.iX &&
					TargetNode->GetPosition_Map().iY == MovePos.iY)
				{
					// 移動先ノードを発見した場合
					Node->AddMoveNode(TargetNode);
					break;
				}
			}
		}
	}
}

// ゲームリソース管理に現在のノード情報を設定
void Scene_WoldMap::Node_SetResource()
{
	this->pDataList_GameResource->SetNowMapNode(this->NowNode);
}

// ステージクリアの確認
void Scene_WoldMap::CheckStageEnd()
{
	/* ワールドマップが無効、あるいはすでにゲームオーバーシーンが作成済みであるなら処理を行わない */
	if (!this->pDataList_GameResource->GetWoldMapActiveFlg() || this->GameOverCreateFlg == true)
	{
		return;
	}

	/* 移動先がないならステージクリアとする */
	if (this->NextAreaCardList.size() == 0)
	{
		/* シーン"ゲームオーバー"を作成 */
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_GameOver>());
		this->GameOverCreateFlg = true;
	}
}
