/* データリストサーバーの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataListServer.h"
// 関連クラス
#include "DataList_Base.h"
// 共通定義
#include "AppFunctionDefine.h"

// デストラクタ
DataListServer::~DataListServer()
{
	/* 終了処理 */
	// リスト内のすべてのシーンを削除
	DeleteAllDataList();			// すべてのデータリスト削除
}

// データリスト描画(デバッグ用)
void DataListServer::DrawDataList()
{
	/* データリストサーバーに登録されているデータリスト名称の描写 */
	// ※主にデバッグ用途
	int iIndex			= 0;
	int iFrameColor		= GetColor(0, 0, 0);			// 枠の色
	int iStringColor	= GetColor(255, 255, 255);		// 文字の色

	DrawBox(0, iIndex * 20, 200, iIndex * 20 + 20, iFrameColor, TRUE);
	DrawString(0, 0, "データリスト名", iStringColor);
	iIndex++;

	/* データリストサーバーに登録されているすべてのデータリストを描写 */
	for (auto& Data : pstDataList)
	{
		DrawBox(0, iIndex * 20, 200, iIndex * 20 + 20, iFrameColor, TRUE);

		/* データリスト名称の描写 */
		DrawString(0, iIndex * 20, Data->stGetDataListName().c_str(), iStringColor);

		iIndex++;
	}
}

// データリスト追加予約
void DataListServer::AddDataList(std::shared_ptr<DataList_Base> NewDataList)
{
	// ※データリストの追加自体は"AddDataList"関数で行う
	// 引数
	// NewDataList		<-	新しくデータリスト一覧に追加するデータリスト

	this->pstDataList.push_back(NewDataList);	// データリストを追加
}

// データリスト取得
std::shared_ptr<DataList_Base> DataListServer::GetDataList(const std::string& cName)
{
	// 引数
	// cName		<-	取得したいデータリストの名称

	/* データリスト一覧からデータリストを取得する */
	for (auto& DataList : pstDataList)
	{
		/* データリストの名称が一致するか確認する */
		if (DataList->stGetDataListName() == cName)
		{
			// データリストの名称が一致するならば
			/* データリストのインスタンスを返す */
			return DataList;
		}
	}

	// データリストが見つからなかった場合
	/* nullptrを戻り値として返す */
	return nullptr;
}

// データリスト削除
void DataListServer::DeleteDataList(const std::string& cName)
{
	// 引数
	// cName	<-	削除したいシーンの名称

	/* 指定の名称のデータリストを削除 */
	pstDataList.erase(std::remove_if(pstDataList.begin(),pstDataList.end(),[&](std::shared_ptr<DataList_Base> DataList)
	{
		/* データリストの名称が一致するか確認 */
		if (DataList->stGetDataListName() == cName)
		{
			// 一致している場合
			/* メモリを開放する */
			DataList.reset();
			return true;
		}
		else
		{
			// 一致していない場合
			return false;
		}
	}),pstDataList.end());
}

// データリスト削除(データリスト一覧内のすべてのデータリストに対して)
void DataListServer::DeleteAllDataList()
{
	/* データリスト一覧内の全てのデータリストを削除する */
	// データリスト一覧に登録されている全てのデータリストを削除する
	for (auto& DataList : pstDataList)
	{
		DataList.reset();	// メモリを開放する
	}

	/* データリストのクリアを行う */
	pstDataList.clear();
}

