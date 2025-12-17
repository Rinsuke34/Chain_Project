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

// コンストラクタ
Scene_WoldMap::Scene_WoldMap() : Scene_Base("Scene_WoldMap", 10, false, false)
{
	/* 初期化 */
	this->Image_BackGround = nullptr;	// ワールドマップ背景背景
	this->WoldMapNodeList.clear();		// ワールドマップノードリスト

	/* ノードの仮作成 */
	// エネミーノード
	std::shared_ptr<WoldMap_Node_Enemy> pEnemyNode1 = std::make_shared<WoldMap_Node_Enemy>();
	pEnemyNode1->SetPosition_Map({ 5, 2 });
	this->WoldMapNodeList.push_back(pEnemyNode1);
	std::shared_ptr<WoldMap_Node_Enemy> pEnemyNode2 = std::make_shared<WoldMap_Node_Enemy>();
	pEnemyNode2->SetPosition_Map({ 4, 3 });
	this->WoldMapNodeList.push_back(pEnemyNode2);
	std::shared_ptr<WoldMap_Node_Enemy> pEnemyNode3 = std::make_shared<WoldMap_Node_Enemy>();
	pEnemyNode3->SetPosition_Map({ 6, 3 });
	this->WoldMapNodeList.push_back(pEnemyNode3);

	// ショップノード
	std::shared_ptr<WoldMap_Node_Shop> pShopNode = std::make_shared<WoldMap_Node_Shop>();
	pShopNode->SetPosition_Map({ 5, 4 });
	this->WoldMapNodeList.push_back(pShopNode);

	pEnemyNode1->AddMoveNode(pEnemyNode2);
	pEnemyNode1->AddMoveNode(pEnemyNode3);
	pEnemyNode2->AddMoveNode(pShopNode);
	pEnemyNode3->AddMoveNode(pShopNode);
}

// デストラクタ
Scene_WoldMap::~Scene_WoldMap()
{

}

// 更新
void Scene_WoldMap::Update()
{
	/* 各ノードの更新処理 */
	Node_Update();

	/* 各ノードの中心座標を設定 */
	Node_SetCenterPosition();
}

// 描画
void Scene_WoldMap::Draw()
{
	/* 背景の仮描写 */
	DrawBox(0, 0, SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT, GetColor(0, 100, 200), TRUE);

	/* 各ノードからつながる道を描写 */
	Road_Draw();

	/* 各ノードの描画処理 */
	Node_Draw();
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

		SetPosition.iX = Node->GetPosition_Map().iX * 200;
		SetPosition.iY = Node->GetPosition_Map().iY * 200;

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
