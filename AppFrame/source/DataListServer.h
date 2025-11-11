/* データ制御処理の管理を行うクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <list>
#include <memory>
#include <string>

/* 前方宣言 */
class DataList_Base;

// データリストサーバークラス
class DataListServer
{
	public:
		DataListServer() {};			// コンストラクタ
		virtual ~DataListServer();		// デストラクタ

		/* 関数 */
		void	DrawDataList();								// データリスト描画(デバッグ用)
		void	DeleteDataList(const std::string& cName);	// 指定データリスト削除

		/* セッター */
		void							AddDataList(std::shared_ptr<DataList_Base> NewDataList);		// データリスト追加

		/* ゲッター */
		std::shared_ptr<DataList_Base>	GetDataList(const std::string& cName);						// データリスト取得

	private:
		// データリスト一覧
		std::list<std::shared_ptr<DataList_Base>> pstDataList;

		/* 関数 */
		void	DeleteAllDataList();			// データリスト削除(データリスト一覧内のすべてのデータリストに対して)

	protected:
};
