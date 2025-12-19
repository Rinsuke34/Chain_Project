/* シーン"ワールドマップ画面"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_WoldMap.h"
// 共通定義
#include "FunctionDefine.h"
// 関連クラス
#include "DataList_Image.h"
#include "WoldMap_Node_Base.h"
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
	// 画像
	this->Image_WoldMap	= MakeScreen(WOLDMAP_IMAGE_WIDTH, WOLDMAP_IMAGE_HEIGHT, TRUE);	// ワールドマップの画像

	/* ノードの仮作成 */
	{
		// エネミーノード
		std::shared_ptr<WoldMap_Node_Enemy> pEnemyNode1 = std::make_shared<WoldMap_Node_Enemy>();
		pEnemyNode1->SetPosition_Map({ 0, 1 });
		this->WoldMapNodeList.push_back(pEnemyNode1);
		std::shared_ptr<WoldMap_Node_Enemy> pEnemyNode2 = std::make_shared<WoldMap_Node_Enemy>();
		pEnemyNode2->SetPosition_Map({ 1, 2 });
		this->WoldMapNodeList.push_back(pEnemyNode2);
		std::shared_ptr<WoldMap_Node_Enemy> pEnemyNode3 = std::make_shared<WoldMap_Node_Enemy>();
		pEnemyNode3->SetPosition_Map({ -1, 2 });
		this->WoldMapNodeList.push_back(pEnemyNode3);

		// ショップノード
		std::shared_ptr<WoldMap_Node_Shop> pShopNode = std::make_shared<WoldMap_Node_Shop>();
		pShopNode->SetPosition_Map({ 0, 3 });
		this->WoldMapNodeList.push_back(pShopNode);

		// ボスノード
		std::shared_ptr<WoldMap_Node_Boss> pBossNode = std::make_shared<WoldMap_Node_Boss>();
		pBossNode->SetPosition_Map({ 0, 4 });
		this->WoldMapNodeList.push_back(pBossNode);

		// ルート設定
		pEnemyNode1->AddMoveNode(pEnemyNode2);
		pEnemyNode1->AddMoveNode(pEnemyNode3);
		pEnemyNode2->AddMoveNode(pShopNode);
		pEnemyNode3->AddMoveNode(pShopNode);
		pShopNode->AddMoveNode(pBossNode);

		// スタート地点の設定
		pEnemyNode1->SetNodeState(WoldMap_Node_Base::NODE_STATE_PLAYER_POS);
	}

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
			this->NowNode = Node;
			break;
		}
	}
}

// 移動先エリアカードの作成
void Scene_WoldMap::NextAreaCard_Create()
{
	for(auto& MoveNode : this->NowNode->GetMoveNodeList())
	{
		/* 移動先エリアカードを作成 */
		std::shared_ptr<Card_NextArea> NewCard = std::make_shared<Card_NextArea>(MoveNode->GetNodeType());
		NewCard->UpdateImage();

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
	/* 描写座標まで移動が完了しているか確認 */
	if (this->WoldMapDrawPos.iX > WOLDMAP_DRAW_POS_X)
	{
		// 完了していない場合
		/* 描写座標を移動量分移動させる */
		this->WoldMapDrawPos.iX -= WOLDMAP_DRAW_POS_MOVE_SPEED;
	}
	else
	{
		// 完了している場合
		/* 描写座標に固定する */
		this->WoldMapDrawPos.iX = WOLDMAP_DRAW_POS_X;
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
