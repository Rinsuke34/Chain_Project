/* 実行中のシーンの管理を行うクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <list>
#include <memory>
#include <string>

/* 前方宣言 */
class Scene_Base;

// シーンサーバークラス
class SceneServer
{
	public:
		SceneServer();				// コンストラクタ
		virtual ~SceneServer();		// デストラクタ

		/* 関数 */
		void	SceneProcess();		// シーン計算処理
		void	SceneDraw();		// シーン描画処理
		void	DrawSceneList();	// データリスト描画(デバッグ用)

		/* セッター */
		void	AddSceneReservation(std::shared_ptr<Scene_Base> NewScene);															// シーン追加予約
		void	SetDeleteCurrentSceneFlg(bool bDeleteCurrentSceneFlg)	{ this->bDeleteCurrentSceneFlg = bDeleteCurrentSceneFlg; };	// シーン追加時の現行シーン削除フラグ設定

		/* ゲッター */
		std::shared_ptr<Scene_Base>	GetScene(const std::string& cName);		// シーン取得
		
	private:
		/* シーンリスト */
		std::list<std::shared_ptr<Scene_Base>> pstSceneList;

		/* 追加予定のシーンリスト */
		std::list<std::shared_ptr<Scene_Base>> pstAddSceneList;

		/* 関数 */
		void	AddScene();						// シーンリストへのシーン追加
		void	SceneSortLayerOrder();			// シーンリストの並び替え(レイヤー順序の大きい順)
		void	DeleteUnnecessaryScene();		// シーン削除(削除フラグが有効なシーンに対して)
		void	DeleteAllScene();				// シーン削除(シーンリスト内のすべてのシーンに対して)
		void	DeleteAllAddScene();			// シーン削除(追加予定のシーンリスト内のすべてのシーンに対して)

		/* 変数 */
		bool	bSceneDeleteFlg;			// シーン削除フラグ(削除予定のシーンがあるならば有効にする)
		bool	bSceneAddFlg;				// シーン追加フラグ(追加予定のシーンがあるならば有効にする)
		bool	bDeleteCurrentSceneFlg;		// 現行シーン削除フラグ(シーンリスト内のすべてのシーンを削除するかのフラグ)
};
