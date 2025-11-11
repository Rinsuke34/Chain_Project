/* シーンサーバーの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "SceneServer.h"
// 関連クラス
#include "Scene_Base.h"
// 共通定義
#include "AppFunctionDefine.h"

// コンストラクタ
SceneServer::SceneServer()
{
	/* 初期化 */
	this->bSceneDeleteFlg			= false;	// シーン削除フラグ
	this->bSceneAddFlg				= false;	// シーン追加フラグ
	this->bDeleteCurrentSceneFlg	= false;	// 現行シーン削除フラグ
}

// デストラクタ
SceneServer::~SceneServer()
{
	/* 終了処理 */
	// リスト内のすべてのシーンを削除
	DeleteAllScene();		// シーンリスト
	DeleteAllAddScene();	// 追加予定のシーンリスト
}

// シーン計算処理
void SceneServer::SceneProcess()
{
	/* レイヤー順序が高いシーンから計算を行う */
	for (auto& Scene : pstSceneList)
	{
		/* シーンの更新処理 */
		Scene->Update();

		/* 対象シーンの削除フラグの確認 */
		if (Scene->bGetDeleteFlg() == true)
		{
			// 対象シーンの削除フラグが有効であるなら
			/* シーン削除フラグを有効にする */
			this->bSceneDeleteFlg = true;
		}

		/* 対象シーンの下層レイヤー計算停止フラグの確認 */
		if (Scene->bGetLowerLayerUpdateStopFlg() == true)
		{
			// 下層レイヤー計算停止フラグが有効であるなら
			/* シーン計算処理を終了する */
			break;
		}
	}

	/* 削除フラグが有効なシーンを削除 */
	DeleteUnnecessaryScene();

	/* シーンの追加 */
	AddScene();
}

// シーン描画処理
void SceneServer::SceneDraw()
{
	/* 3D用の基本設定を有効化 */
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	/* レイヤー順序が低いシーンから描写を行う */
	for (auto Scene = pstSceneList.rbegin(); Scene != pstSceneList.rend(); ++Scene)
	{
		Scene->get()->Draw();
	}
}

// シーン追加予約
void SceneServer::AddSceneReservation(std::shared_ptr<Scene_Base> NewScene)
{
	// ※シーンの追加自体は"AddScene"関数で行う
	// 引数
	// NewScene			<-	新しくシーンサーバーに登録するシーン

	/* シーン追加フラグの有効化 */
	this->bSceneAddFlg = true;

	/* 追加予定のシーンリストにシーンを追加する */
	this->pstAddSceneList.push_back(NewScene);
}

// シーン取得
std::shared_ptr<Scene_Base> SceneServer::GetScene(const std::string& cName)
{
	// 引数
	// cName		<-	取得したいシーンの名称

	/* シーン一覧からシーンを取得する */
	for (auto& Scene : pstSceneList)
	{
		/* シーンの名称が一致するか確認する */
		if (Scene->stGetSceneName() == cName)
		{
			// シーンの名称が一致するならば
			/* シーンのインスタンスを返す */
			return Scene;
		}
	}

	// シーンが見つからなかった場合
	/* nullptrを戻り値として返す */
	return nullptr;
}


// シーンリストへのシーン追加
void SceneServer::AddScene()
{
	/* シーン追加フラグが有効であるか確認 */
	if (this->bSceneAddFlg == true)
	{
		// シーン追加フラグが有効ならば
		/* 現行シーン削除フラグが有効であるか確認 */
		if (this->bDeleteCurrentSceneFlg == true)
		{
			// 現行シーン削除フラグが有効であるならば
			/* シーンリストのシーンをすべて削除する */
			DeleteAllScene();

			/* 現行シーン削除フラグの無効化 */
			this->bDeleteCurrentSceneFlg = false;
		}

		/* 追加予定のシーンをシーンリストに追加する */
		pstSceneList.splice(pstSceneList.end(), pstAddSceneList);

		/* 追加予定のシーンリストをすべて削除する */
		DeleteAllAddScene();

		/* レイヤー順序が大きい順に並び替える */
		SceneSortLayerOrder();

		/* シーン追加フラグの無効化 */
		this->bSceneAddFlg = false;
	}
}

// シーンリストの並び替え(レイヤー順序の大きい順)
void SceneServer::SceneSortLayerOrder()
{
	/* 各レイヤーが所持する"レイヤー順序"が大きい順に並び替える */
	pstSceneList.sort([](std::shared_ptr<Scene_Base> SceneA, std::shared_ptr<Scene_Base> SceneB)
	{
		return SceneA->iGetSceneLayerOrder() > SceneB->iGetSceneLayerOrder();
	});
}


// シーン削除(削除フラグが有効なシーンに対して)
void SceneServer::DeleteUnnecessaryScene()
{
	/* 削除対象のシーンを削除する */
	// 削除フラグが有効(いずれかのシーンが削除待機状態)であるか
	if (this->bSceneDeleteFlg == true)
	{
		/* 削除フラグが有効なシーンをを削除 */
		pstSceneList.erase( std::remove_if(pstSceneList.begin(), pstSceneList.end(), [](std::shared_ptr<Scene_Base> pScene)
		{
			/* 削除フラグが有効であるか確認　*/
			if (pScene->bGetDeleteFlg() == true)
			{
				// 有効である場合
				/* メモリを解放する */
				pScene.reset();
				return true;
			}
			else
			{
				// 無効である場合
				return false;
			}
		}), pstSceneList.end());

		// シーン削除フラグを元に戻す
		this->bSceneDeleteFlg = false;
	}
}

// シーン削除(シーンリスト内のすべてのシーンに対して)
void SceneServer::DeleteAllScene()
{
	/* シーンリスト内のすべてのシーンを削除する */
	// シーンリストに登録されているすべてのシーンを削除する
	for (auto& Scene : pstSceneList)
	{
		/* メモリを解放する */
		Scene.reset();
	}

	/* シーンリストのクリアを行う */
	pstSceneList.clear();
}

// シーン削除(追加予定のシーンリスト内のすべてのシーンに対して)
void SceneServer::DeleteAllAddScene()
{
	/* 追加予定のシーンリスト内のすべてのシーンを削除する */
	// 追加予定のシーンリストに登録されているすべてのシーンを削除する
	for (auto& Scene : pstAddSceneList)
	{
		/* メモリを解放する */
		Scene.reset();
	}

	/* 追加予定のシーンリストのクリアを行う */
	pstAddSceneList.clear();
}

// データリスト描画(デバッグ用)
void SceneServer::DrawSceneList()
{
	/* シーンリストに登録されているシーン名称の描写 */
	int iIndex = 0;

	DrawBox(0, iIndex * 20, 400, iIndex * 20 + 20, GetColor(0, 255, 0), TRUE);
	DrawString(0, 0, "シーン名", GetColor(255, 255, 255));
	DrawString(200, 0, "レイヤー", GetColor(255, 255, 255));
	iIndex++;
	
	/* シーンリストに登録されているすべてのシーンを描写 */
	for (auto& Scene : pstSceneList)
	{
		DrawBox(0, iIndex * 20, 400, iIndex * 20 + 20, GetColor(0, 255, 0), TRUE);

		/* シーン名称の描写 */
		DrawString(0, iIndex * 20, Scene->stGetSceneName().c_str(), GetColor(255, 255, 255));

		/* シーンレイヤーの描写 */
		int i = Scene->iGetSceneLayerOrder();
		DrawFormatString(200, iIndex * 20, GetColor(255, 255, 255), "%d", i);

		iIndex++;
	}
};

