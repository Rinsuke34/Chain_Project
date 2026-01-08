/* シーン"ゲーム進行管理"の定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_GameManager.h"
// 共通定義
#include "FunctionDefine.h"
// 関連クラス
#include "Scene_WoldMap.h"
#include "DataList_GameResource.h"
#include "WoldMap_Node_Base.h"
#include "Scene_Battle.h"
#include "Scene_Shop.h"
#include "Scene_Title.h"

// コンストラクタ
Scene_GameManager::Scene_GameManager() : Scene_Base("Scene_GameManager", 5, false, false)
{
	/* データリスト"ゲームリソース管理"を作成 */
	this->pDataList_GameResource = std::make_shared<DataList_GameResource>();
	gpDataListServer->AddDataList(this->pDataList_GameResource);

	/* シーン(ワールドマップ)を作成 */
	gpSceneServer->AddSceneReservation(std::make_shared<Scene_WoldMap>());
}

// デストラクタ
Scene_GameManager::~Scene_GameManager()
{
	/* データリスト"ゲームリソース管理"を削除 */
	gpDataListServer->DeleteDataList("DataList_GameResource");
}

// 更新
void Scene_GameManager::Update()
{
	/* ゲーム終了フラグが有効であるか確認 */
	if (this->pDataList_GameResource->GetGameEndFlg())
	{
		// 有効であるならば
		/* タイトル画面へ遷移する */
		gpSceneServer->SetDeleteCurrentSceneFlg(true);
		LOAD_FUNCTION::AddLoadScene();
		gpSceneServer->AddSceneReservation(std::make_shared<Scene_Title>());

		return;
	}

	/* ステージ終了フラグが有効であるなら現在の位置のマップノードに対応したシーンを作成する */
	if (this->pDataList_GameResource->GetStageEndFlg()
		&& !this->pDataList_GameResource->GetNextStageSelectFlg())
	{
		/* ステージシーンの作成 */
		Create_StageScene();

		/* ステージ終了フラグを無効に設定 */
		this->pDataList_GameResource->SetStageEndFlg(false);
	}
}

// ステージシーンの作成
void Scene_GameManager::Create_StageScene()
{
	/* ノードタイプに応じたシーンを作成する */
	std::shared_ptr<WoldMap_Node_Base>	NowMapNode	= this->pDataList_GameResource->GetNowMapNode();
	std::shared_ptr<Scene_Base>			NewScene	= nullptr;
	switch (NowMapNode->GetNodeType())
	{
		// エネミーノードの場合
		case WoldMap_Node_Base::NODE_TYPE_ENEMY:
			// ステージシーンを作成
			NewScene = std::make_shared<Scene_Battle>();
			std::dynamic_pointer_cast<Scene_Battle>(NewScene)->Character_Setup(NowMapNode->GetNodeLevel(), false);
			break;

		// ショップノードの場合
		case WoldMap_Node_Base::NODE_TYPE_SHOP:
			// ショップシーンを作成
			NewScene = std::make_shared<Scene_Shop>();
			break;

		// ボスノードの場合
		case WoldMap_Node_Base::NODE_TYPE_BOSS:
			// ボスステージシーンを作成
			NewScene = std::make_shared<Scene_Battle>();
			std::dynamic_pointer_cast<Scene_Battle>(NewScene)->Character_Setup(NowMapNode->GetNodeLevel(), true);
			break;
	}

	/* シーンサーバーにシーン追加予約を行う */
	gpSceneServer->AddSceneReservation(NewScene);
}
