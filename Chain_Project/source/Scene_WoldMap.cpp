/* シーン"ワールドマップ画面"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_WoldMap.h"
// 共通定義
#include "FunctionDefine.h"
// 関連クラス
#include "WoldMap_Node_Base.h"

// テスト
#include "WoldMap_Node_Enemy.h"
#include "WoldMap_Node_Shop.h"
#include "WoldMap_Node_Boss.h"

// コンストラクタ
Scene_WoldMap::Scene_WoldMap() : Scene_Base("Scene_WoldMap", 10, false, false)
{
	/* 初期化 */
	this->WoldMapNodeList.clear();												// ワールドマップノードリスト
	this->WoldMapDrawPos = { WOLDMAP_DRAW_START_POS_X, WOLDMAP_DRAW_POS_Y };	// ワールドマップの描写座標
	// 画像
	this->Image_WoldMap	= MakeScreen(WOLDMAP_IMAGE_WIDTH, WOLDMAP_IMAGE_HEIGHT, TRUE);	// ワールドマップの画像

	/* ノードの仮作成 */
	// エネミーノード
	std::shared_ptr<WoldMap_Node_Enemy> pEnemyNode1 = std::make_shared<WoldMap_Node_Enemy>();
	pEnemyNode1->SetPosition_Map({ 3, 1 });
	this->WoldMapNodeList.push_back(pEnemyNode1);
	std::shared_ptr<WoldMap_Node_Enemy> pEnemyNode2 = std::make_shared<WoldMap_Node_Enemy>();
	pEnemyNode2->SetPosition_Map({ 4, 2 });
	this->WoldMapNodeList.push_back(pEnemyNode2);
	std::shared_ptr<WoldMap_Node_Enemy> pEnemyNode3 = std::make_shared<WoldMap_Node_Enemy>();
	pEnemyNode3->SetPosition_Map({ 2, 2 });
	this->WoldMapNodeList.push_back(pEnemyNode3);

	// ショップノード
	std::shared_ptr<WoldMap_Node_Shop> pShopNode = std::make_shared<WoldMap_Node_Shop>();
	pShopNode->SetPosition_Map({ 3, 3 });
	this->WoldMapNodeList.push_back(pShopNode);

	// ボスノード
	std::shared_ptr<WoldMap_Node_Boss> pBossNode = std::make_shared<WoldMap_Node_Boss>();
	pBossNode->SetPosition_Map({ 3, 4 });
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
}

// 描画
void Scene_WoldMap::Draw()
{
	/* 画像の更新 */
	Update_Image();


	DrawGraph(this->WoldMapDrawPos.iX, this->WoldMapDrawPos.iY, this->Image_WoldMap, TRUE);
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

		SetPosition.iX = Node->GetPosition_Map().iX * WOLDMAP_NODE_INTERVAL_X;
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

	/* 背景の仮描写 */
	DrawBox(0, 0, WOLDMAP_IMAGE_WIDTH, WOLDMAP_IMAGE_HEIGHT, GetColor(0, 100, 200), TRUE);

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
