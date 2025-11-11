/* データリストのベースとなるクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "DataList_Base.h"

// コンストラクタ
DataList_Base::DataList_Base(const std::string& cName)
{
	// 引数
	// cName					<- データリストの名前

	/* 初期化処理 */
	// DataListServerでの管理用データ
	this->stDataListName	= cName;					// データリスト名を設定
}
