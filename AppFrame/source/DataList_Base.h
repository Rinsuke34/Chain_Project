/* すべてのデータリストのベースとなるクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <string>
// 外部ライブラリ
#include <DxLib.h>

// データリストベースクラス
class DataList_Base
{
	public:
		DataList_Base(const std::string& cName);		// コンストラクタ
		virtual ~DataList_Base() {};					// デストラクタ

		/* ゲッター */
		std::string	stGetDataListName()	{ return this->stDataListName; }	// データリスト名称を取得

	private:

		/* 変数 */
		// DataListServerでの管理用データ
		std::string		stDataListName;			// データリスト名称
};
